project(KDEInstallDirs.relative_or_absolute)
cmake_minimum_required(VERSION 3.5)

set(ECM_KDE_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../kde-modules)
set(ECM_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
set(CMAKE_MODULE_PATH "${ECM_KDE_MODULE_DIR}")
include(KDEInstallDirs)

include(../var_list.cmake)
include(../../test_helpers.cmake)

if(APPLE)
    # The default bundle directory is an absolute path
    set(BUNDLEDIR_should_be_absolute TRUE)
endif()

foreach(suffix ${var_suffixes})
    if(${suffix}_should_be_absolute)
        assert_var_absolute_path(KDE_INSTALL_${suffix})
    else()
        assert_var_relative_path(KDE_INSTALL_${suffix})
    endif()
    assert_var_absolute_path(KDE_INSTALL_FULL_${suffix})
endforeach()

add_executable(dummy main.c)
set_target_properties(dummy PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
