#.rst:
# ECMQtDeclareLoggingCategory
# ---------------------------
#
# This module provides the ``ecm_qt_declare_logging_category`` function for
# generating declarations for logging categories in Qt5, and the
# ``ecm_qt_install_logging_categories`` function for generating and installing
# a file in KDebugSettings format with the info about all those categories,
# as well as a file with info about any renamed categories if defined.
# To include in that file any logging categories that are manually defined
# also a function ``ecm_qt_export_logging_category`` is provided.
#
# ::
#
#   ecm_qt_declare_logging_category(<sources_var>
#       HEADER <filename>
#       IDENTIFIER <identifier>
#       CATEGORY_NAME <category_name>
#       [OLD_CATEGORY_NAMES <oldest_cat_name> [<second_oldest_cat_name> [...]]]
#       [DEFAULT_SEVERITY <Debug|Info|Warning|Critical|Fatal>]
#       [EXPORT <exportid>]
#       [DESCRIPTION <description>]
#   )
#
# A header file, ``<filename>``, will be generated along with a corresponding
# source file, which will be added to ``<sources_var>``. These will provide a
# QLoggingCategory category that can be referred to from C++ code using
# ``<identifier>``, and from the logging configuration using
# ``<category_name>``.
#
# If ``<filename>`` is not absolute, it will be taken relative to the current
# binary directory.
#
# If the code is compiled against Qt 5.4 or later, by default it will only log
# output that is at least the severity specified by ``DEFAULT_SEVERITY``, or
# "Info" level if ``DEFAULT_SEVERITY`` is not given. Note that, due to a
# bug in Qt 5.5, "Info" may be treated as more severe than "Fatal".
#
# ``<identifier>`` may include namespaces (eg: ``foo::bar::IDENT``).
#
# If ``EXPORT`` is passed, the category will be registered for the group id
# ``<exportid>``. Info about the categories of that group can then be
# generated in a file and installed by that group id with the
# ``ecm_qt_install_logging_categories`` function. In that case also ``DESCRIPTION``
# will need to be passed, with ``<description>`` being a short single line text.
# And ``OLD_CATEGORY_NAMES`` can be used to inform about any renamings of the category,
# so user settings can be migrated. Since 5.68.0.
#
# Since 5.14.0.
#
# ::
#
#   ecm_qt_export_logging_category(
#       IDENTIFIER <identifier>
#       CATEGORY_NAME <category_name>
#       [OLD_CATEGORY_NAMES <oldest_category_name> [<second_oldest_category_name> [...]]]
#       EXPORT <exportid>
#       DESCRIPTION <description>
#       [DEFAULT_SEVERITY <Debug|Info|Warning|Critical|Fatal>]
#   )
#
# Registers a logging category for being included in the generated and
# installed KDebugSettings files. To be used for categories who are declared by
# manual code or other ways instead of code generated with
# ``ecm_qt_declare_logging_category``.
#
# ``<identifier>`` may include namespaces (eg: ``foo::bar::IDENT``).
#
# ``EXPORT`` specifies the group id with which the category will be registered.
# Info about the categories of that group can then be generated in a file and
# installed by that group id with the ``ecm_qt_install_logging_categories`` function.
#
# ``DESCRIPTION`` specifies a short single line text describing the category.
#
# ``OLD_CATEGORY_NAMES`` can be used to inform about any renamings of the category,
# so user settings can be migrated.
#
# Since 5.68.0.
#
# ::
#
#   ecm_qt_install_logging_categories(
#       EXPORT <exportid>
#       [FILE <filename>]
#       DESTINATION <install_path>
#       [SORT]
#       [COMPONENT <component>]
#   )
#
# Generates and installs a file in KDebugSettings format with the info about all
# the categories registered for the group ``<exportid>``, as well as a file with
# info about any renamed categories, if there are.
#
# The method call needs to be after the last ``ecm_qt_declare_logging_category``
# call which uses the same ``<exportid>``. This can be in the same directory, or
# any subdirectory or parent directory.
#
# ``EXPORT`` specifies the group id of categories whose informatipn should be
# stored in the file generated and installed.
#
# ``FILE`` specifies the name of the file generated and installed. It will default
# to lower-cased ``<exportid>.categories``.
#
# ``DESTINATION`` specifies where the generated file will be
# installed.
#
# IF ``SORT`` is set, entries will be sorted by identifiers.
#
# ``COMPONENT`` specifies the installation component name with which the install
# rules for the generated file are associated.
#
# Example usage:
#
# .. code-block:: cmake
#
#   ecm_qt_declare_logging_category(
#       MYPROJECT_SRCS
#       HEADER "myproject_debug.h"
#       IDENTIFIER "MYPROJECT_DEBUG"
#       CATEGORY_NAME "myproject"
#       OLD_CATEGORY_NAMES "myprojectlog"
#       DESCRIPTION "My project"
#       EXPORT MyProject
#   )
#
#   ecm_qt_export_logging_category(
#       IDENTIFIER "MYPROJECT_SUBMODULE_DEBUG"
#       CATEGORY_NAME "myproject.submodule"
#       DESCRIPTION "My project - submodule"
#       EXPORT MyProject
#   )
#
#   ecm_qt_install_logging_categories(
#       EXPORT MyProject
#       FILE myproject.categories
#       DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
#   )
#
# Since 5.68.0.

#=============================================================================
# SPDX-FileCopyrightText: 2015 Alex Merry <alex.merry@kde.org>
# SPDX-FileCopyrightText: 2020 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

include(CMakeParseArguments)

set(_ECM_QT_DECLARE_LOGGING_CATEGORY_TEMPLATE_CPP "${CMAKE_CURRENT_LIST_DIR}/ECMQtDeclareLoggingCategory.cpp.in")
set(_ECM_QT_DECLARE_LOGGING_CATEGORY_TEMPLATE_H   "${CMAKE_CURRENT_LIST_DIR}/ECMQtDeclareLoggingCategory.h.in")

function(ecm_qt_export_logging_category)
    set(options)
    set(oneValueArgs IDENTIFIER CATEGORY_NAME DEFAULT_SEVERITY EXPORT DESCRIPTION)
    set(multiValueArgs OLD_CATEGORY_NAMES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unexpected arguments to ecm_qt_export_logging_category: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    if(NOT ARG_IDENTIFIER)
        message(FATAL_ERROR "Missing IDENTIFIER argument for ecm_qt_export_logging_category")
    endif()
    if(NOT ARG_CATEGORY_NAME)
        message(FATAL_ERROR "Missing CATEGORY_NAME argument for ecm_qt_export_logging_category")
    endif()
    if(NOT ARG_DEFAULT_SEVERITY)
        set(ARG_DEFAULT_SEVERITY Info)
        set(is_explicite_default_severity FALSE)
    else()
        set(acceptible_severities Debug Info Warning Critical Fatal)
        list(FIND acceptible_severities "${ARG_DEFAULT_SEVERITY}" pos)
        if (pos EQUAL -1)
            message(FATAL_ERROR "Unknown DEFAULT_SEVERITY ${pos}")
        endif()
        set(is_explicite_default_severity TRUE)
    endif()
    if(NOT ARG_EXPORT)
        message(FATAL_ERROR "Missing EXPORT argument for ecm_qt_export_logging_category.")
    endif()
    if(NOT ARG_DESCRIPTION)
        message(FATAL_ERROR "Missing DESCRIPTION argument for ecm_qt_export_logging_category.")
    endif()

    # note data in global properties
    set(_propertyprefix "ECM_QT_LOGGING_CATEGORY_${ARG_EXPORT}")
    set_property(GLOBAL APPEND PROPERTY "${_propertyprefix}_CATEGORIES" ${ARG_CATEGORY_NAME})
    set_property(GLOBAL PROPERTY "${_propertyprefix}_IDENTIFIER_${ARG_CATEGORY_NAME}" "${ARG_IDENTIFIER}")
    set_property(GLOBAL PROPERTY "${_propertyprefix}_DESCRIPTION_${ARG_CATEGORY_NAME}" "${ARG_DESCRIPTION}")
    set_property(GLOBAL PROPERTY "${_propertyprefix}_OLD_NAMES_${ARG_CATEGORY_NAME}" "${ARG_OLD_CATEGORY_NAMES}")
    if (is_explicite_default_severity)
        set_property(GLOBAL PROPERTY "${_propertyprefix}_DEFAULT_SEVERITY_${ARG_CATEGORY_NAME}" "${ARG_DEFAULT_SEVERITY}")
    endif()
endfunction()


function(ecm_qt_declare_logging_category sources_var)
    set(options)
    set(oneValueArgs HEADER IDENTIFIER CATEGORY_NAME DEFAULT_SEVERITY EXPORT DESCRIPTION)
    set(multiValueArgs OLD_CATEGORY_NAMES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unexpected arguments to ecm_qt_declare_logging_category: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    if(NOT ARG_HEADER)
        message(FATAL_ERROR "Missing HEADER argument for ecm_qt_declare_logging_category")
    endif()
    if(NOT ARG_IDENTIFIER)
        message(FATAL_ERROR "Missing IDENTIFIER argument for ecm_qt_declare_logging_category")
    endif()
    if(NOT ARG_CATEGORY_NAME)
        message(FATAL_ERROR "Missing CATEGORY_NAME argument for ecm_qt_declare_logging_category")
    endif()
    if(NOT ARG_DEFAULT_SEVERITY)
        set(ARG_DEFAULT_SEVERITY Info)
        set(is_explicite_default_severity FALSE)
    else()
        set(acceptible_severities Debug Info Warning Critical Fatal)
        list(FIND acceptible_severities "${ARG_DEFAULT_SEVERITY}" pos)
        if (pos EQUAL -1)
            message(FATAL_ERROR "Unknown DEFAULT_SEVERITY ${pos}")
        endif()
        set(is_explicite_default_severity TRUE)
    endif()
    if(ARG_EXPORT AND NOT ARG_DESCRIPTION)
        message(FATAL_ERROR "Missing DESCRIPTION argument for ecm_qt_declare_logging_category.")
    endif()

    if (NOT IS_ABSOLUTE "${ARG_HEADER}")
        set(ARG_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${ARG_HEADER}")
    endif()

    string(REPLACE "::" ";" namespaces "${ARG_IDENTIFIER}")
    list(LENGTH namespaces len)
    math(EXPR last_pos "${len} - 1")
    list(GET namespaces ${last_pos} IDENTIFIER)
    list(REMOVE_AT namespaces ${last_pos})

    set(OPEN_NAMESPACES)
    set(CLOSE_NAMESPACES)
    foreach(ns ${namespaces})
        set(OPEN_NAMESPACES "${OPEN_NAMESPACES} namespace ${ns} {")
        set(CLOSE_NAMESPACES "} ${CLOSE_NAMESPACES}")
    endforeach()

    string(FIND "${ARG_HEADER}" "." pos REVERSE)
    if (pos EQUAL -1)
        set(cpp_filename "${ARG_HEADER}.cpp")
    else()
        string(SUBSTRING "${ARG_HEADER}" 0 ${pos} cpp_filename)
        set(cpp_filename "${cpp_filename}.cpp")
    endif()

    get_filename_component(HEADER_NAME "${ARG_HEADER}" NAME)

    string(REGEX REPLACE "[^a-zA-Z0-9]" "_" GUARD_NAME "${HEADER_NAME}")
    string(REPLACE "::" "_" GUARD_PREFIX "ECM_QLOGGINGCATEGORY_${ARG_IDENTIFIER}")
    string(TOUPPER "${GUARD_PREFIX}_${GUARD_NAME}" GUARD_NAME)

    if (NOT _ECM_QT_DECLARE_LOGGING_CATEGORY_TEMPLATE_CPP)
       message(FATAL_ERROR "You must include(ECMQtDeclareLoggingCategory) before using ecm_qt_declare_logging_category")
    endif()

    configure_file("${_ECM_QT_DECLARE_LOGGING_CATEGORY_TEMPLATE_CPP}" "${cpp_filename}")
    configure_file("${_ECM_QT_DECLARE_LOGGING_CATEGORY_TEMPLATE_H}" "${ARG_HEADER}")

    set(sources "${${sources_var}}")
    list(APPEND sources "${cpp_filename}")
    set(${sources_var} "${sources}" PARENT_SCOPE)

    # note data in global properties
    if (ARG_EXPORT)
        set(_default_severity)
        if (is_explicite_default_severity)
            set(_default_severity DEFAULT_SEVERITY ${ARG_DEFAULT_SEVERITY})
        endif()
        set(_old_category_name)
        if (ARG_OLD_CATEGORY_NAMES)
            set(_old_category_names OLD_CATEGORY_NAMES ${ARG_OLD_CATEGORY_NAMES})
        endif()
        ecm_qt_export_logging_category(
            IDENTIFIER ${ARG_IDENTIFIER}
            CATEGORY_NAME ${ARG_CATEGORY_NAME}
            ${_old_category_names}
            ${_default_severity}
            EXPORT ${ARG_EXPORT}
            DESCRIPTION "${ARG_DESCRIPTION}"
        )
    endif()
endfunction()


function(ecm_qt_install_logging_categories)
    set(options SORT)
    set(oneValueArgs FILE EXPORT DESTINATION COMPONENT)
    set(multiValueArgs)

    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARGS_EXPORT)
        message(FATAL_ERROR "Missing EXPORT argument for ecm_qt_install_logging_categories")
    endif()

    if(NOT ARGS_DESTINATION)
        message(FATAL_ERROR "Missing DESTINATION argument for ecm_qt_install_logging_categories")
    endif()

    if(NOT ARGS_FILE)
        string(TOLOWER "${ARGS_EXPORT}.categories" ARGS_FILE)
    endif()

    set(_propertyprefix "ECM_QT_LOGGING_CATEGORY_${ARGS_EXPORT}")
    get_property(has_category GLOBAL PROPERTY "${_propertyprefix}_CATEGORIES" SET)

    if (NOT has_category)
        message(AUTHOR_WARNING "No Qt logging categories exported for \"${ARGS_EXPORT}\", generating & installing an empty file.")
    endif()

    get_property(_categories GLOBAL PROPERTY "${_propertyprefix}_CATEGORIES")
    if (ARGS_SORT)
        list(SORT _categories)
    endif()

    set(_renamed_categories)

    # generate categories file
    if (NOT IS_ABSOLUTE "${ARGS_FILE}")
        set(ARGS_FILE "${CMAKE_CURRENT_BINARY_DIR}/${ARGS_FILE}")
    endif()

    file(WRITE ${ARGS_FILE}
"# KDebugSettings data file
# This file was generated by ecm_qt_install_logging_categories(). DO NOT EDIT!

")

    foreach(_category IN LISTS _categories)
        get_property(_description GLOBAL PROPERTY "${_propertyprefix}_DESCRIPTION_${_category}")
        get_property(_identifier GLOBAL PROPERTY "${_propertyprefix}_IDENTIFIER_${_category}")
        get_property(_default_severity GLOBAL PROPERTY "${_propertyprefix}_DEFAULT_SEVERITY_${_category}")
        if (_default_severity)
            string(TOUPPER "${_default_severity}" _default_severity)
            set(_default_severity "DEFAULT_SEVERITY [${_default_severity}] ") # final space wanted
        endif()
        get_property(_old_category_names GLOBAL PROPERTY "${_propertyprefix}_OLD_NAMES_${_category}")
        if (_old_category_names)
            list(APPEND _renamed_categories ${_category})
        endif()

        # Format:
        # logname<space>description(optional <space> DEFAULT_SEVERITY [DEFAULT_CATEGORY] as WARNING/DEBUG/INFO/CRITICAL) optional IDENTIFIER [...])
        file(APPEND ${ARGS_FILE} "${_category} ${_description} ${_default_severity}IDENTIFIER [${_identifier}]\n")
    endforeach()

    set(_renamed_cats_file)
    if (_renamed_categories)
        get_filename_component(_dir ${ARGS_FILE} DIRECTORY)
        get_filename_component(_base_name ${ARGS_FILE} NAME_WE)
        set(_renamed_cats_file "${_dir}/${_base_name}.renamecategories")
        file(WRITE ${_renamed_cats_file}
"# KDebugSettings data file
# This file was generated by ecm_qt_install_logging_categories(). DO NOT EDIT!

")

        foreach(_category IN LISTS _renamed_categories)
            get_property(_category_name_history GLOBAL PROPERTY "${_propertyprefix}_OLD_NAMES_${_category}")

            list(APPEND _category_name_history ${_category})
            list(GET _category_name_history 0 _old_category_name)
            list(REMOVE_AT _category_name_history 0)
            foreach(_category_name IN LISTS _category_name_history)
                # Format:
                # oldlogname<space>newlogname
                file(APPEND ${_renamed_cats_file} "${_old_category_name} ${_category_name}\n")
                set(_old_category_name ${_category_name})
            endforeach()
        endforeach()
    endif()

    # install files
    set(_component_install)
    if (ARGS_COMPONENT)
        set(_component_install COMPONENT ${ARGS_COMPONENT})
    endif()
    install(
        FILES ${ARGS_FILE} ${_renamed_cats_file}
        DESTINATION "${ARGS_DESTINATION}"
        ${_component_install}
    )
endfunction()
