#.rst:
# ECMQMLModules
# -------------
#
# Find QML import modules through a find_qmlmodule() call.
# It uses the qmlplugindump application to find the plugins and sets them up as
# runtime dependencies.
#
# This is useful so that when we configure a project we are noified when some
# QML imports are not present in the system, thus having the application compilable
# but fail at runtime.
#
# ::
#
#   ecm_find_qmlmodule(<module_name> <version>...)
#
# Any further arguments passed will be forwarded into a find_package() call. See
# find_package() documentation for more information.
#
# Usage example:
#
# .. code-block:: cmake
#
#   ecm_find_qmlmodule(org.kde.kirigami 2.1)
#
# Since 5.38.0.
#
#=============================================================================
# SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

set(MODULES_DIR ${CMAKE_CURRENT_LIST_DIR})

function(ecm_find_qmlmodule MODULE_NAME VERSION)
    set(GENMODULE "${MODULE_NAME}-QMLModule")

    configure_file("${MODULES_DIR}/ECMFindQMLModule.cmake.in" "Find${GENMODULE}.cmake" @ONLY)

    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}" ${CMAKE_MODULE_PATH})
    find_package(${GENMODULE} ${ARGN})

    if(COMMAND set_package_properties)
        set_package_properties(${GENMODULE} PROPERTIES
            DESCRIPTION "QML module '${MODULE_NAME}' is a runtime dependency."
            TYPE RUNTIME)
    endif()
endfunction()
