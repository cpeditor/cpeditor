cmake_minimum_required(VERSION 3.5)

cmake_policy(SET CMP0048 OLD)
project(old_simple)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
include(ECMSetupVersion)

ecm_setup_version(2.3.4 VARIABLE_PREFIX Foo)

include(../version_helpers.cmake)
standard_version_var_checks(Foo 2.3.4)

add_executable(dummy main.c)
