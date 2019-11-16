cmake_minimum_required(VERSION 3.5)

project(CPEditor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(third_party/QCodeEditor)
add_executable(CPEditor
    src/main.cc
    src/mainwindow.cc
    ui/mainwindow.ui
    include/mainwindow.hpp
    include/MessageLogger.hpp
    include/SettingsManager.hpp
    include/Core.hpp
    include/IO.hpp
    src/IO.cpp
    include/Runner.hpp
    src/Runner.cpp
    src/SettngsManager.cpp
    src/Core.cpp
    include/Formatter.hpp
    src/Formatter.cpp
    include/BaseFiles.hpp
    src/MessageLogger.cpp

    resources/resources.qrc
    appicon.rc)

include_directories("third_party/QCodeEditor/include")
include_directories("include/")

target_link_libraries(CPEditor PRIVATE Qt5::Widgets)
target_link_libraries(CPEditor PRIVATE QCodeEditor)
