cmake_minimum_required(VERSION 3.5)

cmake_policy(SET CMP0048 OLD)
project(old_header_abspath)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
include(ECMSetupVersion)

ecm_setup_version(2.3.4
    VARIABLE_PREFIX Foo
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/../ecm_old_header_abspath_version.h"
)

include(../version_helpers.cmake)

standard_version_var_checks(Foo 2.3.4)

add_executable(check_header main.c)
target_include_directories(check_header PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/..")
