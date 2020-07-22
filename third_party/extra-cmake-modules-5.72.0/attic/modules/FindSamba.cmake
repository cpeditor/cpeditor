# - Try to find the samba directory library
# Once done this will define
#
#  SAMBA_FOUND - system has SAMBA
#  SAMBA_INCLUDE_DIR - the SAMBA include directory
#  SAMBA_LIBRARIES - The libraries needed to use SAMBA
#  Set SAMBA_REQUIRE_SMBC_SET_CONTEXT to TRUE if you need a version of Samba
#  which comes with smbc_set_context()

# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
    # Already in cache, be silent
    set(Samba_FIND_QUIETLY TRUE)
endif()

find_path(SAMBA_INCLUDE_DIR NAMES libsmbclient.h )

find_library(SAMBA_LIBRARIES NAMES smbclient )


if(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
   set(SAMBA_FOUND TRUE)
   # check whether libsmbclient has smbc_set_context()
   include(CheckSymbolExists)
   include(MacroPushRequiredVars)
   macro_push_required_vars()
   set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${SAMBA_LIBRARIES})
   set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${SAMBA_INCLUDE_DIR})
   check_symbol_exists(smbc_set_context "libsmbclient.h" SAMBA_HAVE_SMBC_SET_CONTEXT)
   check_symbol_exists(smbc_option_set "libsmbclient.h" SAMBA_HAVE_SMBC_OPTION_SET)
   macro_pop_required_vars()
   # fail if smbc_set_context() was required but hasn't been found
   if (SAMBA_REQUIRE_SMBC_SET_CONTEXT AND NOT SAMBA_HAVE_SMBC_SET_CONTEXT)
      set(SAMBA_FOUND FALSE)
   endif (SAMBA_REQUIRE_SMBC_SET_CONTEXT AND NOT SAMBA_HAVE_SMBC_SET_CONTEXT)
   # fail if smbc_option_set() was required but hasn't been found
   if (SAMBA_REQUIRE_SMBC_OPTION_SET AND NOT SAMBA_HAVE_SMBC_OPTION_SET)
      set(SAMBA_FOUND FALSE)
   endif (SAMBA_REQUIRE_SMBC_OPTION_SET AND NOT SAMBA_HAVE_SMBC_OPTION_SET)
   
else()
   set(SAMBA_FOUND FALSE)
   set(SAMBA_HAVE_SMBC_SET_CONTEXT FALSE)
endif()


if(SAMBA_FOUND)
   if(NOT Samba_FIND_QUIETLY)
      message(STATUS "Found samba: ${SAMBA_LIBRARIES}")
   endif()
else()
   if (Samba_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Samba library")
   endif (Samba_FIND_REQUIRED)
endif()

mark_as_advanced(SAMBA_INCLUDE_DIR SAMBA_LIBRARIES)

