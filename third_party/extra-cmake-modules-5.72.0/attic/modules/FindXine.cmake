# - Try to find the XINE  library
# Once done this will define
#
#  XINE_FOUND - system has the XINE library
#  XINE_VERSION - XINE version
#  XINE_BUGFIX_VERSION - the XINE bugfix version
#  XINE_INCLUDE_DIR - the XINE include directory
#  XINE_LIBRARY - The libraries needed to use XINE
#  XINE_XCB_FOUND - libxine can use XCB for video output

# Copyright (c) 2006,2007 Laurent Montel, <montel@kde.org>
# Copyright (c) 2006, Matthias Kretz, <kretz@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (XINE_INCLUDE_DIR AND XINE_LIBRARY)
  # Already in cache, be silent
  set(Xine_FIND_QUIETLY TRUE)
endif (XINE_INCLUDE_DIR AND XINE_LIBRARY)

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
   pkg_check_modules(PC_LIBXINE QUIET libxine)
endif (PKG_CONFIG_FOUND)

find_path(XINE_INCLUDE_DIR NAMES xine.h 
    HINTS                    # HINTS is new in cmake 2.6. These dirs will be preferred over the default ones
    ${PC_LIBXINE_INCLUDEDIR} 
    ${PC_LIBXINE_INCLUDE_DIRS} 
    )

find_library(XINE_LIBRARY NAMES xine
 HINTS
  ${PC_LIBXINE_LIBDIR}
  ${PC_LIBXINE_LIBRARY_DIRS}
 )

find_program(XINECONFIG_EXECUTABLE NAMES xine-config 
   HINTS
   ${PC_LIBXINE_PREFIX}/bin
)

if (XINE_INCLUDE_DIR AND XINE_LIBRARY AND XINECONFIG_EXECUTABLE)
   exec_program(${XINECONFIG_EXECUTABLE} ARGS --version RETURN_VALUE _return_VALUE OUTPUT_VARIABLE XINE_VERSION)
   if("${XINE_VERSION}" VERSION_GREATER "1.1.0")   #if (... VERSION_GREATER) is new since cmake 2.6.2
      set(XINE_VERSION_OK TRUE)
      string(REGEX REPLACE "[0-9]\\.[0-9]\\." "" XINE_BUGFIX_VERSION ${XINE_VERSION})
   endif()
endif (XINE_INCLUDE_DIR AND XINE_LIBRARY AND XINECONFIG_EXECUTABLE)


if( XINE_VERSION_OK)
  include(CheckCSourceCompiles)
  set(CMAKE_REQUIRED_INCLUDES ${XINE_INCLUDE_DIR})
  set(CMAKE_REQUIRED_LIBRARIES ${XINE_LIBRARY})
  check_c_source_compiles("#include <xine.h>\nint main()\n{\n  xine_open_video_driver(xine_new(), \"auto\", XINE_VISUAL_TYPE_XCB, NULL);\n  return 0;\n}\n" XINE_XCB_FOUND)
  set(XINE_FOUND TRUE)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Xine  "Could NOT find XINE 1.1.1 or greater"  XINE_INCLUDE_DIR XINE_LIBRARY XINECONFIG_EXECUTABLE  XINE_VERSION_OK)

mark_as_advanced(XINE_INCLUDE_DIR XINE_LIBRARY XINECONFIG_EXECUTABLE)
