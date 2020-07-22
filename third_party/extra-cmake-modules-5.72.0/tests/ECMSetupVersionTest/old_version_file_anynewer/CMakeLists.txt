cmake_minimum_required(VERSION 3.5)

cmake_policy(SET CMP0048 OLD)
project(old_version_file_anynewer)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
include(ECMSetupVersion)

ecm_setup_version(2.3.4
    VARIABLE_PREFIX Foo
    PACKAGE_VERSION_FILE FooVersion.cmake COMPATIBILITY AnyNewerVersion
)

include(../version_helpers.cmake)

standard_version_var_checks(Foo 2.3.4)
AnyNewer_2_3_4_checks("${CMAKE_CURRENT_BINARY_DIR}/FooVersion.cmake")

add_executable(dummy main.c)
