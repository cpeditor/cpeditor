# - Try to find the ACL library
# Once done this will define
#
#  ACL_FOUND - system has the ACL library
#  ACL_LIBS - The libraries needed to use ACL

# Copyright (c) 2006, Pino Toscano, <toscano.pino@tiscali.it>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(CheckIncludeFiles)

check_include_files(attr/libattr.h HAVE_ATTR_LIBATTR_H)
check_include_files(sys/xattr.h HAVE_SYS_XATTR_H)
check_include_files(sys/acl.h HAVE_SYS_ACL_H)
check_include_files(acl/libacl.h HAVE_ACL_LIBACL_H)

if (HAVE_ATTR_LIBATTR_H AND HAVE_SYS_XATTR_H AND HAVE_SYS_ACL_H AND HAVE_ACL_LIBACL_H)
   set(ACL_HEADERS_FOUND TRUE)
endif (HAVE_ATTR_LIBATTR_H AND HAVE_SYS_XATTR_H AND HAVE_SYS_ACL_H AND HAVE_ACL_LIBACL_H)

if (ACL_HEADERS_FOUND)
   find_library(ACL_LIBS NAMES acl )

   find_library(ATTR_LIBS NAMES attr )
endif (ACL_HEADERS_FOUND)

if (ACL_HEADERS_FOUND AND ACL_LIBS AND ATTR_LIBS)
   set(ACL_FOUND TRUE)
   set(ACL_LIBS ${ACL_LIBS} ${ATTR_LIBS})
   message(STATUS "Found ACL support: ${ACL_LIBS}")
endif (ACL_HEADERS_FOUND AND ACL_LIBS AND ATTR_LIBS)

mark_as_advanced(ACL_LIBS  ATTR_LIBS)

