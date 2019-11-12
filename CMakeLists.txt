cmake_minimum_required(VERSION 3.5)

project(CPEditor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)
link_directories("third_party/QCodeEditor/lib")
add_executable(CPEditor
    src/main.cc
    src/mainwindow.cc
    ui/mainwindow.ui
    include/mainwindow.hpp
    include/MessageLogger.hpp
    include/SettingsManager.hpp
    include/ChildManager.hpp
    src/SettngsManager.cpp
    src/ChildManager.cpp
    src/MessageLogger.cpp

    resources/resources.qrc)

include_directories("third_party/QCodeEditor/include")
include_directories("include/")

target_link_libraries(CPEditor PRIVATE Qt5::Widgets)
target_link_libraries(CPEditor PRIVATE libQCodeEditor.a)
