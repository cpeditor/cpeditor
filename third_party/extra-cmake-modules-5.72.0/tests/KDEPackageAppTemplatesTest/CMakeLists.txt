cmake_minimum_required(VERSION 3.0)
set(ECM_KDE_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../kde-modules)
set(ECM_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../modules)
set(CMAKE_MODULE_PATH "${ECM_KDE_MODULE_DIR}")

include(KDEPackageAppTemplates)

message(STATUS "Test: generate compressed template")
kde_package_app_templates(TEMPLATES "qml-plasmoid" INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})

# this will be run by CTest
configure_file(check.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/check.cmake" @ONLY)
