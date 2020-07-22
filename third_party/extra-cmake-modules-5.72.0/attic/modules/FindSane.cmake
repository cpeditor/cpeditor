# cmake macro to test if we use sane
#
#  SANE_FOUND - system has SANE libs
#  SANE_INCLUDE_DIR - the SANE include directory
#  SANE_LIBRARIES - The libraries needed to use SANE

# Copyright (c) 2006, Marcus Hufgard <hufgardm@hufgard.de> 2006
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH(SANE_INCLUDE_DIR sane/sane.h)

FIND_LIBRARY(SANE_LIBRARY NAMES  sane libsane
   PATH_SUFFIXES sane
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Sane  DEFAULT_MSG  SANE_LIBRARY SANE_INCLUDE_DIR )

MARK_AS_ADVANCED(SANE_INCLUDE_DIR SANE_LIBRARY)
