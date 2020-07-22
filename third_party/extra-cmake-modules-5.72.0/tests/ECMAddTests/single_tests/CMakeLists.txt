project(ECMAddTests)
cmake_minimum_required(VERSION 3.5)

set(ECM_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
set(CMAKE_MODULE_PATH "${ECM_MODULE_DIR}")

add_library(testhelper STATIC ../testhelper.cpp)
target_include_directories(testhelper PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

enable_testing()

include(ECMAddTests)

# clean up to avoid false-positives from check_files.cmake
file(REMOVE
    "${CMAKE_CURRENT_BINARY_DIR}/test1.txt"
    "${CMAKE_CURRENT_BINARY_DIR}/test2.txt"
    "${CMAKE_CURRENT_BINARY_DIR}/test3.txt"
    "${CMAKE_CURRENT_BINARY_DIR}/test4.txt"
    "${CMAKE_CURRENT_BINARY_DIR}/test5.txt"
    "${CMAKE_CURRENT_BINARY_DIR}/test6.txt"
    )

ecm_add_test(test1.cpp
    LINK_LIBRARIES testhelper
    )
# check target exists
get_property(_dummy TARGET test1 PROPERTY TYPE)
# check test exists
get_property(_dummy TEST test1 PROPERTY TIMEOUT)
get_property(_is_win32 TARGET test1 PROPERTY WIN32_EXECUTABLE)
if (_is_win32)
    message(FATAL_ERROR "test1 is a WIN32 executable when it should not be")
endif()
get_property(_is_bundle TARGET test1 PROPERTY MACOSX_BUNDLE)
if (_is_bundle)
    message(FATAL_ERROR "test1 is an OS/X bundle when it should not be")
endif()


ecm_add_test(test2.cpp
    LINK_LIBRARIES testhelper
    TEST_NAME named_test
    )
get_property(_dummy TARGET named_test PROPERTY TYPE)
get_property(_dummy TEST named_test PROPERTY TIMEOUT)
get_property(_is_win32 TARGET named_test PROPERTY WIN32_EXECUTABLE)
if (_is_win32)
    message(FATAL_ERROR "named_test is a WIN32 executable when it should not be")
endif()
get_property(_is_bundle TARGET named_test PROPERTY MACOSX_BUNDLE)
if (_is_bundle)
    message(FATAL_ERROR "named_test is an OS/X bundle when it should not be")
endif()


ecm_add_test(test3.cpp
    LINK_LIBRARIES testhelper
    NAME_PREFIX prefix_
    )
get_property(_dummy TARGET test3 PROPERTY TYPE)
get_property(_dummy TEST prefix_test3 PROPERTY TIMEOUT)
get_property(_is_win32 TARGET test3 PROPERTY WIN32_EXECUTABLE)
if (_is_win32)
    message(FATAL_ERROR "test3 is a WIN32 executable when it should not be")
endif()
get_property(_is_bundle TARGET test3 PROPERTY MACOSX_BUNDLE)
if (_is_bundle)
    message(FATAL_ERROR "test3 is an OS/X bundle when it should not be")
endif()


ecm_add_test(test4.cpp
    LINK_LIBRARIES testhelper
    GUI
    )
get_property(_dummy TARGET test4 PROPERTY TYPE)
get_property(_dummy TEST test4 PROPERTY TIMEOUT)
get_property(_is_win32 TARGET test4 PROPERTY WIN32_EXECUTABLE)
if (NOT _is_win32)
    message(FATAL_ERROR "test4 is not a WIN32 executable when it should be")
endif()
get_property(_is_bundle TARGET test4 PROPERTY MACOSX_BUNDLE)
if (NOT _is_bundle)
    message(FATAL_ERROR "test4 is not an OS/X bundle when it should be")
endif()


ecm_add_test(test5.cpp
    LINK_LIBRARIES testhelper
    TEST_NAME combined_test
    NAME_PREFIX another_prefix_
    GUI
    )
get_property(_dummy TARGET combined_test PROPERTY TYPE)
get_property(_dummy TEST another_prefix_combined_test PROPERTY TIMEOUT)
get_property(_is_win32 TARGET combined_test PROPERTY WIN32_EXECUTABLE)
if (NOT _is_win32)
    message(FATAL_ERROR "combined_test is not a WIN32 executable when it should be")
endif()
get_property(_is_bundle TARGET combined_test PROPERTY MACOSX_BUNDLE)
if (NOT _is_bundle)
    message(FATAL_ERROR "combined_test is not an OS/X bundle when it should be")
endif()


ecm_add_test(test6.cpp test6body.cpp
    LINK_LIBRARIES testhelper
    TEST_NAME multifile_test
    )
get_property(_dummy TARGET multifile_test PROPERTY TYPE)
get_property(_dummy TEST multifile_test PROPERTY TIMEOUT)
get_property(_is_win32 TARGET multifile_test PROPERTY WIN32_EXECUTABLE)
if (_is_win32)
    message(FATAL_ERROR "multifile_test is a WIN32 executable when it should not be")
endif()
get_property(_is_bundle TARGET multifile_test PROPERTY MACOSX_BUNDLE)
if (_is_bundle)
    message(FATAL_ERROR "multifile_test is an OS/X bundle when it should not be")
endif()


