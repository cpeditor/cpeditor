cmake_minimum_required(VERSION 3.5)

find_package(yaml-cpp REQUIRED)

add_executable(gen_settings genSettings.cpp)
target_link_libraries(gen_settings PRIVATE yaml-cpp)