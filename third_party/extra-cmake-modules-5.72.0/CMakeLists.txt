#=============================================================================
# SPDX-FileCopyrightText: 2006-2013 Alex Neundorf <neundorf@kde.org>
# SPDX-FileCopyrightText: 2014-2015 Alex Merry <alex.merry@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

#
# Preliminary setup
#

set(VERSION "5.72.0") # handled by release scripts

if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
    project(ECM
        VERSION "${VERSION}"
        LANGUAGES NONE
    )
else()
    project(ECM NONE)
    include("${CMAKE_SOURCE_DIR}/modules/ECMSetupVersion.cmake")
    ecm_setup_version(
        "${VERSION}"
        VARIABLE_PREFIX ECM
    )
endif()


#
# Includes
#

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(FeatureSummary)


#
# Installation paths
#

set(SHARE_INSTALL_DIR share
    CACHE
    PATH
    "read-only architecture-independent data"
)

set(CMAKECONFIG_INSTALL_DIR       ${SHARE_INSTALL_DIR}/ECM/cmake/)

set(FIND_MODULES_INSTALL_DIR      ${SHARE_INSTALL_DIR}/ECM/find-modules/)
set(KDE_MODULES_INSTALL_DIR       ${SHARE_INSTALL_DIR}/ECM/kde-modules/)
set(MODULES_INSTALL_DIR           ${SHARE_INSTALL_DIR}/ECM/modules/)
set(TOOLCHAIN_MODULES_INSTALL_DIR ${SHARE_INSTALL_DIR}/ECM/toolchain/)

set(DOC_INSTALL_DIR ${SHARE_INSTALL_DIR}/doc/ECM
    CACHE
    PATH
    "HTML and QtHelp documentation"
)
set(MAN_INSTALL_DIR ${SHARE_INSTALL_DIR}/man
    CACHE
    PATH
    "man pages"
)


#
# Documentation
#

add_subdirectory(docs)


#
# Tests
#

option(BUILD_TESTING "Build automated tests." ON)
add_feature_info(BUILD_TESTING BUILD_TESTING "Build automated tests.")
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()


#
# Install modules
#

file(GLOB installModuleFiles ${CMAKE_SOURCE_DIR}/modules/*[^~])
install(FILES ${installModuleFiles} DESTINATION ${MODULES_INSTALL_DIR})

file(GLOB installTestModuleFiles ${CMAKE_SOURCE_DIR}/test-modules/*[^~])
install(FILES ${installTestModuleFiles} DESTINATION ${SHARE_INSTALL_DIR}/ECM/test-modules)

file(GLOB installKdeModuleFiles ${CMAKE_SOURCE_DIR}/kde-modules/*[^~])
install(FILES ${installKdeModuleFiles} DESTINATION ${KDE_MODULES_INSTALL_DIR})

file(GLOB installFindModuleFiles ${CMAKE_SOURCE_DIR}/find-modules/*.cmake ${CMAKE_SOURCE_DIR}/find-modules/*.py)
install(FILES ${installFindModuleFiles} DESTINATION ${FIND_MODULES_INSTALL_DIR})

file(GLOB installToolchainModuleFiles ${CMAKE_SOURCE_DIR}/toolchain/*[^~])
install(FILES ${installToolchainModuleFiles} DESTINATION ${TOOLCHAIN_MODULES_INSTALL_DIR})


#
# Config.cmake file
#

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ECMConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ECMConfig.cmake
    INSTALL_DESTINATION
        ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS
        FIND_MODULES_INSTALL_DIR
        KDE_MODULES_INSTALL_DIR
        MODULES_INSTALL_DIR
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ECMConfigVersion.cmake
    VERSION
        "${PROJECT_VERSION}"
    COMPATIBILITY
        AnyNewerVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ECMConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ECMConfigVersion.cmake
    DESTINATION
        ${CMAKECONFIG_INSTALL_DIR}
)


#
# Packaging
#

set(CPACK_PACKAGE_NAME extra-cmake-modules)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_SYSTEM_NAME "generic")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SET_DESTDIR FALSE)

include(CPack)


#
# Feature summary
#

feature_summary(
    FATAL_ON_MISSING_REQUIRED_PACKAGES
    WHAT ALL
)
