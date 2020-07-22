project(KDEInstallDirs.vars_in_sync)
cmake_minimum_required(VERSION 3.5)

set(ECM_KDE_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../kde-modules)
set(ECM_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules)
set(CMAKE_MODULE_PATH "${ECM_KDE_MODULE_DIR}")
include(KDEInstallDirs)

include(../var_list.cmake)
include(../../test_helpers.cmake)

foreach(suffix ${var_suffixes})
    assert_var_defined(KDE_INSTALL_${suffix})
    if(DEFINED ${suffix}_expected_value)
        assert_vars_strequal(KDE_INSTALL_${suffix} ${suffix}_expected_value)
    endif()
    assert_var_defined(KDE_INSTALL_FULL_${suffix})

    if(should_set_CMAKE_INSTALL_${suffix})
        assert_vars_strequal(CMAKE_INSTALL_${suffix} KDE_INSTALL_${suffix})
        assert_vars_strequal(CMAKE_INSTALL_FULL_${suffix} KDE_INSTALL_FULL_${suffix})
    endif()

    if(NOT KDE_INSTALL_DIRS_NO_DEPRECATED AND DEFINED compat_${suffix})
        assert_vars_strequal(${compat_${suffix}} KDE_INSTALL_${suffix})
    endif()
endforeach()

assert_var_defined(KF5_INSTALL_TARGETS_DEFAULT_ARGS)
assert_var_defined(KDE_INSTALL_TARGETS_DEFAULT_ARGS)
if(NOT KDE_INSTALL_DIRS_NO_CMAKE_VARIABLES AND
   NOT KDE_INSTALL_DIRS_NO_DEPRECATED)
    assert_vars_strequal(INSTALL_TARGETS_DEFAULT_ARGS
                         KDE_INSTALL_TARGETS_DEFAULT_ARGS)
endif()

add_executable(dummy main.c)
set_target_properties(dummy PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
