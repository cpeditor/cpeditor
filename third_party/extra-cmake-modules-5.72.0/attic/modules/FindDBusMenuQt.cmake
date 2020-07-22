# - Try to find dbusmenu-qt
# This module helps finding an installation of the DBusMenuQt library (see https://launchpad.net/libdbusmenu-qt/)
# Once done this will define
#
#  DBUSMENUQT_FOUND - system has dbusmenu-qt
#  DBUSMENUQT_INCLUDE_DIR - the dbusmenu-qt include directory
#  DBUSMENUQT_LIBRARIES - the libraries needed to use dbusmenu-qt
#  DBUSMENUQT_DEFINITIONS - Compiler switches required for using dbusmenu-qt
#
# The minimum required version of DBusMenuQt can be specified using the
# standard syntax, e.g. find_package(DBusMenuQt 0.6)
#
# WARNING: versions below 0.4.0 cannot be checked for.
# So if you want to have a version check, require at least 0.4.0 of dbusmenuqt.

# Copyright (c) 2009, Canonical Ltd.
# - Author: Aurélien Gâteau <aurelien.gateau@canonical.com>
#
# Based on FindQCA2.cmake
# Copyright (c) 2006, Michael Larouche, <michael.larouche@kdemail.net>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(FindPackageHandleStandardArgs)

find_package(PkgConfig)
pkg_check_modules(PC_DBUSMENUQT QUIET dbusmenu-qt)


set(DBUSMENUQT_DEFINITIONS ${PC_DBUSMENUQT_CFLAGS_OTHER})

find_library(DBUSMENUQT_LIBRARIES
    NAMES dbusmenu-qt dbusmenu-qtd
    HINTS ${PC_DBUSMENUQT_LIBDIR} ${PC_DBUSMENUQT_LIBRARY_DIRS}
    )

find_path(DBUSMENUQT_INCLUDE_DIR dbusmenuexporter.h
    HINTS ${PC_DBUSMENUQT_INCLUDEDIR} ${PC_DBUSMENUQT_INCLUDE_DIRS}
    PATH_SUFFIXES dbusmenu-qt
    )


# dbusmenu_version.h is installed since 0.4.0, fail if a version below this is required:
if ((DBusMenuQt_FIND_VERSION)  AND ("${DBusMenuQt_FIND_VERSION}" VERSION_LESS "0.4.0"))
  message(FATAL_ERROR "Cannot check reliably for a DBusMenuQt version below 0.4.0 (${DBusMenuQt_FIND_VERSION} was requested)")
endif ((DBusMenuQt_FIND_VERSION)  AND ("${DBusMenuQt_FIND_VERSION}" VERSION_LESS "0.4.0"))


# find the version number from dbusmenu_version.h and store it in the cache
if(DBUSMENUQT_INCLUDE_DIR  AND NOT DBUSMENUQT_VERSION)
  # parse the version number out from dbusmenu_version:
  if(EXISTS ${DBUSMENUQT_INCLUDE_DIR}/dbusmenu_version.h)
    file(READ "${DBUSMENUQT_INCLUDE_DIR}/dbusmenu_version.h" DBUSMENUQT_VERSION_CONTENT)

    if ("${DBUSMENUQT_VERSION_CONTENT}" MATCHES "DBUSMENUQT_VERSION_MAJOR") # introduced after 0.6.4, makes this code here more robust

     string(REGEX MATCH "#define +DBUSMENUQT_VERSION_MAJOR +([0-9]+)"  _dummy "${DBUSMENUQT_VERSION_CONTENT}")
     set(DBUSMENUQT_VERSION_MAJOR "${CMAKE_MATCH_1}")

      string(REGEX MATCH "#define +DBUSMENUQT_VERSION_MINOR +([0-9]+)"  _dummy "${DBUSMENUQT_VERSION_CONTENT}")
      set(DBUSMENUQT_VERSION_MINOR "${CMAKE_MATCH_1}")

      string(REGEX MATCH "#define +DBUSMENUQT_VERSION_PATCH +([0-9]+)"  _dummy "${DBUSMENUQT_VERSION_CONTENT}")
      set(DBUSMENUQT_VERSION_PATCH "${CMAKE_MATCH_1}")

    else()
      # In versions up to 0.6.4, the code for setting the version number in the header looked like
      # shopw below. This made version checking quite un-obvious:
      # #define DBUSMENUQT_VERSION \
      #     ((0 << 16) \
      #     |(6 << 8) \
      #     |4)

      string(REGEX MATCH "\\(\\( *([0-9]+) *<<"  _dummy "${DBUSMENUQT_VERSION_CONTENT}")
      set(DBUSMENUQT_VERSION_MAJOR "${CMAKE_MATCH_1}")

      string(REGEX MATCH "\\|\\( *([0-9]+) *<<"  _dummy "${DBUSMENUQT_VERSION_CONTENT}")
      set(DBUSMENUQT_VERSION_MINOR "${CMAKE_MATCH_1}")

      string(REGEX MATCH "\\| *([0-9]+) *\\)"  _dummy "${DBUSMENUQT_VERSION_CONTENT}")
      set(DBUSMENUQT_VERSION_PATCH "${CMAKE_MATCH_1}")
    endif()
  endif()

  set(DBUSMENUQT_VERSION "${DBUSMENUQT_VERSION_MAJOR}.${DBUSMENUQT_VERSION_MINOR}.${DBUSMENUQT_VERSION_PATCH}" CACHE STRING "Version number of DBusMenuQt" FORCE)
endif()


find_package_handle_standard_args(DBusMenuQt REQUIRED_VARS DBUSMENUQT_LIBRARIES DBUSMENUQT_INCLUDE_DIR
                                             VERSION_VAR DBUSMENUQT_VERSION)
#"Could not find dbusmenu-qt; available at https://launchpad.net/libdbusmenu-qt/" DBUSMENUQT_LIBRARIES DBUSMENUQT_INCLUDE_DIR)

mark_as_advanced(DBUSMENUQT_INCLUDE_DIR DBUSMENUQT_LIBRARIES DBUSMENUQT_VERSION)
