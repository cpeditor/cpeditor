cmake_minimum_required(VERSION 3.0.0)

project(format_version)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
include(ECMGenerateExportHeader)

include(../../test_helpers.cmake)

set(_current_version "4.5.6")

# check 0: with CURRENT_VERSION
ecm_export_header_format_version(0
    CURRENT_VERSION ${_current_version}
    STRING_VAR _versionstring
    HEXNUMBER_VAR _hexnumber
)

assert_var_str_value(_versionstring "0")
assert_var_str_value(_hexnumber "0")

# check 0: no CURRENT_VERSION
ecm_export_header_format_version(0
    STRING_VAR _versionstring
    HEXNUMBER_VAR _hexnumber
)

assert_var_str_value(_versionstring "0")
assert_var_str_value(_hexnumber "0")

# check some version: with CURRENT_VERSION
ecm_export_header_format_version(1.2.3
    CURRENT_VERSION ${_current_version}
    STRING_VAR _versionstring
    HEXNUMBER_VAR _hexnumber
)

assert_var_str_value(_versionstring "1.2.3")
if (NOT CMAKE_VERSION VERSION_LESS 3.13)
    assert_var_str_value(_hexnumber "0x10203")
else()
    assert_var_str_value(_hexnumber "66051")
endif()

# check some version: no CURRENT_VERSION
ecm_export_header_format_version(1.2.3
    STRING_VAR _versionstring
    HEXNUMBER_VAR _hexnumber
)

assert_var_str_value(_versionstring "1.2.3")
if (NOT CMAKE_VERSION VERSION_LESS 3.13)
    assert_var_str_value(_hexnumber "0x10203")
else()
    assert_var_str_value(_hexnumber "66051")
endif()

# check CURRENT
ecm_export_header_format_version(CURRENT
    CURRENT_VERSION ${_current_version}
    STRING_VAR _versionstring
    HEXNUMBER_VAR _hexnumber
)

assert_var_str_value(_versionstring "4.5.6")
if (NOT CMAKE_VERSION VERSION_LESS 3.13)
    assert_var_str_value(_hexnumber "0x40506")
else()
    assert_var_str_value(_hexnumber "263430")
endif()

add_executable(dummy main.cpp)
