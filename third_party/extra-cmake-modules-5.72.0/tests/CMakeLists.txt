#=============================================================================
# SPDX-FileCopyrightText: 2011 Alex Neundorf <neundorf@kde.org>
# SPDX-FileCopyrightText: 2014-2015 Alex Merry <alex.merry@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

# We need to set a language to find CMake packages installed in
# architecture-dependent locations (like /usr/lib64).
# We only set this for the tests, making sure it does not interfere
# with other files (as ECM itself is architecture-independent).
project(ECMTests C)

find_package(Qt5LinguistTools CONFIG)
set_package_properties(
    Qt5LinguistTools
    PROPERTIES
        URL "https://www.qt.io/"
        DESCRIPTION "Qt5 linguist tools."
        TYPE OPTIONAL
        PURPOSE "Required to run tests for the ECMPoQmTools module."
)

find_package(Qt5Core CONFIG)
set_package_properties(
    Qt5Core
    PROPERTIES
        URL "https://www.qt.io/"
        DESCRIPTION "Qt5 core library."
        TYPE OPTIONAL
        PURPOSE "Required to run tests for the ECMQtDeclareLoggingCategory module, and for some tests of the KDEInstallDirs module."
)

add_subdirectory(ECMAddTests)
add_subdirectory(ECMGenerateExportHeaderTest)
add_subdirectory(ECMGenerateHeadersTest)
add_subdirectory(ECMSetupVersionTest)
add_subdirectory(ECMGeneratePkgConfigFile)

# a macro for tests that have a simple format where the name matches the
# directory and project
macro(add_test_variant NAME BASE COMMAND)
    string(REPLACE "." "/" src_dir "${BASE}")
    string(REPLACE "." "/" build_dir "${NAME}")
    string(REGEX REPLACE "[^.]*\\." "" proj "${NAME}")
    add_test(${NAME} ${CMAKE_CTEST_COMMAND}
        --build-and-test
        "${CMAKE_CURRENT_SOURCE_DIR}/${src_dir}"
        "${CMAKE_CURRENT_BINARY_DIR}/${build_dir}"
        --build-two-config
        --build-generator ${CMAKE_GENERATOR}
        --build-makeprogram ${CMAKE_MAKE_PROGRAM}
        --build-project ${proj}
        ${${NAME}_EXTRA_OPTIONS}
        --test-command ${COMMAND} ${ARGN})
endmacro()
macro(add_test_macro NAME)
    add_test_variant("${NAME}" "${NAME}" ${ARGN})
endmacro()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/find-modules)

# Skip if PyQt not available
find_file(SIP_Qt5Core_Mod_FILE
    NAMES QtCoremod.sip
    PATH_SUFFIXES share/sip/PyQt5/QtCore
)

if(NOT SIP_Qt5Core_Mod_FILE)
    message(STATUS "WARNING: skipping tests that require PyQt")
else()
  find_package(PythonModuleGeneration)
  foreach(pyversion 2 3)
    if (GPB_PYTHON${pyversion}_COMMAND)
      if (pythonCommands)
        list(APPEND pythonCommands " && ")
      endif()
      set(pythonCommands
        ${GPB_PYTHON${pyversion}_COMMAND}
        "${CMAKE_CURRENT_SOURCE_DIR}/GenerateSipBindings/testscript.py"
        "${CMAKE_CURRENT_BINARY_DIR}/GenerateSipBindings/py${pyversion}"
      )
    endif()
  endforeach()
  if (pythonCommands)
    add_test_macro(GenerateSipBindings ${pythonCommands})
 else()
    message(STATUS "WARNING: skipping GenerateSipBindings test")
  endif()
endif()

add_test_macro(ExecuteCoreModules dummy)
add_test_macro(ExecuteKDEModules dummy)
add_test_macro(KDEFetchTranslations dummy)

add_test_variant(KDEInstallDirsTest.vars_in_sync_no_args
                 KDEInstallDirsTest.vars_in_sync dummy)
add_test_macro(KDEInstallDirsTest.not_cache_variable dummy)

set(KDEInstallDirsTest.vars_in_sync_kde_arg_EXTRA_OPTIONS
    --build-options -DKDE_INSTALL_BINDIR=altbin
                    -DBINDIR_expected_value=altbin
                    -DCMAKE_WARN_DEPRECATED=TRUE
    )
add_test_variant(KDEInstallDirsTest.vars_in_sync_kde_arg
                 KDEInstallDirsTest.vars_in_sync dummy)

set(KDEInstallDirsTest.vars_in_sync_cmake_arg_EXTRA_OPTIONS
    --build-options -DCMAKE_INSTALL_BINDIR=altbin
                    -DBINDIR_expected_value=altbin
                    -DCMAKE_WARN_DEPRECATED=TRUE
    )
add_test_variant(KDEInstallDirsTest.vars_in_sync_cmake_arg
                 KDEInstallDirsTest.vars_in_sync dummy)

set(KDEInstallDirsTest.vars_in_sync_oldstyle_arg_EXTRA_OPTIONS
    --build-options -DBIN_INSTALL_DIR=altbin
                    -DBINDIR_expected_value=altbin
                    -DCMAKE_WARN_DEPRECATED=TRUE
    )
add_test_variant(KDEInstallDirsTest.vars_in_sync_oldstyle_arg
                 KDEInstallDirsTest.vars_in_sync dummy)

set(KDEInstallDirsTest.relative_or_absolute_usr_EXTRA_OPTIONS
    --build-options -DCMAKE_INSTALL_PREFIX=/usr
                    -DKDE_INSTALL_USE_QT_SYS_PATHS=FALSE
                    -DAUTOSTARTDIR_should_be_absolute=TRUE
                    -DCONFDIR_should_be_absolute=TRUE
                    -DSYSCONFDIR_should_be_absolute=TRUE
    )
add_test_variant(KDEInstallDirsTest.relative_or_absolute_usr
                 KDEInstallDirsTest.relative_or_absolute dummy)

if (TARGET Qt5::qmake)
    set(KDEInstallDirsTest.relative_or_absolute_qt_EXTRA_OPTIONS
        --build-options -DCMAKE_INSTALL_PREFIX=/tmp
                        -DKDE_INSTALL_USE_QT_SYS_PATHS=TRUE
                        -DPLUGINDIR_should_be_absolute=TRUE
                        -DQMLDIR_should_be_absolute=TRUE
                        -DQTQCHDIR_should_be_absolute=TRUE
                        -DQTPLUGINDIR_should_be_absolute=TRUE
                        -DQTQUICKIMPORTSDIR_should_be_absolute=TRUE
        )
    add_test_variant(KDEInstallDirsTest.relative_or_absolute_qt
                     KDEInstallDirsTest.relative_or_absolute dummy)
endif ()
if (Qt5Core_FOUND)
    add_test_macro(ECMQtDeclareLoggingCategoryTest testmain)
endif()

add_test_macro(FindModules dummy)
add_test_macro(UseFindModules dummy)

set(ECMAddAppIconTest_EXTRA_OPTIONS
    --build-target all
    --build-options
        "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/ECMAddAppIconTest/InstallDirectory"
)
add_test_macro(ECMAddAppIconTest
    ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/ECMAddAppIconTest/check_files.cmake"
)

set(ECMInstallIconsTest_EXTRA_OPTIONS
    --build-target install
    --build-options
        "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/ECMInstallIconsTest/InstallDirectory"
)
add_test_macro(ECMInstallIconsTest
    ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/ECMInstallIconsTest/check_tree.cmake"
)

set(KDEPackageAppTemplatesTest_EXTRA_OPTIONS
    --build-target install
    --build-options
        "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/KDEPackageAppTemplatesTest/InstallDirectory"
)
add_test_macro(KDEPackageAppTemplatesTest
    ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/KDEPackageAppTemplatesTest/check.cmake"
)

if (Qt5Core_FOUND AND Qt5LinguistTools_FOUND)
    set(ECMPoQmToolsTest_EXTRA_OPTIONS
        --build-target install
        --build-options
            "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/ECMPoQmToolsTest/InstallDirectory"
    )
    add_test_macro(ECMPoQmToolsTest
        ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/ECMPoQmToolsTest/check.cmake"
    )
endif()

find_package(Qt5Quick CONFIG)
set_package_properties(
    Qt5Quick
    PROPERTIES
        URL "https://www.qt.io/"
        DESCRIPTION "Qt5 Quick library."
        TYPE OPTIONAL
        PURPOSE "Required to run tests for the ECMQMLModules module."
)
if (TARGET Qt5::Quick)
    add_test_macro(ECMQMLModules dummy)
endif()
