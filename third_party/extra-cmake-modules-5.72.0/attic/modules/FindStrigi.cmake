# - Try to find Strigi, a fast and small desktop search program (http://strigi.sourceforge.net )
# Once done this will define
#
#  STRIGI_FOUND - system has Strigi
#  STRIGI_INCLUDE_DIR - the Strigi include directory
#  STRIGI_STREAMANALYZER_LIBRARY - Link these to use Strigi streamanalyzer
#  STRIGI_STREAMS_LIBRARY - Link these to use Strigi streams
#  STRIGI_LINE_ANALYZER_PREFIX - strigi plugin prefix
#  STRIGI_THROUGH_ANALYZER_PREFIX - strigi plugin prefix

# Copyright (c) 2008, Jos van den Oever, <jos@vandenoever.info>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if(NOT STRIGI_MIN_VERSION)
    set(STRIGI_MIN_VERSION "0.5.9")
endif()

file(TO_CMAKE_PATH "$ENV{STRIGI_HOME}" strigi_home)

set(_Strigi_FIND_QUIETLY ${Strigi_FIND_QUIETLY})
# Try to find and load the StrigiConfig.cmake installed by kdesupport/strigi
find_package(Strigi QUIET NO_MODULE PATHS "${strigi_home}/lib/strigi" "${strigi_home}/lib64/strigi")
set(Strigi_FIND_QUIETLY ${_Strigi_FIND_QUIETLY})

# If StrigiConfig.cmake (installed by kdesupport/Strigi) has been found
# and it contains all necessary information (since November 16th, 2008), use the information 
# included there, otherwise search it in the same way as any non-cmake project. 
# This variable is set by StrigiConfig.cmake .  Alex
if (STRIGI_CONFIG_FOUND_AND_HAS_COMPLETE_INFORMATION)
   set (_strigiErrorMessage "Couldn't find Strigi streams and streamanalyzer libraries. Set the environment variable STRIGI_HOME (or CMAKE_PREFIX_PATH) to the strigi install dir.")
   set(STRIGI_VERSION_OK TRUE)
   if(STRIGI_VERSION VERSION_LESS ${STRIGI_MIN_VERSION})
      set(_strigiErrorMessage "Strigi version ${STRIGI_VERSION} found, but at least version ${STRIGI_MIN_VERSION} is required")
      set(STRIGI_VERSION_OK FALSE)
   endif()

   include(FindPackageHandleStandardArgs)
   find_package_handle_standard_args(Strigi 
       "${_strigiErrorMessage}"
       STRIGI_STREAMS_LIBRARY  STRIGI_STREAMANALYZER_LIBRARY  STRIGI_INCLUDE_DIR  STRIGI_VERSION_OK)

else()
   # this else() branch is for finding strigi versions before November 16th, 2008. Alex
   include(FindLibraryWithDebug)
   include(MacroPushRequiredVars)

   if (WIN32)
       file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" _program_FILES_DIR)
   endif()

   if (NOT WIN32)
       if(NOT strigi_home)
           find_package(PkgConfig)
           if(PKG_CONFIG_EXECUTABLE)
               pkg_check_modules(STRIGI QUIET libstreamanalyzer>=${STRIGI_MIN_VERSION})
           endif()
       endif()
   endif()

   if (NOT STRIGI_INCLUDEDIR)
       find_path(STRIGI_INCLUDE_DIR strigi/streamanalyzer.h 
            PATHS
            ${strigi_home}/include
            ${STRIGI_INCLUDEDIR}
            ${_program_FILES_DIR}/strigi/include
           )
       set( STRIGI_INCLUDEDIR ${STRIGI_INCLUDE_DIR} )
   else (NOT STRIGI_INCLUDEDIR)
       set( STRIGI_INCLUDE_DIR ${STRIGI_INCLUDEDIR} )
   endif (NOT STRIGI_INCLUDEDIR)


   find_library_with_debug(STRIGI_STREAMANALYZER_LIBRARY
     WIN32_DEBUG_POSTFIX d
     NAMES streamanalyzer
     PATHS
     ${strigi_home}/lib
     ${STRIGI_LIBRARY_DIRS}
     ${_program_FILES_DIR}/strigi/lib
   )

   find_library_with_debug(STRIGI_STREAMS_LIBRARY
     WIN32_DEBUG_POSTFIX d
     NAMES streams
     PATHS
     ${strigi_home}/lib
     ${STRIGI_LIBRARY_DIRS}
     ${_program_FILES_DIR}/strigi/lib
   )

   find_library_with_debug(STRIGI_STRIGIQTDBUSCLIENT_LIBRARY
     WIN32_DEBUG_POSTFIX d
     NAMES strigiqtdbusclient
     PATHS
     ${strigi_home}/lib
     ${STRIGI_LIBRARY_DIRS}
     ${_program_FILES_DIR}/strigi/lib
   )

   include(FindPackageHandleStandardArgs)
   find_package_handle_standard_args(Strigi
       "Couldn't find Strigi streams and streamanalyzer libraries. Set the environment variable STRIGI_HOME (or CMAKE_PREFIX_PATH if using CMake >=2.6) to the strigi install dir."
       STRIGI_STREAMS_LIBRARY  STRIGI_STREAMANALYZER_LIBRARY  STRIGI_INCLUDE_DIR)

   if (STRIGI_FOUND)
       set( STRIGI_NEEDS_SIGNED_CHAR TRUE CACHE BOOL "TRUE if strigi is 0.6.0 or later" )
       set( STRIGI_NEEDS_CHAR FALSE CACHE BOOL "TRUE if strigi is 0.5.9 or before" )
   endif (STRIGI_FOUND)

endif (STRIGI_CONFIG_FOUND_AND_HAS_COMPLETE_INFORMATION)


if(WIN32)
  # this is needed to have mingw, cygwin and msvc libs installed in one directory
  if(MSVC)
    set(STRIGI_LINE_ANALYZER_PREFIX msvc_strigila_)
    set(STRIGI_THROUGH_ANALYZER_PREFIX msvc_strigita_)
  elseif(CYGWIN)
    set(STRIGI_LINE_ANALYZER_PREFIX cyg_strigila_)
    set(STRIGI_THROUGH_ANALYZER_PREFIX cyg_strigita_)
  elseif(MINGW)
    set(STRIGI_LINE_ANALYZER_PREFIX mingw_strigila_)
    set(STRIGI_THROUGH_ANALYZER_PREFIX mingw_strigita_)
  endif()
else()
  set(STRIGI_LINE_ANALYZER_PREFIX strigila_)
  set(STRIGI_THROUGH_ANALYZER_PREFIX strigita_)
endif()

mark_as_advanced(
    STRIGI_INCLUDE_DIR
    STRIGI_STREAMANALYZER_LIBRARY
    STRIGI_STREAMANALYZER_LIBRARY_DEBUG
    STRIGI_STREAMANALYZER_LIBRARY_RELEASE
    STRIGI_STREAMS_LIBRARY
    STRIGI_STREAMS_LIBRARY_DEBUG
    STRIGI_STREAMS_LIBRARY_RELEASE
    STRIGI_STRIGIQTDBUSCLIENT_LIBRARY
    STRIGI_STRIGIQTDBUSCLIENT_LIBRARY_DEBUG
    STRIGI_STRIGIQTDBUSCLIENT_LIBRARY_RELEASE
    STRIGI_LINE_ANALYZER_PREFIX
    STRIGI_THROUGH_ANALYZER_PREFIX
    STRIGI_NEEDS_SIGNED_CHAR
    STRIGI_NEEDS_CHAR
)
