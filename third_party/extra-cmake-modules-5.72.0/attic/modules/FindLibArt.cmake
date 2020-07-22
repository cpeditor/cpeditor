# - Try to find the LibArt 2D graphics library
# Once done this will define
#
#  LIBART_FOUND - system has the LibArt
#  LIBART_INCLUDE_DIR - the LibArt include directory
#  LIBART_LIBRARIES - The libraries needed to use LibArt

# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if (LIBART_INCLUDE_DIR AND LIBART_LIBRARIES)

  # in cache already
  SET(LIBART_FOUND TRUE)

else (LIBART_INCLUDE_DIR AND LIBART_LIBRARIES)

  IF (NOT WIN32)
    find_package(PkgConfig)

    # use pkg-config to get the directories and then use these values
    # in the FIND_PATH() and FIND_LIBRARY() calls
    pkg_check_modules(PC_LIBART QUIET libart-2.0)

    ######### ?? where is this used ?? ###############
    set(LIBART_DEFINITIONS ${PC_LIBART_CFLAGS_OTHER})
  ENDIF (NOT WIN32)

  FIND_PATH(LIBART_INCLUDE_DIR NAMES libart_lgpl/libart.h
     PATHS
     ${PC_LIBART_INCLUDEDIR}
     ${PC_LIBART_INCLUDE_DIRS}
     PATH_SUFFIXES libart-2.0
  )
  
  FIND_LIBRARY(LIBART_LIBRARIES NAMES art_lgpl_2
     PATHS
     ${PC_LIBART_LIBDIR}
     ${PC_LIBART_LIBRARY_DIRS}
  )
  
  
  if (LIBART_INCLUDE_DIR AND LIBART_LIBRARIES)
     set(LIBART_FOUND TRUE)
  endif (LIBART_INCLUDE_DIR AND LIBART_LIBRARIES)
  
  
  if (LIBART_FOUND)
     if (NOT LibArt_FIND_QUIETLY)
        message(STATUS "Found libart: ${LIBART_LIBRARIES}")
     endif (NOT LibArt_FIND_QUIETLY)
  else (LIBART_FOUND)
     if (LibArt_FIND_REQUIRED)
        message(FATAL_ERROR "Could NOT find libart")
     endif (LibArt_FIND_REQUIRED)
  endif (LIBART_FOUND)

  MARK_AS_ADVANCED(LIBART_INCLUDE_DIR LIBART_LIBRARIES)

endif (LIBART_INCLUDE_DIR AND LIBART_LIBRARIES)
