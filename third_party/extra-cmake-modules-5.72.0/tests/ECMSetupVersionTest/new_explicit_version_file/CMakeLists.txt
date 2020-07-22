cmake_minimum_required(VERSION 3.0.0)

project(new_explicit_version_file VERSION 1.5.6.7)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
include(ECMSetupVersion)

ecm_setup_version(2.3.4
    VARIABLE_PREFIX Foo
    PACKAGE_VERSION_FILE FooVersion.cmake
)

include(../version_helpers.cmake)
project_version_var_checks(1.5.6.7)
version_var_checks(Foo 2.3.4)
assert_var_num_value(Foo_SOVERSION 2)

AnyNewer_2_3_4_checks("${CMAKE_CURRENT_BINARY_DIR}/FooVersion.cmake")

add_executable(dummy main.c)
