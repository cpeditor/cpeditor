#.rst:
# ECMSetupQtPluginMacroNames
# --------------------------
#
# Instruct CMake's automoc about C++ preprocessor macros used to define Qt-style plugins.
#
# ::
#
#   ecm_setup_qtplugin_macro_names(
#       [JSON_NONE <macro_name> [<macro_name> [...]]]
#       [JSON_ARG1 <macro_name> [<macro_name> [...]]]
#       [JSON_ARG2 <macro_name> [<macro_name> [...]]]
#       [JSON_ARG3 <macro_name> [<macro_name> [...]]]
#       [CONFIG_CODE_VARIABLE <variable_name>] )
#
# CMake's automoc needs some support when parsing C++ source files to detect whether moc
# should be run on those files and if there are also dependencies on other files, like those
# with Qt plugin metadata in JSON format. Because automoc just greps overs the raw plain text
# of the sources without any C++ preprocessor-like processing.
# CMake in newer versions provides the variables ``CMAKE_AUTOMOC_DEPEND_FILTERS`` (CMake >= 3.9.0)
# and ``CMAKE_AUTOMOC_MACRO_NAMES`` (CMake >= 3.10) to allow the developer to assist automoc.
#
# This macro cares for the explicit setup needed for those variables for common cases of
# C++ preprocessor macros used for Qt-style plugins.
#
# JSON_NONE lists the names of C++ preprocessor macros for Qt-style plugins which do not refer to
# external files with the plugin metadata.
#
# JSON_ARG1 lists the names of C++ preprocessor macros for Qt-style plugins where the first argument
# to the macro is the name of the external file with the plugin metadata.
#
# JSON_ARG2 is the same as JSON_ARG1 but with the file name being the second argument.
#
# JSON_ARG3 is the same as JSON_ARG1 but with the file name being the third argument.
#
# CONFIG_CODE_VARIABLE specifies the name of the variable which will get set as
# value some generated CMake code for instructing automoc for the given macro names,
# as useful in an installed CMake config file. The variable can then be used as usual in
# the template file for such a CMake config file, by ``@<variable_name>@``.
#
#
# Example usage:
#
# Given some plugin-oriented Qt-based software which defines a custom C++ preprocessor
# macro ``EXPORT_MYPLUGIN`` for declaring the central plugin object:
#
# .. code-block:: c++
#
#   #define EXPORT_MYPLUGIN_WITH_JSON(classname, jsonFile) \
#   class classname : public QObject \
#   { \
#       Q_OBJECT \
#       Q_PLUGIN_METADATA(IID "myplugin" FILE jsonFile) \
#       explicit classname() {} \
#   };
#
# In the CMake buildsystem of the library one calls
#
# .. code-block:: cmake
#
#   ecm_setup_qtplugin_macro_names(
#       JSON_ARG2
#          EXPORT_MYPLUGIN_WITH_JSON
#   )
#
# to instruct automoc about the usage of that macro in the sources of the
# library itself.
#
# Given the software installs a library including the header with the macro
# definition and a CMake config file, so 3rd-party can create additional
# plugins by linking against the library, one passes additionally the name of
# a variable which shall be set as value the CMake code needed to instruct
# automoc about the usage of that macro.
#
# .. code-block:: cmake
#
#   ecm_setup_qtplugin_macro_names(
#       JSON_ARG2
#          EXPORT_MYPLUGIN_WITH_JSON
#       CONFIG_CODE_VARIABLE
#          PACKAGE_SETUP_AUTOMOC_VARIABLES
#   )
#
# This variable then is used in the template file (e.g.
# ``MyProjectConfig.cmake.in``) for the libary's installed CMake config file
# and that way will ensure that in the 3rd-party plugin's buildsystem
# automoc is instructed as well as needed:
#
# ::
#
#   @PACKAGE_SETUP_AUTOMOC_VARIABLES@
#
# Since 5.45.0.

#=============================================================================
# SPDX-FileCopyrightText: 2018 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

include(CMakePackageConfigHelpers)

macro(ecm_setup_qtplugin_macro_names)
    set(options )
    set(oneValueArgs CONFIG_CODE_VARIABLE)
    set(multiValueArgs JSON_NONE JSON_ARG1 JSON_ARG2 JSON_ARG3)

    cmake_parse_arguments(ESQMN "${options}" "${oneValueArgs}" "${multiValueArgs}"  ${ARGN})

    if(ESQMN_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown keywords given to ECM_SETUP_QTPLUGIN_MACRO_NAMES(): \"${ESQMN_UNPARSED_ARGUMENTS}\"")
    endif()

    # CMAKE_AUTOMOC_MACRO_NAMES
    if(NOT CMAKE_VERSION VERSION_LESS "3.10.0")
        # CMake 3.9+ warns about automoc on files without Q_OBJECT, and doesn't know about other macros.
        # 3.10+ lets us provide more macro names that require automoc.
        list(APPEND CMAKE_AUTOMOC_MACRO_NAMES
            ${ESQMN_JSON_NONE}
            ${ESQMN_JSON_ARG1}
            ${ESQMN_JSON_ARG2}
            ${ESQMN_JSON_ARG3}
        )
    endif()

    # CMAKE_AUTOMOC_DEPEND_FILTERS
    if(NOT CMAKE_VERSION VERSION_LESS "3.9.0")
        # CMake's automoc needs help to find names of plugin metadata files in case Q_PLUGIN_METADATA
        # is indirectly used via other C++ preprocessor macros
        # 3.9+ lets us provide some filter rule pairs (keyword, regexp) to match the names of such files
        # in the plain text of the sources. See AUTOMOC_DEPEND_FILTERS docs for details.
        foreach(macro_name  ${ESQMN_JSON_ARG1})
            list(APPEND CMAKE_AUTOMOC_DEPEND_FILTERS
                "${macro_name}"
                "[\n^][ \t]*${macro_name}[ \t\n]*\\([ \t\n]*\"([^\"]+)\""
            )
        endforeach()
        foreach(macro_name  ${ESQMN_JSON_ARG2})
            list(APPEND CMAKE_AUTOMOC_DEPEND_FILTERS
                "${macro_name}"
                "[\n^][ \t]*${macro_name}[ \t\n]*\\([^,]*,[ \t\n]*\"([^\"]+)\""
            )
        endforeach()
        foreach(macro_name  ${ESQMN_JSON_ARG3})
            list(APPEND CMAKE_AUTOMOC_DEPEND_FILTERS
                "${macro_name}"
                "[\n^][ \t]*${macro_name}[ \t\n]*\\([^,]*,[^,]*,[ \t\n]*\"([^\"]+)\""
            )
        endforeach()
    endif()

    if (ESQMN_CONFIG_CODE_VARIABLE)
        set(_content
"####################################################################################
# CMAKE_AUTOMOC
")
        set(_all_macro_names
            ${ESQMN_JSON_NONE}
            ${ESQMN_JSON_ARG1}
            ${ESQMN_JSON_ARG2}
            ${ESQMN_JSON_ARG3}
        )
        string(APPEND _content "
if(NOT CMAKE_VERSION VERSION_LESS \"3.10.0\")
    # CMake 3.9+ warns about automoc on files without Q_OBJECT, and doesn't know about other macros.
    # 3.10+ lets us provide more macro names that require automoc.
    list(APPEND CMAKE_AUTOMOC_MACRO_NAMES ${_all_macro_names})
endif()
")

        if(ESQMN_JSON_ARG1 OR ESQMN_JSON_ARG2 OR ESQMN_JSON_ARG3)
            string(APPEND _content "
if(NOT CMAKE_VERSION VERSION_LESS \"3.9.0\")
    # CMake's automoc needs help to find names of plugin metadata files in case Q_PLUGIN_METADATA
    # is indirectly used via other C++ preprocessor macros
    # 3.9+ lets us provide some filter rule pairs (keyword, regexp) to match the names of such files
    # in the plain text of the sources. See AUTOMOC_DEPEND_FILTERS docs for details.
")
            if(ESQMN_JSON_ARG1)
                string(APPEND _content
"    foreach(macro_name  ${ESQMN_JSON_ARG1})
        list(APPEND CMAKE_AUTOMOC_DEPEND_FILTERS
            \"\${macro_name}\"
            \"[\\n^][ \\t]*\${macro_name}[ \\t\\n]*\\\\([ \\t\\n]*\\\"([^\\\"]+)\\\"\"
        )
    endforeach()
")
            endif()
            if(ESQMN_JSON_ARG2)
                string(APPEND _content
"    foreach(macro_name  ${ESQMN_JSON_ARG2})
        list(APPEND CMAKE_AUTOMOC_DEPEND_FILTERS
            \"\${macro_name}\"
            \"[\\n^][ \\t]*\${macro_name}[ \\t\\n]*\\\\([^,]*,[ \\t\\n]*\\\"([^\\\"]+)\\\"\"
        )
    endforeach()
")
            endif()
            if(ESQMN_JSON_ARG3)
                string(APPEND _content
"    foreach(macro_name  ${ESQMN_JSON_ARG3})
        list(APPEND CMAKE_AUTOMOC_DEPEND_FILTERS
            \"\${macro_name}\"
            \"[\\n^][ \\t]*\${macro_name}[ \\t\\n]*\\\\([^,]*,[^,]*,[ \\t\\n]*\\\"([^\\\"]+)\\\"\"
        )
    endforeach()
")
            endif()
            string(APPEND _content "endif()")
        endif()
        string(APPEND _content "
####################################################################################"
        )

        set(${ESQMN_CONFIG_CODE_VARIABLE} ${_content})
    endif()
endmacro()
