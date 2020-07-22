# Search xmms
# Once done this will define
#
#  XMMS_FOUND        - system has xmms
#  XMMS_INCLUDE_DIRS - the xmms include directory
#  XMMS_LIBRARIES    - Link these to use xmms
#  XMMS_LDFLAGS      - for compatibility only, same as XMMS_LIBRARIES

# Copyright (c) 2006, 2007 Laurent Montel, <montel@kde.org>
# Copyright (c) 2007 Allen Winter <winter@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (XMMS_INCLUDE_DIRS AND XMMS_LIBRARIES)
  # in cache already
  set(XMMS_FOUND TRUE)

else (XMMS_INCLUDE_DIRS AND XMMS_LIBRARIES)
  if (NOT WIN32)
    # use pkg-config to get the directories and then use these values
    # in the FIND_PATH() and FIND_LIBRARY() calls
    find_package(PkgConfig)

    pkg_check_modules(PC_XMMS QUIET xmms)
  endif()

  find_path(XMMS_INCLUDE_DIRS xmmsctrl.h
    PATHS ${PC_XMMS_INCLUDEDIR} ${PC_XMMS_INCLUDE_DIRS} 
    PATH_SUFFIXES xmms)

  find_library(XMMS_LIBRARIES NAMES xmms
    PATHS ${PC_XMMS_LIBDIR} ${PC_XMMS_LIBRARY_DIRS})

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Xmms DEFAULT_MSG
                                    XMMS_LIBRARIES XMMS_INCLUDE_DIRS)

  mark_as_advanced(XMMS_INCLUDE_DIRS XMMS_LIBRARIES)

endif (XMMS_INCLUDE_DIRS AND XMMS_LIBRARIES)

# for compatibility
set(XMMS_LDFLAGS ${XMMS_LIBRARIES})
