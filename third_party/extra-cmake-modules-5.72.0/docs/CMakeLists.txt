#=============================================================================
# SPDX-FileCopyrightText: 2000-2013 Kitware, Inc.
# SPDX-FileCopyrightText: 2014-2015 Alex Merry <alex.merry@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

include(CMakeDependentOption)

find_package(Sphinx 1.2 MODULE)
set_package_properties(
    Sphinx
    PROPERTIES
        URL "https://www.sphinx-doc.org/"
        DESCRIPTION "Tool to generate documentation."
        TYPE OPTIONAL
        PURPOSE "Required to build documentation for Extra CMake Modules."
)

find_package(QCollectionGenerator MODULE)
set_package_properties(
    QCollectionGenerator
    PROPERTIES
        URL "https://www.qt.io/"
        DESCRIPTION "Qt help collection generator."
        TYPE OPTIONAL
        PURPOSE "Required to build Extra CMake Modules documentation in Qt Help format."
)

cmake_dependent_option(
    BUILD_HTML_DOCS "Build html help with Sphinx" ON
    "Sphinx_FOUND" OFF
)
add_feature_info(BUILD_HTML_DOCS BUILD_HTML_DOCS "Generate HTML documentation for installed modules.")

cmake_dependent_option(
    BUILD_MAN_DOCS "Build man pages with Sphinx" ON
    "Sphinx_FOUND" OFF
)
add_feature_info(BUILD_MAN_DOCS BUILD_MAN_DOCS "Generate man page documentation for installed modules.")

cmake_dependent_option(
    BUILD_QTHELP_DOCS "Build Qt help with Sphinx" OFF
    "Sphinx_FOUND;QCollectionGenerator_FOUND" OFF
)
add_feature_info(BUILD_QTHELP_DOCS BUILD_QTHELP_DOCS "Generate QtHelp documentation for installed modules.")


set(doc_formats "")
if(BUILD_HTML_DOCS)
    list(APPEND doc_formats html)
endif()
if(BUILD_MAN_DOCS)
    list(APPEND doc_formats man)
endif()
if(BUILD_QTHELP_DOCS)
    list(APPEND doc_formats qthelp)
    set(qthelp_extra_commands
        COMMAND
            QCollectionGenerator::Generator
            ${CMAKE_CURRENT_BINARY_DIR}/qthelp/ExtraCMakeModules.qhcp
    )
endif()

if(NOT doc_formats)
    return()
endif()

if (Sphinx_VERSION VERSION_LESS 1.3)
    set(sphinx_theme default)
else()
    set(sphinx_theme classic)
endif()
configure_file(sphinx/conf.py.in conf.py @ONLY)
configure_file(sphinx/ecm.css.in static/ecm.css)


set(doc_format_outputs "")
set(doc_format_last "")
foreach(format ${doc_formats})
    set(doc_format_output "doc_format_${format}")
    set(doc_format_log "build-${format}.log")
    add_custom_command(
        OUTPUT ${doc_format_output}
        COMMAND
            Sphinx::Build
            -c ${CMAKE_CURRENT_BINARY_DIR}
            -d ${CMAKE_CURRENT_BINARY_DIR}/doctrees
            -b ${format}
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/${format}
            > ${doc_format_log} # log stdout, pass stderr
        ${${format}_extra_commands}
        DEPENDS ${doc_format_last}
        COMMENT "sphinx-build ${format}: see ${CMAKE_CURRENT_BINARY_DIR}/${doc_format_log}"
        VERBATIM
    )
    set_property(SOURCE ${doc_format_output} PROPERTY SYMBOLIC 1)
    list(APPEND doc_format_outputs ${doc_format_output})
    set(doc_format_last ${doc_format_output})
endforeach()

add_custom_target(documentation ALL DEPENDS ${doc_format_outputs})

if(BUILD_MAN_DOCS)
    file(GLOB man_rst RELATIVE ${ECM_SOURCE_DIR}/docs/manual
        ${ECM_SOURCE_DIR}/docs/manual/*.[1-9].rst)
    foreach(m ${man_rst})
        if("x${m}" MATCHES "^x(.+)\\.([1-9])\\.rst$")
            set(name "${CMAKE_MATCH_1}")
            set(sec "${CMAKE_MATCH_2}")
            install(
                FILES ${CMAKE_CURRENT_BINARY_DIR}/man/${name}.${sec}
                DESTINATION ${MAN_INSTALL_DIR}/man${sec}
            )
        endif()
    endforeach()
endif()
if(BUILD_HTML_DOCS)
    install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
        DESTINATION ${DOC_INSTALL_DIR}
        PATTERN .buildinfo EXCLUDE
        PATTERN objects.inv EXCLUDE
    )
endif()
if(BUILD_QTHELP_DOCS)
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/qthelp/ExtraCMakeModules.qch
        DESTINATION ${DOC_INSTALL_DIR}
    )
endif()
