# - Try to find NetworkManager
# Once done this will define
#
#  NETWORKMANAGER_FOUND - system has NetworkManager
#  NETWORKMANAGER_INCLUDE_DIRS - the NetworkManager include directories
#  NETWORKMANAGER_LIBRARIES - the libraries needed to use NetworkManager
#  NETWORKMANAGER_CFLAGS - Compiler switches required for using NetworkManager
#  NETWORKMANAGER_VERSION - version number of NetworkManager

# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2007, Will Stephenson, <wstephenson@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


IF (NETWORKMANAGER_INCLUDE_DIRS AND NM-UTIL_INCLUDE_DIRS)
   # in cache already
   SET(NetworkManager_FIND_QUIETLY TRUE)
ENDIF (NETWORKMANAGER_INCLUDE_DIRS AND NM-UTIL_INCLUDE_DIRS)

IF (NOT WIN32)
   # use pkg-config to get the directories and then use these values
   # in the FIND_PATH() and FIND_LIBRARY() calls
   find_package(PkgConfig)
   PKG_SEARCH_MODULE( NETWORKMANAGER NetworkManager )
   PKG_SEARCH_MODULE( NM-UTIL libnm-util )
ENDIF (NOT WIN32)

IF (NETWORKMANAGER_FOUND AND NM-UTIL_FOUND)
   IF (NOT NetworkManager_FIND_QUIETLY)
      MESSAGE(STATUS "Found NetworkManager ${NETWORKMANAGER_VERSION}: ${NETWORKMANAGER_LIBRARY_DIRS}")
      MESSAGE(STATUS "Found libnm-util: ${NM-UTIL_LIBRARY_DIRS}")
   ENDIF (NOT NetworkManager_FIND_QUIETLY)
ELSE (NETWORKMANAGER_FOUND AND NM-UTIL_FOUND)
   IF (NetworkManager_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could NOT find NetworkManager or libnm-util, check FindPkgConfig output above!")
   ENDIF (NetworkManager_FIND_REQUIRED)
ENDIF (NETWORKMANAGER_FOUND AND NM-UTIL_FOUND)

MARK_AS_ADVANCED(NETWORKMANAGER_INCLUDE_DIRS NM-UTIL_INCLUDE_DIRS)

