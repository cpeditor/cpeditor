# - Try to find BlueZ bluetooth library.
# Once done this will define
#
#  BLUEZ_FOUND - system has BlueZ
#  BLUEZ_INCLUDE_DIR - the BlueZ include directory
#  BLUEZ_LIBRARIES - Link these to use BlueZ
#  BLUEZ_DEFINITIONS - Compiler switches required for using BlueZ
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

# Copyright (c) 2008, Daniel Gollub, <dgollub@suse.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
find_package(PkgConfig)
pkg_check_modules(PC_BLUEZ QUIET bluez)

set(BLUEZ_DEFINITIONS ${PC_BLUEZ_CFLAGS_OTHER})

find_path(BLUEZ_INCLUDE_DIR NAMES bluetooth/bluetooth.h
  PATHS
  ${PC_BLUEZ_INCLUDEDIR}
  ${PC_BLUEZ_INCLUDE_DIRS}
  /usr/X11/include
)

find_library(BLUEZ_LIBRARY NAMES bluetooth
  PATHS
  ${PC_BLUEZ_LIBDIR}
  ${PC_BLUEZ_LIBRARY_DIRS}
)

set(BLUEZ_LIBRARIES ${BLUEZ_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BlueZ DEFAULT_MSG BLUEZ_LIBRARY BLUEZ_INCLUDE_DIR)

include(FeatureSummary)
set_package_properties(BlueZ PROPERTIES URL http://www.bluez.org/
  DESCRIPTION "Official Linux Bluetooth protocol stack")

# show the BLUEZ_INCLUDE_DIR and BLUEZ_LIBRARY variables only in the advanced view
mark_as_advanced(BLUEZ_INCLUDE_DIR BLUEZ_LIBRARY)
