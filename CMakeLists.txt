cmake_minimum_required(VERSION 3.5)

if(APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version")
endif()

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

configure_file(cmake/version.hpp.in ${CMAKE_BINARY_DIR}/generated/version.hpp)
configure_file(cmake/aur/PKGBUILD.in ${CMAKE_BINARY_DIR}/aur/PKGBUILD)
configure_file(cmake/aur/.SRCINFO.in ${CMAKE_BINARY_DIR}/aur/.SRCINFO)

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

    src/Core/Checker.cpp
    src/Core/Checker.hpp
    src/Core/Compiler.cpp
    src/Core/Compiler.hpp
    src/Core/EventLogger.cpp
    src/Core/EventLogger.hpp
    src/Core/Formatter.cpp
    src/Core/Formatter.hpp
    src/Core/MessageLogger.cpp
    src/Core/MessageLogger.hpp
    src/Core/Runner.cpp
    src/Core/Runner.hpp
    src/Core/SettingsManager.cpp
    src/Core/SettingsManager.hpp

    src/Telemetry/UpdateNotifier.cpp
    src/Telemetry/UpdateNotifier.hpp

    src/Widgets/DiffViewer.cpp
    src/Widgets/DiffViewer.hpp
    src/Widgets/TestCase.cpp
    src/Widgets/TestCase.hpp
    src/Widgets/TestCaseEdit.cpp
    src/Widgets/TestCaseEdit.hpp
    src/Widgets/TestCases.cpp
    src/Widgets/TestCases.hpp

    src/Extensions/CFTool.cpp
    src/Extensions/CFTool.hpp
    src/Extensions/CompanionServer.cpp
    src/Extensions/CompanionServer.hpp
    src/Extensions/EditorTheme.cpp
    src/Extensions/EditorTheme.hpp

    src/Preferences/CodeSnippetsPage.cpp
    src/Preferences/CodeSnippetsPage.hpp
    src/Preferences/FontItem.cpp
    src/Preferences/FontItem.hpp
    src/Preferences/PathItem.cpp
    src/Preferences/PathItem.hpp
    src/Preferences/PreferencesGridPage.cpp
    src/Preferences/PreferencesGridPage.hpp
    src/Preferences/PreferencesHomePage.cpp
    src/Preferences/PreferencesHomePage.hpp
    src/Preferences/PreferencesPage.cpp
    src/Preferences/PreferencesPage.hpp
    src/Preferences/PreferencesPageTemplate.cpp
    src/Preferences/PreferencesPageTemplate.hpp
    src/Preferences/PreferencesWindow.cpp
    src/Preferences/PreferencesWindow.hpp
    src/Preferences/ShortcutItem.cpp
    src/Preferences/ShortcutItem.hpp
    src/Preferences/ValueWrapper.cpp
    src/Preferences/ValueWrapper.hpp

    src/appwindow.cpp
    src/appwindow.hpp
    src/mainwindow.cpp
    src/mainwindow.hpp

    src/Util.cpp
    src/Util.hpp

    src/main.cpp

    third_party/diff_match_patch/diff_match_patch.h
    third_party/diff_match_patch/diff_match_patch.cpp

    ui/mainwindow.ui
    ui/appwindow.ui

    ${CMAKE_BINARY_DIR}/generated/version.hpp
    ${CMAKE_BINARY_DIR}/generated/SettingsHelper.hpp
    ${CMAKE_BINARY_DIR}/generated/SettingsInfo.hpp

    resources/resources.qrc
    assets/appicon.rc)

include_directories("generated/")
include_directories("src/")
include_directories("third_party/diff_match_patch")
include_directories("third_party/lsp-cpp/include")
include_directories("third_party/QCodeEditor/include")
include_directories("third_party/QtFindReplaceDialog")

target_link_libraries(cpeditor PRIVATE LSPClient)
target_link_libraries(cpeditor PRIVATE QCodeEditor)
target_link_libraries(cpeditor PRIVATE Qt5::Network)
target_link_libraries(cpeditor PRIVATE Qt5::Widgets)
target_link_libraries(cpeditor PRIVATE QtFindReplaceDialog)
target_link_libraries(cpeditor PRIVATE SingleApplication)

if(APPLE)
    set_target_properties(cpeditor
        PROPERTIES
        MACOSX_BUNDLE TRUE
	MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/Info.plist.in
        RESOURCE ".ci/mac/cpeditor.icns"
    )
endif()

if(UNIX AND NOT APPLE)
	install(TARGETS cpeditor
		RUNTIME DESTINATION bin
        	LIBRARY DESTINATION lib)
	install(FILES .ci/linux/cpeditor.desktop DESTINATION share/applications)
	install(FILES .ci/linux/cpeditor-icon.png DESTINATION share/icons)
endif()
