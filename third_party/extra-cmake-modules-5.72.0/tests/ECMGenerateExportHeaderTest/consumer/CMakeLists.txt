cmake_minimum_required(VERSION 3.0.0)

project(consumer)

include(CheckCXXSourceCompiles)
include(CMakeParseArguments)

include(../../test_helpers.cmake)

include("${CMAKE_CURRENT_BINARY_DIR}/../installation/${LIBRARY}/lib/library/LibraryTargets.cmake")

# map any "CURRENT" value to library version string
# keep version in sync with the one used in library/CMakeLists.txt
set(library_version 2.40.0)
if(LIBRARY_DISABLE_DEPRECATED_BEFORE_AND_AT STREQUAL "CURRENT")
    set(LIBRARY_DISABLE_DEPRECATED_BEFORE_AND_AT ${library_version})
endif()
if(DEPRECATED_EXCLUDED_BEFORE_AND_AT STREQUAL "CURRENT")
    set(DEPRECATED_EXCLUDED_BEFORE_AND_AT ${library_version})
endif()

# load the test variant specific testAPI method
include(testAPI_${TEST_VARIANT}.cmake)

# for each API element test their visibility to the compiler and if a warning is emitted
set(_code "Enum enumerator = Enumerator_deprecatedAt2_0;")
testAPI(_code DEPRECATED_AT 2.0 BUILD_TIME_ONLY_DISABLABLE NO_WARNING)

set(_code "Enum enumerator = Enumerator_not_deprecated;")
testAPI(_code)

set(_code "function_deprecatedAt2_0();")
testAPI(_code DEPRECATED_AT 2.0)

set(_code "function_deprecatedAt2_12();")
testAPI(_code DEPRECATED_AT 2.12)

set(_code "function_not_deprecated();")
testAPI(_code)

set(_code "Class().method_deprecatedAt2_0();")
testAPI(_code DEPRECATED_AT 2.0 BUILD_TIME_ONLY_DISABLABLE)

set(_code "Class().method_deprecatedAt2_12();")
testAPI(_code DEPRECATED_AT 2.12 BUILD_TIME_ONLY_DISABLABLE)

set(_code "Class().method_not_deprecated();")
testAPI(_code)

add_executable(dummy main.cpp)
