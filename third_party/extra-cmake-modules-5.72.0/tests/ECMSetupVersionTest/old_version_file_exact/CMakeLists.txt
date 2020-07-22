cmake_minimum_required(VERSION 3.5)

cmake_policy(SET CMP0048 OLD)
project(old_version_file_exact)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
include(ECMSetupVersion)

ecm_setup_version(2.3.4
    VARIABLE_PREFIX Foo
    PACKAGE_VERSION_FILE FooVersion.cmake COMPATIBILITY ExactVersion
)

include(../../test_helpers.cmake)
include(../version_helpers.cmake)

macro(find_foo version)
    test_version_file("${CMAKE_CURRENT_BINARY_DIR}/FooVersion.cmake" "${version}")
    assert_var_str_value(PACKAGE_VERSION "2.3.4")
endmacro()

standard_version_var_checks(Foo 2.3.4)

# too old - fails
find_foo("3.1.1")
assert_var_bool_value(PACKAGE_VERSION_COMPATIBLE FALSE ALLOW_UNDEFINED)
assert_var_bool_value(PACKAGE_VERSION_EXACT FALSE ALLOW_UNDEFINED)

# wrong major version - fails
find_foo("1.1.1")
assert_var_bool_value(PACKAGE_VERSION_COMPATIBLE FALSE ALLOW_UNDEFINED)
assert_var_bool_value(PACKAGE_VERSION_EXACT FALSE ALLOW_UNDEFINED)

# wrong minor - fails
find_foo("2.1.1")
assert_var_bool_value(PACKAGE_VERSION_COMPATIBLE FALSE ALLOW_UNDEFINED)
assert_var_bool_value(PACKAGE_VERSION_EXACT FALSE ALLOW_UNDEFINED)

# wrong patch - fails
find_foo("2.3.1")
assert_var_bool_value(PACKAGE_VERSION_COMPATIBLE FALSE ALLOW_UNDEFINED)
assert_var_bool_value(PACKAGE_VERSION_EXACT FALSE ALLOW_UNDEFINED)

# tweak doesn't matter (but prevents P_V_EXACT being set) - succeeds
find_foo("2.3.4.3")
assert_var_bool_value(PACKAGE_VERSION_COMPATIBLE TRUE)
assert_var_bool_value(PACKAGE_VERSION_EXACT FALSE ALLOW_UNDEFINED)

# exact - succeeds
find_foo("2.3.4")
assert_var_bool_value(PACKAGE_VERSION_COMPATIBLE TRUE)
assert_var_bool_value(PACKAGE_VERSION_EXACT TRUE)

add_executable(dummy main.c)
