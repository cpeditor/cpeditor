project(UseFindModules)
cmake_minimum_required(VERSION 3.5)

set(ECM_FIND_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../find-modules")
set(ECM_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../modules")

file(GLOB all_find_modules
     RELATIVE "${ECM_FIND_MODULE_DIR}"
     "${ECM_FIND_MODULE_DIR}/Find*cmake")

set(package_list)
foreach(module ${all_find_modules})
    string(REGEX REPLACE ".+Find([^\\.]+)\\.cmake" "\\1" packageName "${module}")
    list(APPEND package_list "${packageName}")
endforeach()

include("${ECM_MODULE_DIR}/ECMUseFindModules.cmake")
ecm_use_find_modules(DIR "${CMAKE_CURRENT_BINARY_DIR}/cmake"
                     MODULES ${package_list})

foreach(packageName ${package_list})
    message(STATUS "Finding package: ${packageName}")
    find_package(${packageName})

    if(NOT DEFINED ${packageName}_FOUND)
        message(SEND_ERROR "${packageName}_FOUND not defined!")
    endif()
endforeach()

add_executable(dummy main.c)
