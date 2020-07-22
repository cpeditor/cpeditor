# Test KDEInstallDirs does not crash if a variable like CMAKE_INSTALL_LIBDIR is
# defined but is not a cache variable.
cmake_minimum_required(VERSION 2.8)

set(ECM_KDE_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../kde-modules)
set(ECM_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
set(CMAKE_MODULE_PATH "${ECM_KDE_MODULE_DIR}")

set(KDE_INSTALL_BINDIR "altbin")
set(CMAKE_INSTALL_DATADIR "altdata")
set(LIB_INSTALL_DIR "altlib")

include(KDEInstallDirs)


include(../var_list.cmake)
include(../../test_helpers.cmake)

message(STATUS "Checking result of setting KDE_INSTALL_BINDIR")
assert_var_str_value(KDE_INSTALL_BINDIR "altbin")
if(should_set_CMAKE_INSTALL_BINDIR)
    assert_var_str_value(CMAKE_INSTALL_BINDIR "altbin")
endif()
if(NOT KDE_INSTALL_DIRS_NO_DEPRECATED)
    assert_var_str_value(BIN_INSTALL_DIR "altbin")
endif()

if(should_set_CMAKE_INSTALL_DATADIR)
    message(STATUS "Checking result of setting CMAKE_INSTALL_DATADIR")
    assert_var_str_value(KDE_INSTALL_DATADIR "altdata")
    assert_var_str_value(CMAKE_INSTALL_DATADIR "altdata")
    if(NOT KDE_INSTALL_DIRS_NO_DEPRECATED)
        assert_var_str_value(DATA_INSTALL_DIR "altdata")
    endif()
endif()

if(NOT KDE_INSTALL_DIRS_NO_DEPRECATED)
    message(STATUS "Checking result of setting LIB_INSTALL_DIR")
    assert_var_str_value(KDE_INSTALL_LIBDIR "altlib")
    if(should_set_CMAKE_INSTALL_LIBDIR)
        assert_var_str_value(CMAKE_INSTALL_LIBDIR "altlib")
    endif()
    assert_var_str_value(LIB_INSTALL_DIR "altlib")
endif()

add_executable(dummy main.c)
set_target_properties(dummy PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
