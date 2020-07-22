project(ECMQtDeclareLoggingCategoryTest)
cmake_minimum_required(VERSION 3.5)
set(ECM_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../modules")

set(CMAKE_MODULE_PATH ${ECM_MODULE_DIR})

include(ECMQtDeclareLoggingCategory)
include(CMakeParseArguments)

function (check_file)
    set(options)
    set(oneValueArgs GENERATED EXPECTED)
    set(multiValueArgs)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT EXISTS "${ARGS_GENERATED}")
        message(FATAL_ERROR "${ARGS_GENERATED} was not generated")
    endif()
    file(READ "${ARGS_GENERATED}" generated_contents)
    if (NOT EXISTS "${ARGS_EXPECTED}")
        message(FATAL_ERROR "Original ${ARGS_EXPECTED} was not found")
    endif()
    file(READ "${ARGS_EXPECTED}" original_contents)
    if (NOT "${generated_contents}" STREQUAL "${original_contents}")
        message(FATAL_ERROR "${generated_file} contains '${generated_contents}' instead of '${original_contents}'")
    endif()
endfunction()

ecm_qt_declare_logging_category(
    sources
    HEADER "log1.h"
    IDENTIFIER "log1"
    CATEGORY_NAME "log.one"
    OLD_CATEGORY_NAMES "log1old"
    DESCRIPTION "log 1"
    EXPORT LOG
)

ecm_qt_declare_logging_category(
    sources
    HEADER "log2.h"
    IDENTIFIER "foo::bar::log2"
    CATEGORY_NAME "log.two"
    DEFAULT_SEVERITY Info
    DESCRIPTION "log 2"
    EXPORT LOG
)

ecm_qt_declare_logging_category(
    sources
    HEADER "${CMAKE_CURRENT_BINARY_DIR}/log3.h"
    IDENTIFIER "log3"
    CATEGORY_NAME "three"
    DEFAULT_SEVERITY Critical
)

ecm_qt_export_logging_category(
    IDENTIFIER "log4"
    CATEGORY_NAME "log.four"
    OLD_CATEGORY_NAMES "logfouroldest" "log4old"
    DEFAULT_SEVERITY Warning
    EXPORT LOG
    DESCRIPTION "log 4"
)

find_package(Qt5Core REQUIRED)

add_executable(testmain testmain.cpp ${sources})
target_include_directories(testmain
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
)
target_link_libraries(testmain
    PRIVATE
        Qt5::Core
)

ecm_qt_install_logging_categories(
    EXPORT LOG
    FILE log.categories
    DESTINATION "${CMAKE_BINARY_DIR}/dummyinstall"
)

check_file(
    GENERATED "${CMAKE_CURRENT_BINARY_DIR}/log.categories"
    EXPECTED "${CMAKE_CURRENT_SOURCE_DIR}/log.categories"
)
check_file(
    GENERATED "${CMAKE_CURRENT_BINARY_DIR}/log.renamecategories"
    EXPECTED "${CMAKE_CURRENT_SOURCE_DIR}/log.renamecategories"
)

