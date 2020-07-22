cmake_minimum_required(VERSION 3.0.0)

project(new_project_soversion VERSION 2.3.4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
include(ECMSetupVersion)

ecm_setup_version(PROJECT
    SOVERSION 1
)

include(../version_helpers.cmake)
# NB: name comes from project() command
standard_version_var_checks(new_project_soversion 2.3.4 1)

add_executable(dummy main.c)
