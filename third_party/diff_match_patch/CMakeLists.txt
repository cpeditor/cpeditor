cmake_minimum_required(VERSION 3.5)

project(diff_match_patch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(
  diff_match_patch STATIC
  diff_match_patch.cpp
  diff_match_patch.h
)

target_link_libraries(diff_match_patch PRIVATE Qt5::Core)
