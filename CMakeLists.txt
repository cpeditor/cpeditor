cmake_minimum_required(VERSION 3.5)

project(CPEditor LANGUAGES CXX VERSION 3.1.0)

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

configure_file(version.hpp.in ${CMAKE_BINARY_DIR}/generated/version.hpp)

add_executable(CPEditor
    ${GUI_TYPE}
    src/main.cc
    src/mainwindow.cc
    ui/mainwindow.ui
    ui/expand.ui
    include/mainwindow.hpp
    include/MessageLogger.hpp
    include/SettingsManager.hpp
    include/Core.hpp
    include/IO.hpp
    include/UpdateNotifier.hpp
    src/UpdateNotifier.cpp
    src/IO.cpp
    include/Runner.hpp
    src/Runner.cpp
    src/SettingsManager.cpp
    src/Core.cpp
    include/Formatter.hpp
    src/Formatter.cpp
    include/BaseFiles.hpp
    src/MessageLogger.cpp
    include/expand.hpp
    src/expand.cpp
    ui/diffviewer.ui
    include/DiffViewer.hpp
    src/DiffViewer.cpp
    src/CompanionServer.cpp
    include/CompanionServer.hpp

    ${CMAKE_BINARY_DIR}/generated/version.hpp

    resources/resources.qrc
    third_party/qdarkstyle/qdarkstyle/style.qrc
    appicon.rc)

include_directories("third_party/QCodeEditor/include")
include_directories("include/")
include_directories("generated/")

target_link_libraries(CPEditor PRIVATE Qt5::Widgets)
target_link_libraries(CPEditor PRIVATE Qt5::Network)
target_link_libraries(CPEditor PRIVATE QCodeEditor)
