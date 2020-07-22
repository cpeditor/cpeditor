#.rst:
# ECMGenerateQmlTypes
# -------------------
#
# Generates plugins.qmltypes files for QML plugins.
#
# ::
#
#   ecm_generate_qmltypes(<org.kde.pluginname> 1.3
#                         DESTINATION <${KDE_INSTALL_QMLDIR}/org/kde/pluginname>)
#
# Makes it possible to generate plugins.qmltypes files for the QML plugins that
# our project offers. These files offer introspection upon our plugin and are
# useful for integrating with IDE language support of our plugin. It offers
# information about the objects its methods and their argument types.
#
# The developer will be in charge of making sure that these files are up to date.
# The plugin.qmltypes file will sit in the source directory. This function will
# include the code that installs the file in the right place and a small unit
# test named qmltypes-pluginname-version that makes sure that it doesn't need updating.
#
#
# Since 5.33.0

#=============================================================================
# SPDX-FileCopyrightText: 2017 Aleix Pol Gonzalez <aleixpol@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

include(CMakeParseArguments)
function(ecm_generate_qmltypes)
    if (NOT TARGET qmltypes)
        add_custom_target(qmltypes)
    endif ()

    set(options)
    set(oneValueArgs DESTINATION TEST_ENABLED)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

    set(targetname "qmltypes-${ARG_UNPARSED_ARGUMENTS}")
    string(REPLACE ";" - targetname "${targetname}")

    set(generatedFile ${CMAKE_CURRENT_SOURCE_DIR}/plugins.qmltypes)
    add_custom_target(${targetname}
        BYPRODUCTS ${generatedFile}
        COMMAND qmlplugindump -nonrelocatable ${ARG_UNPARSED_ARGUMENTS} ${KDE_INSTALL_QMLDIR} > ${generatedFile}
    )
    add_dependencies(qmltypes ${targetname})
    if (EXISTS ${generatedFile})
        install(FILES ${generatedFile} DESTINATION "${ARG_DESTINATION}")
    endif()

    string(REPLACE ";" / processedArgs "${ARG_UNPARSED_ARGUMENTS}")

    # sometimes qmlplugindump output isn't reproducible, we better have it opt in for now
    if(ARG_TEST_ENABLED)
        add_test(NAME ${targetname} COMMAND
            cmake -DARG_UNPARSED_ARGUMENTS=${processedArgs} -DKDE_INSTALL_QMLDIR=${KDE_INSTALL_QMLDIR} -DINPUT=${generatedFile} -P ${ECM_MODULE_DIR}/../test-modules/test_execute_and_compare.cmake
        )
    endif()
endfunction()
