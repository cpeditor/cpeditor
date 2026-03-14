cmake_minimum_required(VERSION 3.5...3.12)

project(diff_match_patch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(
  diff_match_patch STATIC
  diff_match_patch.cpp
  diff_match_patch.h
)

target_link_libraries(diff_match_patch PRIVATE Qt${QT_VERSION_MAJOR}::Core)
