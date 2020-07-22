project(FindModules)
cmake_minimum_required(VERSION 3.5)

file(GLOB all_find_modules
     RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/../../find-modules"
     "${CMAKE_CURRENT_SOURCE_DIR}/../../find-modules/Find*cmake")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../find-modules)

foreach(module ${all_find_modules})
    string(REGEX REPLACE ".+Find([^\\.]+)\\.cmake" "\\1" packageName "${module}")
    message(STATUS "Finding package: ${packageName}")
    find_package(${packageName})

    if(NOT DEFINED ${packageName}_FOUND)
        message(SEND_ERROR "${packageName}_FOUND not defined!")
    endif()
endforeach()

add_executable(dummy main.c)
