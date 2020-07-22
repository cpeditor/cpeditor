# Try to find Wcecompat functionality
# Once done this will define
#
#  WCECOMPAT_FOUND - system has Wcecompat
#  WCECOMPAT_INCLUDE_DIR - Wcecompat include directory
#  WCECOMPAT_LIBRARIES - Libraries needed to use Wcecompat
#
# Copyright (c) 2010, Andreas Holzammer, <andy@kdab.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

find_path(WCECOMPAT_INCLUDE_DIR errno.h PATH_SUFFIXES wcecompat)

set(WCECOMPAT_LIB_FOUND FALSE)

if(WCECOMPAT_INCLUDE_DIR)
    find_library(WCECOMPAT_LIBRARIES NAMES wcecompat wcecompatex )

    if(WCECOMPAT_LIBRARIES)
      set(WCECOMPAT_LIB_FOUND TRUE)
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Wcecompat  DEFAULT_MSG  WCECOMPAT_LIBRARIES  WCECOMPAT_LIB_FOUND)
