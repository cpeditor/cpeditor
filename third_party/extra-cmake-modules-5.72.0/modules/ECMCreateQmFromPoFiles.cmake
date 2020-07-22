#.rst:
# ECMCreateQmFromPoFiles
# ----------------------
#
# .. warning:: This module is deprecated and will be removed by ECM 1.0. Use
#              ECMPoQmTools instead.
#
# Generate QTranslator (.qm) catalogs from Gettext (.po) catalogs.
#
# ::
#
#   ecm_create_qm_from_po_files(PO_FILES <file1>... <fileN>
#                               [CATALOG_NAME <catalog_name>]
#                               [INSTALL_DESTINATION <install_destination>])
#
# Creates the necessary rules to compile .po files into .qm files, and install
# them.
#
# The .qm files are installed in ``<install_destination>/<lang>/LC_MESSAGES``,
# where <install_destination> is the INSTALL_DESTINATION argument and <lang> is
# extracted from the "Language" field inside the .po file.
#
# INSTALL_DESTINATION defaults to ``${LOCALE_INSTALL_DIR}`` if defined,
# otherwise it uses ``${CMAKE_INSTALL_LOCALEDIR}`` if that is defined, otherwise
# it uses ``share/locale``.
#
# CATALOG_NAME defines the name of the installed .qm files. If set, .qm files
# will be installed as ``<catalog_name>.qm``. If not set .qm files will be named
# after the name of their source .po file.
#
# Setting the catalog name is useful when all .po files for a target are kept
# in a single source directory. For example, the "mylib" probject might keep all
# its translations in a "po" directory, like this::
#
#   po/
#       es.po
#       fr.po
#
# Without setting CATALOG_NAME, those .po will be turned into .qm and installed
# as::
#
#   share/locale/fr/LC_MESSAGES/fr.qm
#   share/locale/es/LC_MESSAGES/es.qm
#
# If CATALOG_NAME is set to "mylib", they will be installed as::
#
#   share/locale/fr/LC_MESSAGES/mylib.qm
#   share/locale/es/LC_MESSAGES/mylib.qm
#
# Which is what the loader created by ecm_create_qm_loader() expects.
#
# ecm_create_qm_from_po_files() creates a "translation" target. This target
# builds all .po files into .qm files.
#
# ::
#
#   ecm_create_qm_loader(<source_files_var> <catalog_name>)
#
# ecm_create_qm_loader() generates a C++ file which ensures translations are
# automatically loaded at startup. The path of the .cpp file is appended to
# <source_files_var>.  Typical usage is like:
#
# .. code-block:: cmake
#
#   set(mylib_SRCS foo.cpp bar.cpp)
#   ecm_create_qm_loader(mylib_SRCS mylib)
#   add_library(mylib ${mylib_SRCS})
#
# This generates a C++ file which loads "mylib.qm" at startup, assuming it has
# been installed by ecm_create_qm_from_po_files(), and compiles it into ``mylib``.
#
# Since pre-1.0.0.

#=============================================================================
# SPDX-FileCopyrightText: 2014 Aurélien Gâteau <agateau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

message(AUTHOR_WARNING "ECMCreateQmFromPoFiles is deprecated and will be removed before the release of Extra CMake Modules 1.0. Use ECMPoQmTools instead.")

# Stolen from FindGettext.cmake
function(_ECM_QM_GET_UNIQUE_TARGET_NAME _name _unique_name)
   set(propertyName "_ECM_QM_UNIQUE_COUNTER_${_name}")
   get_property(currentCounter GLOBAL PROPERTY "${propertyName}")
   if(NOT currentCounter)
      set(currentCounter 1)
   endif()
   set(${_unique_name} "${_name}_${currentCounter}" PARENT_SCOPE)
   math(EXPR currentCounter "${currentCounter} + 1")
   set_property(GLOBAL PROPERTY ${propertyName} ${currentCounter} )
endfunction()

function(_ECM_QM_EXTRACT_LANGUAGE out_language po_file)
    file(READ ${po_file} content)
    # msginit uses "Language: <lang>" but lconvert uses "X-Language: <lang>"
    string(REGEX MATCH "\"(X-)?Language: ([a-z_A-Z]+)" match "${content}")
    if (NOT match)
        message(FATAL_ERROR "_ECM_QM_EXTRACT_LANGUAGE: Could not extract language from ${po_file}")
    endif()
    set(${out_language} ${CMAKE_MATCH_2} PARENT_SCOPE)
endfunction()

function(_ECM_QM_CREATE_TARGET install_destination catalog_name)
    # Find lconvert
    if(TARGET Qt5::lconvert)
        set(lconvert_executable Qt5::lconvert)
    else()
        # Qt < 5.3.1 does not define Qt5::lconvert
        get_target_property(lrelease_location Qt5::lrelease LOCATION)
        get_filename_component(lrelease_path ${lrelease_location} PATH)
        find_program(lconvert_executable
            NAMES lconvert-qt5 lconvert
            PATHS ${lrelease_path}
            NO_DEFAULT_PATH
            )
    endif()

    if (catalog_name)
        set(install_args RENAME ${catalog_name}.qm)
    else()
        set(install_args)
    endif()

    foreach (it ${ARGN})
        get_filename_component(filename_base ${it} ABSOLUTE)
        get_filename_component(filename_base ${it} NAME_WE)

        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        set(tsfile ${CMAKE_CURRENT_BINARY_DIR}/${filename_base}.ts)
        set(qmfile ${CMAKE_CURRENT_BINARY_DIR}/${filename_base}.qm)

        _ECM_QM_EXTRACT_LANGUAGE(language ${it})

        # lconvert from .po to .ts and then run lupdate to generate the correct
        # strings. Finally run lrelease to create the .qm files.
        add_custom_command(OUTPUT ${qmfile}
            COMMAND ${lconvert_executable}
                ARGS -i ${it} -o ${tsfile} -target-language ${language}
            COMMAND Qt5::lrelease
                ARGS -removeidentical -silent ${tsfile} -qm ${qmfile}
            DEPENDS ${it}
            )
        install(
            FILES ${qmfile}
            DESTINATION ${install_destination}/${language}/LC_MESSAGES
            ${install_args}
        )
        set(qmfiles ${qmfiles} ${qmfile})
    endforeach()

    if(NOT TARGET translations)
        add_custom_target(translations ALL)
    endif()
    _ecm_qm_get_unique_target_name(translations target_name)
    add_custom_target(${target_name} DEPENDS ${qmfiles})
    add_dependencies(translations ${target_name})
endfunction()

function(ECM_CREATE_QM_LOADER out_var catalog_name)
    # catalog_name is used in ECMQmLoader.cpp.in
    configure_file(${ECM_MODULE_DIR}/ECMQmLoader.cpp.in ECMQmLoader.cpp @ONLY)
    set(${out_var} ${${out_var}} ${CMAKE_CURRENT_BINARY_DIR}/ECMQmLoader.cpp PARENT_SCOPE)
endfunction()

function(ECM_CREATE_QM_FROM_PO_FILES)
    # This gives us Qt5::lrelease and Qt5::lupdate but unfortunately no Qt5::lconvert
    # See https://bugreports.qt-project.org/browse/QTBUG-37937
    find_package(Qt5LinguistTools CONFIG REQUIRED)

    foreach (arg ${ARGN})
        if (arg STREQUAL "PO_DIR")
            _ecm_create_qm_from_po_files_legacy(${ARGN})
            return()
        endif()
    endforeach()

    set(options)
    set(oneValueArgs CATALOG_NAME INSTALL_DESTINATION)
    set(multiValueArgs PO_FILES)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(ARGS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown keywords given to ECM_CREATE_QM_FROM_PO_FILES(): \"${ARGS_UNPARSED_ARGUMENTS}\"")
    endif()

    if(NOT ARGS_PO_FILES)
        message(FATAL_ERROR "ECM_CREATE_QM_FROM_PO_FILES(): Must be called with PO_FILES argument")
    endif()

    if(NOT ARGS_INSTALL_DESTINATION)
        if (LOCALE_INSTALL_DIR)
            set(ARGS_INSTALL_DESTINATION "${LOCALE_INSTALL_DIR}")
        elseif (CMAKE_INSTALL_LOCALEDIR)
            set(ARGS_INSTALL_DESTINATION "${CMAKE_INSTALL_LOCALEDIR}")
        else()
            set(ARGS_INSTALL_DESTINATION share/locale)
        endif()
    endif()

    _ecm_qm_create_target(${ARGS_INSTALL_DESTINATION} "${ARGS_CATALOG_NAME}" ${ARGS_PO_FILES})
endfunction()

# Handles the syntax exposed in ECM 0.0.12, shipped with KDE Frameworks 5.0beta1
#
# This is a macro so that the value written in ${ARGS_CREATE_LOADER} is
# correctly propagated to ECM_CREATE_QM_FROM_PO_FILES parent scope. If it were
# not a macro, ECM_CREATE_QM_FROM_PO_FILES would have to ckeck if
# CREATE_LOADER is in the arguments and propagate the value itself.
macro(_ECM_CREATE_QM_FROM_PO_FILES_LEGACY)
    set(options)
    set(oneValueArgs PO_DIR POT_NAME DATA_INSTALL_DIR DATA_INSTALL_SUB_DIR CREATE_LOADER)
    set(multiValueArgs)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(ARGS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown keywords given to _ECM_CREATE_QM_FROM_PO_FILES_LEGACY(): \"${ARGS_UNPARSED_ARGUMENTS}\"")
    endif()

    if(NOT ARGS_POT_NAME)
        message(FATAL_ERROR "Required argument POT_NAME missing in _ECM_CREATE_QM_FROM_PO_FILES_LEGACY() call")
    endif()
    get_filename_component(catalog_name ${ARGS_POT_NAME} NAME_WE)

    if (LOCALE_INSTALL_DIR)
        set(install_destination "${LOCALE_INSTALL_DIR}")
    elseif (CMAKE_INSTALL_LOCALEDIR)
        set(install_destination "${CMAKE_INSTALL_LOCALEDIR}")
    else()
        set(install_destination share/locale)
    endif()

    file(GLOB po_files "${ARGS_PO_DIR}/*.po")
    _ecm_qm_create_target(${install_destination} "${catalog_name}" ${po_files})

    if (ARGS_CREATE_LOADER)
        ecm_create_qm_loader(loader ${catalog_name})
        set(${ARGS_CREATE_LOADER} ${loader} PARENT_SCOPE)
    endif()
endmacro()
