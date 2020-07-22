project(ExecuteCoreModules)
cmake_minimum_required(VERSION 3.5)

file(GLOB all_core_modules "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/*cmake")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../modules)

foreach(module ${all_core_modules})
    message(STATUS "module: ${module}")
    include("${module}")
endforeach()

add_executable(dummy main.c)
