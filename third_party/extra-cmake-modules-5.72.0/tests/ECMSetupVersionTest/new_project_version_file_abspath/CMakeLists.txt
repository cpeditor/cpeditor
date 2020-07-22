cmake_minimum_required(VERSION 3.0.0)

project(new_project_version_file_abspath VERSION 2.3.4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
include(ECMSetupVersion)

ecm_setup_version(PROJECT
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/../NewProjectVersionAbsPath.cmake"
)

include(../version_helpers.cmake)

# NB: name comes from project() command
standard_version_var_checks(new_project_version_file_abspath 2.3.4)
AnyNewer_2_3_4_checks("${CMAKE_CURRENT_BINARY_DIR}/../NewProjectVersionAbsPath.cmake")

add_executable(dummy main.c)
