# - Try to find the KDEWIN32 library
# - Try to find the KDEWIN32 library - deprecated 
# 
# Once done this will define
#
#  KDEWIN32_FOUND - system has KDEWIN32
#  KDEWIN32_INCLUDES - the KDEWIN32 include directories
#  KDEWIN32_LIBRARIES - The libraries needed to use KDEWIN32

# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2007-2009, Ralf Habacker, <ralf.habacker@freenet.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if (WIN32)
  message(STATUS "The kdewin32 cmake module is deprecated, use kdewin instead")
  find_package(KDEWIN)

  if (KDEWIN_FOUND)
    set(KDEWIN32_FOUND ${KDEWIN_FOUND})
    set(KDEWIN32_INCLUDES ${KDEWIN_INCLUDES})
    set(KDEWIN32_LIBRARIES ${KDEWIN_LIBRARIES})
  endif (KDEWIN_FOUND)
endif (WIN32)
