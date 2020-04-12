cmake_minimum_required(VERSION 3.5)

project(cpeditor LANGUAGES CXX VERSION 6.3.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    SET(GUI_TYPE WIN32)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_subdirectory(third_party/QCodeEditor)

set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")

add_subdirectory(third_party/singleapplication)

add_subdirectory(third_party/QtFindReplaceDialog/dialogs)

add_subdirectory(third_party/lsp-cpp)

configure_file(version.hpp.in ${CMAKE_BINARY_DIR}/generated/version.hpp)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/generated/SettingsHelper.hpp ${CMAKE_BINARY_DIR}/generated/SettingsInfo.hpp 
                   COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/genSettings.py ${PROJECT_SOURCE_DIR}/setting.json
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   DEPENDS ${PROJECT_SOURCE_DIR}/setting.json ${PROJECT_SOURCE_DIR}/tools/genSettings.py)
set_property(SOURCE ${CMAKE_BINARY_DIR}/generated/SettingsHelper.hpp ${CMAKE_BINARY_DIR}/generated/SettingsInfo.hpp PROPERTY SKIP_AUTOGEN ON)

IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
  FIND_PACKAGE(Git)
  IF(GIT_FOUND)
    EXECUTE_PROCESS(
      COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      OUTPUT_VARIABLE "COMMIT_HASH"
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    add_definitions(-DGIT_COMMIT_HASH="${COMMIT_HASH}")
    MESSAGE( STATUS "Git commit hash: ${COMMIT_HASH}")
  ENDIF(GIT_FOUND)
ENDIF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)

add_executable(cpeditor
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

    include/Widgets/DiffViewer.hpp
    include/Widgets/TestCaseEdit.hpp
    include/Widgets/TestCase.hpp
    include/Widgets/TestCases.hpp
    src/Widgets/DiffViewer.cpp
    src/Widgets/TestCaseEdit.cpp
    src/Widgets/TestCase.cpp
    src/Widgets/TestCases.cpp

    include/Extensions/CompanionServer.hpp
    include/Extensions/CFTool.hpp
    include/Extensions/EditorTheme.hpp
    src/Extensions/CompanionServer.cpp
    src/Extensions/CFTool.cpp
    src/Extensions/EditorTheme.cpp

    include/Preferences/ValueWrapper.hpp
    include/Preferences/PreferencesPage.hpp
    include/Preferences/PreferencesHomePage.hpp
    include/Preferences/PreferencesWindow.hpp
    include/Preferences/PreferencesGridPage
    include/Preferences/PreferencesPageTemplate.hpp
    include/Preferences/FontItem.hpp
    include/Preferences/PathItem.hpp
    include/Preferences/ShortcutItem.hpp
    include/Preferences/CodeSnippetsPage.hpp
    src/Preferences/ValueWrapper.cpp
    src/Preferences/PreferencesPage.cpp
    src/Preferences/PreferencesHomePage.cpp
    src/Preferences/PreferencesWindow.cpp
    src/Preferences/PreferencesGridPage.cpp
    src/Preferences/PreferencesPageTemplate.cpp
    src/Preferences/FontItem.cpp
    src/Preferences/PathItem.cpp
    src/Preferences/ShortcutItem.cpp
    src/Preferences/CodeSnippetsPage.cpp

    include/mainwindow.hpp
    include/appwindow.hpp
    src/mainwindow.cpp
    src/appwindow.cpp

    include/Util.hpp
    src/Util.cpp

    third_party/diff_match_patch/diff_match_patch.h
    third_party/diff_match_patch/diff_match_patch.cpp

    src/main.cpp

    ui/mainwindow.ui
    ui/appwindow.ui

    ${CMAKE_BINARY_DIR}/generated/version.hpp
    ${CMAKE_BINARY_DIR}/generated/SettingsHelper.hpp
    ${CMAKE_BINARY_DIR}/generated/SettingsInfo.hpp

    resources/resources.qrc
    assets/appicon.rc)

include_directories("third_party/QCodeEditor/include")
include_directories("third_party/diff_match_patch")
include_directories("third_party/QtFindReplaceDialog")
include_directories("third_party/lsp-cpp/include")
include_directories("include/")
include_directories("generated/")

target_link_libraries(cpeditor PRIVATE Qt5::Widgets)
target_link_libraries(cpeditor PRIVATE Qt5::Network)
target_link_libraries(cpeditor PRIVATE QCodeEditor)
target_link_libraries(cpeditor PRIVATE SingleApplication)
target_link_libraries(cpeditor PRIVATE QtFindReplaceDialog)
target_link_libraries(cpeditor PRIVATE LSPClient)
 
if(APPLE)
    set_target_properties(cpeditor
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
