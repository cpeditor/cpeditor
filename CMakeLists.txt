cmake_minimum_required(VERSION 3.5)

project(CPEditor LANGUAGES CXX VERSION 5.7.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    SET(GUI_TYPE WIN32)
endif()
if(APPLE)
    SET(GUI_TYPE MACOSX_BUNDLE)
endif()


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

add_subdirectory(third_party/QCodeEditor)

set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")
add_subdirectory(third_party/singleapplication)

configure_file(version.hpp.in ${CMAKE_BINARY_DIR}/generated/version.hpp)

add_executable(CPEditor
    ${GUI_TYPE}

    include/mainwindow.hpp
    include/MessageLogger.hpp
    include/SettingsManager.hpp
    include/Compiler.hpp
    include/IO.hpp
    include/BaseFiles.hpp
    include/UpdateNotifier.hpp
    include/Runner.hpp
    include/Formatter.hpp
    include/TestCases.hpp
    include/CompanionServer.hpp
    include/cftools.hpp
    include/EditorTheme.hpp
    include/preferencewindow.hpp
    include/appwindow.hpp
    third_party/diff_match_patch/diff_match_patch.h

    src/mainwindow.cc
    src/MessageLogger.cpp
    src/SettingsManager.cpp
    src/main.cc
    src/Compiler.cpp
    src/IO.cpp
    src/UpdateNotifier.cpp
    src/Runner.cpp
    src/Formatter.cpp
    src/TestCases.cpp
    src/CompanionServer.cpp
    src/cftools.cpp
    src/editortheme.cpp
    src/preferencewindow.cpp
    src/appwindow.cpp
    third_party/diff_match_patch/diff_match_patch.cpp

    ui/mainwindow.ui
    ui/preferencewindow.ui
    ui/appwindow.ui

    ${CMAKE_BINARY_DIR}/generated/version.hpp

    resources/resources.qrc
    assets/appicon.rc)

include_directories("third_party/QCodeEditor/include")
include_directories("third_party/diff_match_patch")
include_directories("include/")
include_directories("generated/")

target_link_libraries(CPEditor PRIVATE Qt5::Widgets)
target_link_libraries(CPEditor PRIVATE Qt5::Network)
target_link_libraries(CPEditor PRIVATE QCodeEditor)
target_link_libraries(CPEditor PRIVATE SingleApplication)
