cmake_minimum_required(VERSION 3.5)

project(cpeditor LANGUAGES CXX VERSION 6.1.6)

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

add_subdirectory(third_party/QCodeEditor)

set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")

add_subdirectory(third_party/singleapplication)

add_subdirectory(third_party/QtFindReplaceDialog/dialogs)

add_subdirectory(third_party/lsp-cpp)

configure_file(cmake/version.hpp.in ${CMAKE_BINARY_DIR}/generated/version.hpp)
configure_file(scripts/aur/PKGBUILD.in ${CMAKE_BINARY_DIR}/aur/PKGBUILD)
configure_file(scripts/aur/.SRCINFO.in ${CMAKE_BINARY_DIR}/aur/.SRCINFO)

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
