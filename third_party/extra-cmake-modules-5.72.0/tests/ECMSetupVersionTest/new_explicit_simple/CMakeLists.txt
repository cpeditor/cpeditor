cmake_minimum_required(VERSION 3.0.0)

project(new_explicit_simple VERSION 1.5.6.7)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
include(ECMSetupVersion)

ecm_setup_version(2.3.4 VARIABLE_PREFIX Foo)

include(../version_helpers.cmake)
project_version_var_checks(1.5.6.7)
version_var_checks(Foo 2.3.4)
assert_var_num_value(Foo_SOVERSION 2)

add_executable(dummy main.c)
