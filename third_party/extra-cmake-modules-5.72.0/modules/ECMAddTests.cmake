#.rst:
# ECMAddTests
# -----------
#
# Convenience functions for adding tests.
#
# ::
#
#   ecm_add_tests(<sources> LINK_LIBRARIES <library> [<library> [...]]
#                           [NAME_PREFIX <prefix>]
#                           [GUI]
#                           [TARGET_NAMES_VAR <target_names_var>]
#                           [TEST_NAMES_VAR <test_names_var>])
#
# A convenience function for adding multiple tests, each consisting of a
# single source file. For each file in <sources>, an executable target will be
# created (the name of which will be the basename of the source file). This
# will be linked against the libraries given with LINK_LIBRARIES. Each
# executable will be added as a test with the same name.
#
# If NAME_PREFIX is given, this prefix will be prepended to the test names, but
# not the target names. As a result, it will not prevent clashes between tests
# with the same name in different parts of the project, but it can be used to
# give an indication of where to look for a failing test.
#
# If the flag GUI is passed the test binaries will be GUI executables, otherwise
# the resulting binaries will be console applications (regardless of the value
# of CMAKE_WIN32_EXECUTABLE or CMAKE_MACOSX_BUNDLE). Be aware that this changes
# the executable entry point on Windows (although some frameworks, such as Qt,
# abstract this difference away).
#
# The TARGET_NAMES_VAR and TEST_NAMES_VAR arguments, if given, should specify a
# variable name to receive the list of generated target and test names,
# respectively. This makes it convenient to apply properties to them as a
# whole, for example, using set_target_properties() or  set_tests_properties().
#
# The generated target executables will have the effects of ecm_mark_as_test()
# (from the :module:`ECMMarkAsTest` module) applied to it.
#
# ::
#
#   ecm_add_test(<sources> LINK_LIBRARIES <library> [<library> [...]]
#                          [TEST_NAME <name>]
#                          [NAME_PREFIX <prefix>]
#                          [GUI])
#
# This is a single-test form of ecm_add_tests that allows multiple source files
# to be used for a single test. If using multiple source files, TEST_NAME must
# be given; this will be used for both the target and test names (and, as with
# ecm_add_tests(), the NAME_PREFIX argument will be prepended to the test name).
#
#
# Since pre-1.0.0.

#=============================================================================
# SPDX-FileCopyrightText: 2013 Alexander Richardson <arichardson.kde@gmail.com>
# SPDX-FileCopyrightText: 2015 Alex Merry <alex.merry@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

include(CMakeParseArguments)
include(ECMMarkAsTest)
include(ECMMarkNonGuiExecutable)

function(ecm_add_test)
  set(options GUI)
  # TARGET_NAME_VAR and TEST_NAME_VAR are undocumented args used by
  # ecm_add_tests
  set(oneValueArgs TEST_NAME NAME_PREFIX TARGET_NAME_VAR TEST_NAME_VAR)
  set(multiValueArgs LINK_LIBRARIES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  set(_sources ${ARG_UNPARSED_ARGUMENTS})
  list(LENGTH _sources _sourceCount)
  if(ARG_TEST_NAME)
    set(_targetname ${ARG_TEST_NAME})
  elseif(${_sourceCount} EQUAL "1")
    #use the source file name without extension as the testname
    get_filename_component(_targetname ${_sources} NAME_WE)
  else()
    #more than one source file passed, but no test name given -> error
    message(FATAL_ERROR "ecm_add_test() called with multiple source files but without setting \"TEST_NAME\"")
  endif()

  set(_testname ${ARG_NAME_PREFIX}${_targetname})
  set(gui_args)
  if(ARG_GUI)
      set(gui_args WIN32 MACOSX_BUNDLE)
  endif()
  add_executable(${_targetname} ${gui_args} ${_sources})
  if(NOT ARG_GUI)
    ecm_mark_nongui_executable(${_targetname})
  endif()
  add_test(NAME ${_testname} COMMAND ${_targetname})
  target_link_libraries(${_targetname} ${ARG_LINK_LIBRARIES})
  ecm_mark_as_test(${_targetname})
  if (CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    if(CMAKE_HOST_SYSTEM MATCHES "Windows")
      set(PATHSEP ";")
    else() # e.g. Linux
      set(PATHSEP ":")
    endif()
    set(_plugin_path $ENV{QT_PLUGIN_PATH})
    set_property(TEST ${_testname} PROPERTY ENVIRONMENT QT_PLUGIN_PATH=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}${PATHSEP}${_plugin_path})
  endif()
  if (ARG_TARGET_NAME_VAR)
    set(${ARG_TARGET_NAME_VAR} "${_targetname}" PARENT_SCOPE)
  endif()
  if (ARG_TEST_NAME_VAR)
    set(${ARG_TEST_NAME_VAR} "${_testname}" PARENT_SCOPE)
  endif()
endfunction()

function(ecm_add_tests)
  set(options GUI)
  set(oneValueArgs NAME_PREFIX TARGET_NAMES_VAR TEST_NAMES_VAR)
  set(multiValueArgs LINK_LIBRARIES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(ARG_GUI)
    set(_exe_type GUI)
  else()
    set(_exe_type "")
  endif()
  set(test_names)
  set(target_names)
  foreach(_test_source ${ARG_UNPARSED_ARGUMENTS})
    ecm_add_test(${_test_source}
      NAME_PREFIX ${ARG_NAME_PREFIX}
      LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
      TARGET_NAME_VAR target_name
      TEST_NAME_VAR test_name
      ${_exe_type}
    )
    list(APPEND _test_names "${test_name}")
    list(APPEND _target_names "${target_name}")
  endforeach()
  if (ARG_TARGET_NAMES_VAR)
    set(${ARG_TARGET_NAMES_VAR} "${_target_names}" PARENT_SCOPE)
  endif()
  if (ARG_TEST_NAMES_VAR)
    set(${ARG_TEST_NAMES_VAR} "${_test_names}" PARENT_SCOPE)
  endif()
endfunction()
