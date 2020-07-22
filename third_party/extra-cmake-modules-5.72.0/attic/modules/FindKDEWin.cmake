# - Try to find the KDEWIN library
# 
# Once done this will define
#
#  KDEWIN_FOUND - system has KDEWIN
#  KDEWIN_INCLUDES - the KDEWIN include directories
#  KDEWIN_LIBRARIES - The libraries needed to use KDEWIN

# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2007-2009, Ralf Habacker, <ralf.habacker@freenet.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if (WIN32)

  if(WINCE)
    FIND_PACKAGE(WCECOMPAT REQUIRED)
  endif()
    
  if (NOT KDEWIN_LIBRARY)
 

    find_path(KDEWIN_INCLUDE_DIR kdewin_export.h
      ${KDE4_INCLUDE_DIR}
      ${CMAKE_INCLUDE_PATH}
      ${CMAKE_INSTALL_PREFIX}/include
    )
 
    # search for kdewin in the default install directory for applications (default of (n)make install)
    FILE(TO_CMAKE_PATH "${CMAKE_LIBRARY_PATH}" _cmakeLibraryPathCmakeStyle)

    string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_TOLOWER)
    if (CMAKE_BUILD_TYPE_TOLOWER MATCHES debug)
        set (LIBRARY_NAME kdewind)
    else()
        set (LIBRARY_NAME kdewin)
    endif (CMAKE_BUILD_TYPE_TOLOWER MATCHES debug)

    find_library(KDEWIN_LIBRARY
      NAMES ${LIBRARY_NAME}
      PATHS 
        ${KDE4_LIB_DIR}
        ${_cmakeLibraryPathCmakeStyle}
        ${CMAKE_INSTALL_PREFIX}/lib
      NO_SYSTEM_ENVIRONMENT_PATH
    )
  endif (NOT KDEWIN_LIBRARY)

  if (KDEWIN_LIBRARY AND KDEWIN_INCLUDE_DIR)
    set(KDEWIN_FOUND TRUE)
    # add needed system libs
    if(NOT WINCE)
        set(KDEWIN_LIBRARIES ${KDEWIN_LIBRARY} user32 shell32 ws2_32 netapi32 userenv)
    else()
         set(KDEWIN_LIBRARIES ${KDEWIN_LIBRARY} ws2 ${WCECOMPAT_LIBRARIES})
    endif()

    if (MINGW)
      #mingw compiler
      set(KDEWIN_INCLUDES ${KDEWIN_INCLUDE_DIR} ${KDEWIN_INCLUDE_DIR}/mingw ${QT_INCLUDES})
    else (MINGW)
      # msvc compiler
      # add the MS SDK include directory if available
      file(TO_CMAKE_PATH "$ENV{MSSDK}" MSSDK_DIR)
      if (WINCE)
        set(KDEWIN_INCLUDES ${KDEWIN_INCLUDE_DIR} ${KDEWIN_INCLUDE_DIR}/msvc ${WCECOMPAT_INCLUDE_DIR} ${QT_INCLUDES} ${MSSDK_DIR})
      else()
        set(KDEWIN_INCLUDES ${KDEWIN_INCLUDE_DIR} ${KDEWIN_INCLUDE_DIR}/msvc ${QT_INCLUDES} ${MSSDK_DIR})
      endif()
    endif (MINGW)

  endif (KDEWIN_LIBRARY AND KDEWIN_INCLUDE_DIR)
  # required for configure
  set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${KDEWIN_INCLUDES})
  set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${KDEWIN_LIBRARIES})      

  if (KDEWIN_FOUND)
    if (NOT KDEWin_FIND_QUIETLY)
      message(STATUS "Found KDEWin library: ${KDEWIN_LIBRARY}")
    endif (NOT KDEWin_FIND_QUIETLY)

  else (KDEWIN_FOUND)
    if (KDEWin_FIND_REQUIRED)
      message(FATAL_ERROR "Could NOT find KDEWin library\nPlease install it first")
    endif (KDEWin_FIND_REQUIRED)
  endif (KDEWIN_FOUND)
endif (WIN32)
