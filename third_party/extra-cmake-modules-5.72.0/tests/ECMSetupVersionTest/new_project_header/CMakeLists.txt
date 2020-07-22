cmake_minimum_required(VERSION 3.0.0)

project(new_project_header VERSION 2.3.4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
include(ECMSetupVersion)

ecm_setup_version(PROJECT
    VERSION_HEADER "ecm_new_project_header_version.h"
)

include(../version_helpers.cmake)
# NB: name comes from project() command
standard_version_var_checks(new_project_header 2.3.4)

add_executable(check_header main.c)
target_include_directories(check_header PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
