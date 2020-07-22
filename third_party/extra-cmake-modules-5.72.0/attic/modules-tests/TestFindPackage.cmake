# This module provides the macro TEST_FIND_PACKAGE()
#
# TEST_FIND_PACKAGE(package prefix var1 .. varN )
#    It can be used to test a CMake Find-module.
#    It executes FIND_PACKAGE(<package>) and then prints some results.
#    <package> is the same as for FIND_PACKAGE() and
#    prefix should be the prefix which is used in that module for
#    all variables.
#    It checks <prefix>_FOUND to decide whether the module was successful or not.
#
# Example:
#   test_find_package(Xine  XINE  VERSION INCLUDE_DIR LIBRARY)
#
# This will check XINE_FOUND and then print the variables XINE_VERSION, XINE_INCLUDE_DIR and XINE_LIBRARY.

cmake_minimum_required(VERSION 2.6.2)
cmake_policy(SET CMP0000 OLD)

macro(TEST_FIND_PACKAGE package prefix )
   # if PKG_CONFIG_EXECUTABLE is set to "echo", FindPkgConfig.cmake
   # will not search again for the real pkg-config, as it would if it was set to NOTFOUND
   # and still the execute_process(${PKG_CONFIG_EXECUTABLE} ) calls will work
   # but never return success.
   if("${PKG_CONFIG_EXECUTABLE}" STREQUAL "echo")
      message(STATUS ":: ***** Testing Find${package}.cmake, with pkg-config disabled *****")
   else()
      message(STATUS ":: ***** Testing Find${package}.cmake *****")
   endif()

   find_package(${package})
   message(STATUS ":: ***** Results from Find${package}.cmake *****")

   if ( ${prefix}_FOUND)
      message(STATUS ":: Find${package}.cmake: ${package} has been found")
   else ( ${prefix}_FOUND)
      message(STATUS ":: Find${package}.cmake: ${package} has NOT been found !")
   endif ( ${prefix}_FOUND)

   message(STATUS ":: ${prefix}_FOUND: \"${${prefix}_FOUND}\"")
   foreach(var ${ARGN})
      message(STATUS ":: ${prefix}_${var}: \"${${prefix}_${var}}\"")
   endforeach(var)
   message(STATUS "::")
endmacro(TEST_FIND_PACKAGE package)
