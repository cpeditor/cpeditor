cmake_minimum_required(VERSION 3.5)

project(CPEditor LANGUAGES CXX VERSION 5.9.9)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    SET(GUI_TYPE WIN32)
endif()


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

add_subdirectory(third_party/QCodeEditor)

set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")
add_subdirectory(third_party/singleapplication)

add_subdirectory(third_party/QtFindReplaceDialog/dialogs)

configure_file(version.hpp.in ${CMAKE_BINARY_DIR}/generated/version.hpp)

add_executable(CPEditor
    ${GUI_TYPE}

    include/Core/Checker.hpp
    include/Core/Compiler.hpp
    include/Core/Runner.hpp
    include/Core/Formatter.hpp
    include/Core/SettingsManager.hpp
    include/Core/MessageLogger.hpp
	include/Core/EventLogger.hpp
    src/Core/Checker.cpp
    src/Core/Compiler.cpp
    src/Core/Runner.cpp
    src/Core/Formatter.cpp
    src/Core/SettingsManager.cpp
    src/Core/MessageLogger.cpp
	src/Core/EventLogger.cpp

    include/Telemetry/UpdateNotifier.hpp
    src/Telemetry/UpdateNotifier.cpp

    include/Widgets/TestCases.hpp
    src/Widgets/TestCases.cpp

    include/Extensions/CompanionServer.hpp
    include/Extensions/CFTools.hpp
    include/Extensions/EditorTheme.hpp
    src/Extensions/CompanionServer.cpp
    src/Extensions/CFTools.cpp
    src/Extensions/EditorTheme.cpp

    include/mainwindow.hpp
    include/preferencewindow.hpp
    include/appwindow.hpp
    src/mainwindow.cpp
    src/preferencewindow.cpp
    src/appwindow.cpp

    include/Util.hpp
    src/Util.cpp

    third_party/diff_match_patch/diff_match_patch.h
    third_party/diff_match_patch/diff_match_patch.cpp

    src/main.cpp

    ui/mainwindow.ui
    ui/preferencewindow.ui
    ui/appwindow.ui

    ${CMAKE_BINARY_DIR}/generated/version.hpp

    resources/resources.qrc
    assets/appicon.rc)

include_directories("third_party/QCodeEditor/include")
include_directories("third_party/diff_match_patch")
include_directories("third_party/QtFindReplaceDialog")
include_directories("include/")
include_directories("generated/")

target_link_libraries(CPEditor PRIVATE Qt5::Widgets)
target_link_libraries(CPEditor PRIVATE Qt5::Network)
target_link_libraries(CPEditor PRIVATE QCodeEditor)
target_link_libraries(CPEditor PRIVATE SingleApplication)
target_link_libraries(CPEditor PRIVATE QtFindReplaceDialog)
 
if(APPLE)
    set_target_properties(CPEditor
        PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.coder3101.cpeditor"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
        MACOSX_BUNDLE_ICON_FILE "cpeditor.icns"
        MACOSX_BUNDLE_COPYRIGHT "GNU GPL v3"
        RESOURCE ".ci/mac/cpeditor.icns"
    )
endif()
