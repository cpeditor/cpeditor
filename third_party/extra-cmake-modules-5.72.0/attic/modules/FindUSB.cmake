# - Try to find the freetype library
# Once done this defines
#
#  LIBUSB_FOUND - system has libusb
#  LIBUSB_INCLUDE_DIR - the libusb include directory
#  LIBUSB_LIBRARIES - Link these to use libusb

# Copyright (c) 2006, 2008  Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if (NOT WIN32)
  # use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  find_package(PkgConfig)
  pkg_check_modules(PC_LIBUSB QUIET libusb)
endif()

find_path(LIBUSB_INCLUDE_DIR usb.h
  HINTS ${PC_LIBUSB_INCLUDEDIR} ${PC_LIBUSB_INCLUDE_DIRS})

find_library(LIBUSB_LIBRARIES NAMES usb
  HINTS ${PC_LIBUSB_LIBDIR} ${PC_LIBUSB_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBUSB  DEFAULT_MSG  LIBUSB_LIBRARIES LIBUSB_INCLUDE_DIR)

mark_as_advanced(LIBUSB_INCLUDE_DIR LIBUSB_LIBRARIES)
