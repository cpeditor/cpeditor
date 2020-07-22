#.rst:
# ECMAddQch
# ------------------
#
# This module provides the ``ecm_add_qch`` function for generating API
# documentation files in the QCH format, and the ``ecm_install_qch_export``
# function for generating and installing exported CMake targets for such
# generated QCH files to enable builds of other software with generation of
# QCH files to create links into the given QCH files.
#
# ::
#
#   ecm_add_qch(<target_name>
#       NAME <name>
#       VERSION <version>
#       QCH_INSTALL_DESTINATION <qchfile_install_path>
#       TAGFILE_INSTALL_DESTINATION <tagsfile_install_path>
#       [COMPONENT <component>]
#       [BASE_NAME <basename>]
#       [SOURCE_DIRS <dir> [<dir2> [...]]]
#       [SOURCES <file> [<file2> [...]]]
#       |MD_MAINPAGE <md_file>]
#       [INCLUDE_DIRS <incdir> [<incdir2> [...]]]
#       [IMAGE_DIRS <idir> [<idir2> [...]]]
#       [EXAMPLE_DIRS <edir> [<edir2> [...]]]
#       [ORG_DOMAIN <domain>]
#       [NAMESPACE <namespace>]
#       [LINK_QCHS <qch> [<qch2> [...]]]
#       [PREDEFINED_MACROS <macro[=content]> [<macro2[=content]> [...]]]
#       [BLANK_MACROS <macro> [<macro2> [...]]]
#       [CONFIG_TEMPLATE <configtemplate_file>]
#       [VERBOSE]
#   )
#
# This macro adds a target called <target_name> for the creation of an API
# documentation manual in the QCH format from the given sources.
# It currently uses doxygen, future versions might optionally also allow other
# tools.
# Next to the QCH file the target will generate a corresponding doxygen tag
# file, which enables creating links from other documentation into the
# generated QCH file.
#
# It is recommended to make the use of this macro optional, by depending
# the call to ``ecm_add_qch`` on a CMake option being set, with a name like
# ``BUILD_QCH`` and being TRUE by default. This will allow the developers to
# saves resources on normal source development build cycles by setting this
# option to FALSE.
#
# The macro will set the target properties DOXYGEN_TAGFILE, QHP_NAMESPACE,
# QHP_NAMESPACE_VERSIONED, QHP_VIRTUALFOLDER and LINK_QCHS to the respective
# values, to allow other code access to them, e.g. the macro
# ``ecm_install_qch_export``.
# To enable the use of the target <target_name> as item for LINK_QCHS
# in further ``ecm_add_qch`` calls in the current build,
# additionally a target property DOXYGEN_TAGFILE_BUILD is set, with the path
# of the created doxygen tag file in the build dir.
# If existing, ``ecm_add_qch`` will use this property instead of
# DOXYGEN_TAGFILE for access to the tags file.
#
# NAME specifies the name for the generated documentation.
#
# VERSION specifies the version of the library for which the documentation is
# created.
#
# BASE_NAME specifies the base name for the generated files.
# The default basename is ``<name>``.
#
# SOURCE_DIRS specifies the dirs (incl. subdirs) with the source files for
# which the API documentation should be generated.  Dirs can be relative to
# the current source dir. Dependencies to the files in the dirs are not
# tracked currently, other than with the SOURCES argument. So do not use for
# sources generated during the build.
# Needs to be used when SOURCES or CONFIG_TEMPLATE are not used.
#
# SOURCES specifies the source files for which the API documentation should be
# generated.
# Needs to be used when SOURCE_DIRS or CONFIG_TEMPLATE are not used.
#
# MD_MAINPAGE specifies a file in Markdown format that should be used as main
# page. This page will overrule any ``\mainpage`` command in the included
# sources.
#
# INCLUDE_DIRS specifies the dirs which should be searched for included
# headers. Dirs can be relative to the current source dir. Since 5.63.
#
# IMAGE_DIRS specifies the dirs which contain images that are included in the
# documentation. Dirs can be relative to the current source dir.
#
# EXAMPLE_DIRS specifies the dirs which contain examples that are included in
# the documentation. Dirs can be relative to the current source dir.
#
# QCH_INSTALL_DESTINATION specifies where the generated QCH file will be
# installed.
#
# TAGFILE_INSTALL_DESTINATION specifies where the generated tag file will be
# installed.
#
# COMPONENT specifies the installation component name with which the install
# rules for the generated QCH file and tag file are associated.
#
# NAMESPACE can be used to set a custom namespace <namespace> of the generated
# QCH file. The namepspace is used as the unique id by QHelpEngine (cmp.
# https://doc.qt.io/qt-5/qthelpproject.html#namespace).
# The default namespace is ``<domain>.<name>``.
# Needs to be used when ORG_DOMAIN is not used.
#
# ORG_DOMAIN can be used to define the organization domain prefix for the
# default namespace of the generated QCH file.
# Needs to be used when NAMESPACE is not used.
#
# LINK_QCHS specifies a list of other QCH targets which should be used for
# creating references to API documentation of code in external libraries.
# For each target <qch> in the list these target properties are expected to be
# defined: DOXYGEN_TAGFILE, QHP_NAMESPACE and QHP_VIRTUALFOLDER.
# If any of these is not existing, <qch> will be ignored.
# Use the macro ``ecm_install_qch_export`` for exporting a target with these
# properties with the CMake config of a library.
# Any target <qch> can also be one created before in the same buildsystem by
# another call of ``ecm_add_qch``.
#
# PREDEFINED_MACROS specifies a list of C/C++ macros which should be handled as
# given by the API dox generation tool.
# Examples are macros only defined in generated files, so whose
# definition might be not available to the tool.
#
# BLANK_MACROS specifies a list of C/C++ macro names which should be ignored by
# the API dox generation tool and handled as if they resolve to empty strings.
# Examples are export macros only defined in generated files, so whose
# definition might be not available to the tool.
#
# CONFIG_TEMPLATE specifies a custom cmake template file for the config file
# that is created to control the execution of the API dox generation tool.
# The following CMake variables need to be used:
# ECM_QCH_DOXYGEN_QHELPGENERATOR_EXECUTABLE,
# ECM_QCH_DOXYGEN_FILEPATH, ECM_QCH_DOXYGEN_TAGFILE.
# The following CMake variables can be used:
# ECM_QCH_DOXYGEN_PROJECTNAME, ECM_QCH_DOXYGEN_PROJECTVERSION,
# ECM_QCH_DOXYGEN_VIRTUALFOLDER, ECM_QCH_DOXYGEN_FULLNAMESPACE,
# ECM_QCH_DOXYGEN_TAGFILES,
# ECM_QCH_DOXYGEN_WARN_LOGFILE, ECM_QCH_DOXYGEN_QUIET.
# There is no guarantue that the other CMake variables currently used in the
# default config file template will also be present with the same semantics
# in future versions of this macro.
#
# VERBOSE tells the API dox generation tool to be more verbose about its
# activity.
#
# The default config file for the API dox generation tool, so the one when not
# using CONFIG_TEMPLATE, allows code to handle the case of being processed by
# the tool by defining the C/C++ preprocessor macro ``K_DOXYGEN`` when run
# (since v5.67.0). For backward-compatibility also the definition
# ``DOXYGEN_SHOULD_SKIP_THIS`` is set, but its usage is deprecated.
#
# Example usage:
#
# .. code-block:: cmake
#
#   ecm_add_qch(
#       MyLib_QCH
#       NAME MyLib
#       VERSION "0.42.0"
#       ORG_DOMAIN org.myorg
#       SOURCE_DIRS
#           src
#       LINK_QCHS
#           Qt5Core_QCH
#           Qt5Xml_QCH
#           Qt5Gui_QCH
#           Qt5Widgets_QCH
#       BLANK_MACROS
#           MyLib_EXPORT
#           MyLib_DEPRECATED
#       TAGFILE_INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/share/docs/tags
#       QCH_INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/share/docs/qch
#       COMPONENT Devel
#   )
#
# Example usage (with two QCH files, second linking first):
#
# .. code-block:: cmake
#
#   ecm_add_qch(
#       MyLib_QCH
#       NAME MyLib
#       VERSION ${MyLib_VERSION}
#       ORG_DOMAIN org.myorg
#       SOURCES ${MyLib_PUBLIC_HEADERS}
#       MD_MAINPAGE src/mylib/README.md
#       LINK_QCHS Qt5Core_QCH
#       TAGFILE_INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/share/docs/tags
#       QCH_INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/share/docs/qch
#       COMPONENT Devel
#   )
#   ecm_add_qch(
#       MyOtherLib_QCH
#       NAME MyOtherLib
#       VERSION ${MyOtherLib_VERSION}
#       ORG_DOMAIN org.myorg
#       SOURCES ${MyOtherLib_PUBLIC_HEADERS}
#       MD_MAINPAGE src/myotherlib/README.md
#       LINK_QCHS Qt5Core_QCH MyLib_QCH
#       TAGFILE_INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/share/docs/tags
#       QCH_INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/share/docs/qch
#       COMPONENT Devel
#   )
#
# ::
#
#   ecm_install_qch_export(
#       TARGETS [<name> [<name2> [...]]]
#       FILE <file>
#       DESTINATION <dest>
#       [COMPONENT <component>]
#   )
#
# This macro creates and installs a CMake file <file> which exports the given
# QCH targets <name> etc., so they can be picked up by CMake-based builds of
# other software that also generate QCH files (using ``ecm_add_qch``) and
# which should include links to the QCH files created by the given targets.
# The installed CMake file <file> is expected to be included by the CMake
# config file created for the software the related QCH files are documenting.
#
# TARGETS specifies the QCH targets which should be exported. If a target does
# not exist or does not have all needed properties, a warning will be
# generated and the target skipped.
# This behaviour might change in future versions to result in a fail instead.
#
# FILE specifies the name of the created CMake file, typically with a .cmake
# extension.
#
# DESTINATION specifies the directory on disk to which the file will be
# installed. It usually is the same as the one where the CMake config files
# for this software are installed.
#
# COMPONENT specifies the installation component name with which the
# install rule is associated.
#
# Example usage:
#
# .. code-block:: cmake
#
#   ecm_install_qch_export(
#       TARGETS MyLib_QCH
#       FILE MyLibQCHTargets.cmake
#       DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/MyLib"
#       COMPONENT Devel
#   )
#
# Since 5.36.0.

#=============================================================================
# SPDX-FileCopyrightText: 2016-2017 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

include(CMakeParseArguments)
include(ECMQueryQmake)


# Helper method: adding the LINK_QCHS property to a Qt QCH targets, from module base names ("Core" etc.)
# if target does not exist (e.g. because no tagsfile was found), this is a no-op
macro(_ecm_setup_qt_qch_links _module)
    set(_target "Qt5${_module}_QCH")
    if(TARGET ${_target})
        set(_linkqchs)
        foreach(_linkqch ${ARGN})
            list(APPEND _linkqchs "Qt5${_linkqch}_QCH")
        endforeach()
        set_property(TARGET ${_target} PROPERTY LINK_QCHS ${_linkqchs})
    endif()
endmacro()

# Helper method: ensure Qt QCH targets are created
function(_ecm_ensure_qt_qch_targets)
    # create QCH targets for Qt
    # Ideally one day Qt CMake Config files provide these
    if(NOT TARGET Qt5Core_QCH)
        # get Qt version, if any
        find_package(Qt5Core CONFIG QUIET)
        # lookup tag files
        query_qmake(qt_docs_dir QT_INSTALL_DOCS TRY)
        find_path(_qtcoreTagsPath qtcore/qtcore.tags
            PATHS
                ${qt_docs_dir}
        )

        if(Qt5Core_FOUND AND _qtcoreTagsPath)
            string(REPLACE "." "" _version ${Qt5Core_VERSION})
            # TODO: properly find each tag file
            # TODO: complete list of Qt modules
            # qtdbus.tags file missing since 5.0, QTBUG-60933, extra handling?
            foreach(_module
                3D Bluetooth Concurrent Core DBus Gui Location
                Network Positioning PrintSupport Qml Quick Sensors SerialPort Sql Svg
                WebChannel WebEngine WebSockets Widgets Xml XmlPatterns
            )
                string(TOLOWER ${_module} _lowermodule)

                set(_tagfile "${_qtcoreTagsPath}/qt${_lowermodule}/qt${_lowermodule}.tags")
                if(EXISTS "${_tagfile}")
                    add_custom_target(Qt5${_module}_QCH)
                    set_target_properties(Qt5${_module}_QCH PROPERTIES
                        DOXYGEN_TAGFILE         "${_tagfile}"
                        QHP_NAMESPACE           "org.qt-project.qt${_lowermodule}"
                        QHP_NAMESPACE_VERSIONED "org.qt-project.qt${_lowermodule}.${_version}"
                        QHP_VIRTUALFOLDER       "qt${_lowermodule}"
                        IMPORTED TRUE
                    )
                endif()
            endforeach()
            _ecm_setup_qt_qch_links(3D           Gui Core)
            _ecm_setup_qt_qch_links(Bluetooth    DBus Core)
            _ecm_setup_qt_qch_links(Concurrent   Gui Core)
            _ecm_setup_qt_qch_links(DBus         Core)
            _ecm_setup_qt_qch_links(Gui          Core)
            _ecm_setup_qt_qch_links(Location     Positioning Gui Core)
            _ecm_setup_qt_qch_links(Network      Core)
            _ecm_setup_qt_qch_links(Positioning  Core)
            _ecm_setup_qt_qch_links(PrintSupport Widgets Gui Core)
            _ecm_setup_qt_qch_links(Qml          Network Core)
            _ecm_setup_qt_qch_links(Quick        Qml Network Gui Core)
            _ecm_setup_qt_qch_links(Sensors      Core)
            _ecm_setup_qt_qch_links(SerialPort   Core)
            _ecm_setup_qt_qch_links(Sql          Core)
            _ecm_setup_qt_qch_links(Svg          Widgets Gui Core)
            _ecm_setup_qt_qch_links(WebChannel   Qml Core)
            _ecm_setup_qt_qch_links(WebEngine    Quick Qml Gui Core)
            _ecm_setup_qt_qch_links(WebSockets   Network Core)
            _ecm_setup_qt_qch_links(Widgets      Gui Core)
            _ecm_setup_qt_qch_links(Xml          Core)
            _ecm_setup_qt_qch_links(XmlPatterns  Network Core)
        endif()
    endif()
endfunction()

# Helper method: collect all qch targets from the LINK_QCHS dependency tree and set result to <var>
function(_ecm_collect_linkable_qch_targets name)
    set(_candidate_qchs ${ARGN})
    set(_handled_qchs)
    set(_good_qchs)
    # while unhandled qch targets
    while(_candidate_qchs)
        # get another unhandled qch target
        list(GET _candidate_qchs 0 _qch)
        list(REMOVE_AT _candidate_qchs 0)
        list(FIND _handled_qchs ${_qch} _index)
        # if not already handled
        if(_index EQUAL -1)
            list(APPEND _handled_qchs ${_qch})
            if(TARGET ${_qch})
                # always look at other linked qch targets, also for incomplete targets
                get_property(_link_qchs TARGET ${_qch} PROPERTY LINK_QCHS)
                if(_link_qchs)
                    list(APPEND _candidate_qchs ${_link_qchs})
                endif()
                # check if this target has all needed properties
                set(_target_usable TRUE)
                foreach(_propertyname
                    DOXYGEN_TAGFILE
                    QHP_NAMESPACE
                    QHP_VIRTUALFOLDER
                )
                    get_target_property(_property ${_qch} ${_propertyname})
                    if(NOT _property)
                        message(STATUS "No property ${_propertyname} set on ${_qch} when calling ecm_add_qch(). <<${_property}>>")
                        set(_target_usable FALSE)
                    endif()
                endforeach()
                get_target_property(_tagfile_build ${_qch} DOXYGEN_TAGFILE_BUILD)
                if (NOT _tagfile_build)
                    get_target_property(_tagfile ${_qch} DOXYGEN_TAGFILE)
                    if(NOT EXISTS ${_tagfile})
                        message(STATUS "No such tag file \"${_tagfile}\" found for ${_qch} when calling ecm_add_qch().")
                        set(_target_usable FALSE)
                    endif()
                endif()
                if(_target_usable)
                    list(APPEND _good_qchs ${_qch})
                else()
                    message(WARNING "No linking to API dox of ${_qch}.")
                endif()
            else()
                message(STATUS "No such target ${_qch} defined when calling ecm_add_qch(), ignored.")
            endif()
        endif()
    endwhile()
    set(${name} ${_good_qchs} PARENT_SCOPE)
endfunction()


function(ecm_add_qch target_name)
    # Parse arguments
    set(options VERBOSE)
    set(oneValueArgs NAME BASE_NAME QCH_INSTALL_DESTINATION TAGFILE_INSTALL_DESTINATION COMPONENT VERSION NAMESPACE MD_MAINPAGE ORG_DOMAIN CONFIG_TEMPLATE)
    set(multiValueArgs SOURCE_DIRS SOURCES INCLUDE_DIRS IMAGE_DIRS EXAMPLE_DIRS PREDEFINED_MACROS BLANK_MACROS LINK_QCHS)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # check required args
    foreach(_arg_name NAME QCH_INSTALL_DESTINATION TAGFILE_INSTALL_DESTINATION VERSION)
        if(NOT DEFINED ARGS_${_arg_name})
            message(FATAL_ERROR "${_arg_name} needs to be defined when calling ecm_add_qch")
        endif()
    endforeach()
    if(NOT DEFINED ARGS_SOURCE_DIRS AND NOT DEFINED ARGS_SOURCES AND NOT DEFINED ARGS_CONFIG_TEMPLATE)
        message(FATAL_ERROR "SOURCE_DIRS or SOURCES needs to be defined when calling ecm_add_qch")
    endif()
    if(DEFINED ARGS_SOURCE_DIRS AND DEFINED ARGS_SOURCES)
        message(FATAL_ERROR "Either SOURCE_DIRS or SOURCES, not both, needs to be defined when calling ecm_add_qch")
    endif()
    if(NOT DEFINED ARGS_ORG_DOMAIN AND NOT DEFINED ARGS_NAMESPACE)
        message(FATAL_ERROR "ORG_DOMAIN or NAMESPACE needs to be defined when calling ecm_add_qch")
    endif()

    # find required tools
    if (NOT DOXYGEN_PATCHED_JSFILESADDED)
        set(REQUIRED_DOXYGEN_VERSION 1.8.13)
    endif()
    find_package(Doxygen ${REQUIRED_DOXYGEN_VERSION} REQUIRED)
    if (NOT DOXYGEN_FOUND AND NOT DOXYGEN_PATCHED_JSFILESADDED)
        set(doxygen_description_addition " (Or older version patched with https://github.com/doxygen/doxygen/commit/bf9415698e53d79b, pass -DDOXYGEN_PATCHED_JSFILESADDED=ON to cmake if patched)")
    endif()
    set_package_properties(Doxygen PROPERTIES
        TYPE REQUIRED
        PURPOSE "Needed for API dox QCH file generation${doxygen_description_addition}"
    )
    find_package(QHelpGenerator REQUIRED)
    set_package_properties(QHelpGenerator PROPERTIES
        TYPE REQUIRED
        PURPOSE "Needed for API dox QCH file generation"
        DESCRIPTION "Part of Qt5 tools"
    )
    set(_missing_tools)
    if (NOT DOXYGEN_FOUND)
        list(APPEND _missing_tools "Doxygen")
    endif()
    if (NOT QHelpGenerator_FOUND)
        list(APPEND _missing_tools "qhelpgenerator")
    endif()

    if (_missing_tools)
        message(WARNING "API dox QCH file will not be generated, tools missing: ${_missing_tools}!")
    else()
        _ecm_ensure_qt_qch_targets()

        # prepare base dirs, working file names and other vars
        if (DEFINED ARGS_BASE_NAME)
            set(_basename ${ARGS_BASE_NAME})
        else()
            set(_basename ${ARGS_NAME})
        endif()
        set(_qch_file_basename "${_basename}.qch")
        set(_tags_file_basename "${_basename}.tags")
        set(_qch_buildpath "${CMAKE_CURRENT_BINARY_DIR}/${_qch_file_basename}")
        set(_tags_buildpath "${CMAKE_CURRENT_BINARY_DIR}/${_tags_file_basename}")
        set(_apidox_builddir "${CMAKE_CURRENT_BINARY_DIR}/${_basename}_ECMQchDoxygen")
        if (DEFINED ARGS_NAMESPACE)
            set(_namespace "${ARGS_NAMESPACE}")
        else()
            set(_namespace "${ARGS_ORG_DOMAIN}.${ARGS_NAME}")
        endif()
        string(REPLACE "." "_" _dotLessVersion ${ARGS_VERSION})
        set(_versioned_namespace "${_namespace}.${_dotLessVersion}")
        set(_sources)
        set(_dep_tagfiles)
        set(_dep_qch_targets)

        ### Create doxygen config file
        set(_doxygenconfig_file "${CMAKE_CURRENT_BINARY_DIR}/${_basename}_ECMQchDoxygen.config")
        if (DEFINED ARGS_CONFIG_TEMPLATE)
            set(_doxygenconfig_template_file "${ARGS_CONFIG_TEMPLATE}")
        else()
            set(_doxygenconfig_template_file "${ECM_MODULE_DIR}/ECMQchDoxygen.config.in")
        endif()
        set(_doxygen_layout_file "${ECM_MODULE_DIR}/ECMQchDoxygenLayout.xml")
        # Setup variables used in config file template, ECM_QCH_DOXYGEN_*
        set(ECM_QCH_DOXYGEN_OUTPUTDIR "\"${_apidox_builddir}\"")
        set(ECM_QCH_DOXYGEN_TAGFILE "\"${_tags_buildpath}\"")
        set(ECM_QCH_DOXYGEN_LAYOUTFILE "\"${_doxygen_layout_file}\"")
        set(ECM_QCH_DOXYGEN_INCLUDE_PATH)
        foreach(_include_DIR IN LISTS ARGS_INCLUDE_DIRS)
            if (NOT IS_ABSOLUTE ${_include_DIR})
                set(_include_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${_include_DIR}")
            endif()
            # concat dirs separated by a break, it is no issue that first has also a leading break
            set(ECM_QCH_DOXYGEN_INCLUDE_PATH "${ECM_QCH_DOXYGEN_INCLUDE_PATH} \\\n\"${_include_DIR}\"")
        endforeach()
        set(ECM_QCH_DOXYGEN_IMAGEDIRS)
        foreach(_image_DIR IN LISTS ARGS_IMAGE_DIRS)
            if (NOT IS_ABSOLUTE ${_image_DIR})
                set(_image_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${_image_DIR}")
            endif()
            # concat dirs separated by a break, it is no issue that first has also a leading break
            set(ECM_QCH_DOXYGEN_IMAGEDIRS "${ECM_QCH_DOXYGEN_IMAGEDIRS} \\\n\"${_image_DIR}\"")
        endforeach()
        set(ECM_QCH_DOXYGEN_EXAMPLEDIRS)
        foreach(_example_DIR IN LISTS ARGS_EXAMPLE_DIRS)
            if (NOT IS_ABSOLUTE ${_example_DIR})
                set(_example_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${_example_DIR}")
            endif()
            # concat dirs separated by a break, it is no issue that first has also a leading break
            set(ECM_QCH_DOXYGEN_EXAMPLEDIRS "${ECM_QCH_DOXYGEN_EXAMPLEDIRS} \\\n\"${_example_DIR}\"")
        endforeach()
        if (ARGS_MD_MAINPAGE)
            if (NOT IS_ABSOLUTE ${ARGS_MD_MAINPAGE})
                set(ARGS_MD_MAINPAGE "${CMAKE_CURRENT_SOURCE_DIR}/${ARGS_MD_MAINPAGE}")
            endif()
            set(ECM_QCH_DOXYGEN_MAINPAGE_MDFILE "\"${ARGS_MD_MAINPAGE}\"")
        else()
            set(ECM_QCH_DOXYGEN_MAINPAGE_MDFILE)
        endif()
        set(ECM_QCH_DOXYGEN_INPUT)
        if (ARGS_SOURCE_DIRS)
            foreach(_source_DIR IN LISTS ARGS_SOURCE_DIRS)
                if (NOT IS_ABSOLUTE ${_source_DIR})
                    set(_source_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${_source_DIR}")
                endif()
                # concat dirs separated by a break, it is no issue that first has also a leading break
                set(ECM_QCH_DOXYGEN_INPUT "${ECM_QCH_DOXYGEN_INPUT} \\\n\"${_source_DIR}\"")
            endforeach()
            if (ARGS_MD_MAINPAGE)
                set(ECM_QCH_DOXYGEN_INPUT "${ECM_QCH_DOXYGEN_INPUT} \\\n\"${ARGS_MD_MAINPAGE}\"")
            endif()
            set(ECM_QCH_DOXYGEN_FILE_PATTERNS "*.h *.cpp *.hpp *.hh *.cc *.h++ *.c++ *.hxx *.cxx *.dox *.md")
        else()
            foreach(_source IN LISTS ARGS_SOURCES)
                if (NOT IS_ABSOLUTE ${_source})
                    set(_source "${CMAKE_CURRENT_SOURCE_DIR}/${_source}")
                endif()
                list(APPEND _sources "${_source}")
            endforeach()
            if (ARGS_MD_MAINPAGE)
                list(FIND _sources ${ARGS_MD_MAINPAGE} _mainpage_index)
                if (_mainpage_index STREQUAL -1)
                    list(APPEND _sources "${ARGS_MD_MAINPAGE}")
                endif()
            endif()
            foreach(_source IN LISTS _sources)
                # concat sources separated by a break, it is no issue that first has also a leading break
                set(ECM_QCH_DOXYGEN_INPUT "${ECM_QCH_DOXYGEN_INPUT} \\\n\"${_source}\"")
            endforeach()
            set(ECM_QCH_DOXYGEN_FILE_PATTERNS "")
        endif()

        set(ECM_QCH_DOXYGEN_PROJECTNAME ${ARGS_NAME})
        file(RELATIVE_PATH _builddirrelative_filepath "${_apidox_builddir}/html"  ${_qch_buildpath})
        set(ECM_QCH_DOXYGEN_FILEPATH "\"${_builddirrelative_filepath}\"")
        set(ECM_QCH_DOXYGEN_PROJECTVERSION ${ARGS_VERSION})
        string(TOLOWER ${ARGS_NAME} ECM_QCH_DOXYGEN_VIRTUALFOLDER)
        set(ECM_QCH_DOXYGEN_FULLNAMESPACE ${_versioned_namespace})
        set(ECM_QCH_DOXYGEN_PREDEFINED_MACROS)
        foreach(_macro IN LISTS ARGS_PREDEFINED_MACROS)
            # concat dirs separated by a break, it is no issue that first has also a leading break
            # wrap each macro in quotes, to handle potential blanks and commas
            string(REPLACE "\"" "\\\"" _macro "${_macro}")
            set(ECM_QCH_DOXYGEN_PREDEFINED_MACROS "${ECM_QCH_DOXYGEN_PREDEFINED_MACROS} \\\n\"${_macro}\"")
        endforeach()
        set(ECM_QCH_DOXYGEN_BLANK_MACROS)
        foreach(_macro IN LISTS ARGS_BLANK_MACROS)
            # concat dirs separated by a break, it is no issue that first has also a leading break
            # wrap each macro in quotes, to handle potential blanks and commas
            string(REPLACE "\"" "\\\"" _macro "${_macro}")
            set(ECM_QCH_DOXYGEN_BLANK_MACROS "${ECM_QCH_DOXYGEN_BLANK_MACROS} \\\n\"${_macro}=\"")
        endforeach()

        # create list of tag files for linking other QCH files
        set(ECM_QCH_DOXYGEN_TAGFILES)
        _ecm_collect_linkable_qch_targets(_link_qchs ${ARGS_LINK_QCHS})
        foreach(_link_qch IN LISTS _link_qchs)
            list(APPEND _dep_qch_targets ${_link_qch})
            get_target_property(_link_qch_tagfile ${_link_qch} DOXYGEN_TAGFILE)
            get_target_property(_link_qch_tagfile_build ${_link_qch} DOXYGEN_TAGFILE_BUILD)
            get_target_property(_link_qch_namespace ${_link_qch} QHP_NAMESPACE)
            get_target_property(_link_qch_virtualfolder ${_link_qch} QHP_VIRTUALFOLDER)
            # if same build, then prefer build version over any installed one
            if (_link_qch_tagfile_build)
                set(_link_qch_tagfile ${_link_qch_tagfile_build})
                list(APPEND _dep_tagfiles "${_link_qch_tagfile}")
            endif()
            get_property(_linkqchs TARGET ${_link_qch} PROPERTY LINK_QCHS)
            set(_tagfile_entry "\"${_link_qch_tagfile}=qthelp://${_link_qch_namespace}/${_link_qch_virtualfolder}/\"")
            # concat dirs separated by a break, it is no issue that first has also a leading break
            set(ECM_QCH_DOXYGEN_TAGFILES "${ECM_QCH_DOXYGEN_TAGFILES} \\\n${_tagfile_entry}")
        endforeach()

        set(ECM_QCH_DOXYGEN_WARN_LOGFILE "\"${_doxygenconfig_file}.log\"")
        if(ARGS_VERBOSE)
            set(ECM_QCH_DOXYGEN_QUIET "NO")
        else()
            set(ECM_QCH_DOXYGEN_QUIET "YES")
        endif()
        set(ECM_QCH_DOXYGEN_QHELPGENERATOR_EXECUTABLE ${QHelpGenerator_EXECUTABLE})

        # finally create doxygen config file
        configure_file(
            "${_doxygenconfig_template_file}"
            "${_doxygenconfig_file}"
            @ONLY
        )

        # setup make target
        set(_qch_INSTALLPATH ${ARGS_QCH_INSTALL_DESTINATION})
        set(_tags_INSTALLPATH ${ARGS_TAGFILE_INSTALL_DESTINATION})
        file(RELATIVE_PATH _relative_qch_file ${CMAKE_BINARY_DIR}  ${_qch_buildpath})
        file(RELATIVE_PATH _relative_tags_file ${CMAKE_BINARY_DIR}  ${_tags_buildpath})
        add_custom_command(
            OUTPUT ${_qch_buildpath} ${_tags_buildpath}
            COMMENT "Generating ${_relative_qch_file}, ${_relative_tags_file}"
            COMMAND cmake -E remove_directory "${ECM_QCH_DOXYGEN_OUTPUTDIR}"
            COMMAND cmake -E make_directory "${ECM_QCH_DOXYGEN_OUTPUTDIR}"
            COMMAND ${DOXYGEN_EXECUTABLE} "${_doxygenconfig_file}"
            DEPENDS
                ${_doxygenconfig_file}
                ${_doxygen_layout_file}
                ${_sources}
                ${_dep_tagfiles}
                ${_dep_qch_targets}
        )
        add_custom_target(${target_name} ALL DEPENDS ${_qch_buildpath} ${_tags_buildpath})
        set_target_properties(${target_name} PROPERTIES
            DOXYGEN_TAGFILE "${_qch_INSTALLPATH}/${_tags_file_basename}"
            DOXYGEN_TAGFILE_NAME "${_tags_file_basename}"
            DOXYGEN_TAGFILE_INSTALLDIR "${_qch_INSTALLPATH}"
            DOXYGEN_TAGFILE_BUILD "${_tags_buildpath}"
            QHP_NAMESPACE "${_namespace}"
            QHP_NAMESPACE_VERSIONED  "${_versioned_namespace}"
            QHP_VIRTUALFOLDER "${ECM_QCH_DOXYGEN_VIRTUALFOLDER}"
        )
        # list as value does not work with set_target_properties
        set_property(TARGET ${target_name} PROPERTY LINK_QCHS "${ARGS_LINK_QCHS}")

        if (DEFINED ARGS_COMPONENT)
            set(_component COMPONENT ${ARGS_COMPONENT})
        else()
            set(_component)
        endif()

        # setup installation
        install(FILES
            ${_qch_buildpath}
            DESTINATION ${_qch_INSTALLPATH}
            ${_component}
        )

        install(FILES
            ${_tags_buildpath}
            DESTINATION ${_tags_INSTALLPATH}
            ${_component}
        )
    endif()

endfunction()


function(ecm_install_qch_export)
    set(options )
    set(oneValueArgs FILE DESTINATION COMPONENT)
    set(multiValueArgs TARGETS)

    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT DEFINED ARGS_FILE)
        message(FATAL_ERROR "FILE needs to be defined when calling ecm_install_qch_export().")
    endif()

    if(NOT DEFINED ARGS_DESTINATION)
        message(FATAL_ERROR "DESTINATION needs to be defined when calling ecm_install_qch_export().")
    endif()

    # TARGETS may be empty (and ARGS_TARGETS will not be defined then by cmake_parse_arguments)

    set(_content
"# This file was generated by ecm_install_qch_export(). DO NOT EDIT!
"
    )

    foreach(_target IN LISTS ARGS_TARGETS)
        set(_target_usable TRUE)

        if (NOT TARGET ${_target})
            message(STATUS "No such target ${_target} when calling ecm_install_qch_export().")
            set(_target_usable FALSE)
        else()
            foreach(_propertyname
                DOXYGEN_TAGFILE_NAME
                DOXYGEN_TAGFILE_INSTALLDIR
                QHP_NAMESPACE
                QHP_NAMESPACE_VERSIONED
                QHP_VIRTUALFOLDER
            )
                get_target_property(_property ${_target} ${_propertyname})
                if(NOT _property)
                    message(STATUS "No property ${_propertyname} set on ${_target} when calling ecm_install_qch_export(). <${_property}>")
                    set(_target_usable FALSE)
                endif()
            endforeach()
        endif()
        if(_target_usable)
            get_target_property(_tagfile_name ${_target} DOXYGEN_TAGFILE_NAME)
            get_target_property(_tagfile_installdir ${_target} DOXYGEN_TAGFILE_INSTALLDIR)
            if (NOT IS_ABSOLUTE ${_tagfile_installdir})
                set(_tagfile_installdir "${CMAKE_INSTALL_PREFIX}/${_tagfile_installdir}")
            endif()
            get_target_property(_namespace ${_target} QHP_NAMESPACE)
            get_target_property(_namespace_versioned ${_target} QHP_NAMESPACE_VERSIONED)
            get_target_property(_virtualfolder ${_target} QHP_VIRTUALFOLDER)
            get_property(_linkqchs TARGET ${_target} PROPERTY LINK_QCHS)
            set(_content "${_content}
if (NOT TARGET ${_target})

add_custom_target(${_target})
set_target_properties(${_target} PROPERTIES
    DOXYGEN_TAGFILE \"${_tagfile_installdir}/${_tagfile_name}\"
    QHP_NAMESPACE \"${_namespace}\"
    QHP_NAMESPACE_VERSIONED \"${_namespace_versioned}\"
    QHP_VIRTUALFOLDER \"${_virtualfolder}\"
    IMPORTED TRUE
)
set_property(TARGET ${_target} PROPERTY LINK_QCHS ${_linkqchs})

endif()
"
            )
        else()
            message(STATUS "No target exported for ${_target}.")
        endif()
    endforeach()

    if (NOT IS_ABSOLUTE ${ARGS_FILE})
        set(ARGS_FILE "${CMAKE_CURRENT_BINARY_DIR}/${ARGS_FILE}")
    endif()

    file(GENERATE
        OUTPUT "${ARGS_FILE}"
        CONTENT "${_content}"
    )

    if (DEFINED ARGS_COMPONENT)
        set(_component COMPONENT ${ARGS_COMPONENT})
    else()
        set(_component)
    endif()
    install(
        FILES "${ARGS_FILE}"
        DESTINATION "${ARGS_DESTINATION}"
        ${_component}
    )

endfunction()
