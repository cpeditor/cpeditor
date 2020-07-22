# - Find LibLZMA
# Find LibLZMA headers and library
#
#  LIBLZMA_FOUND             - True if liblzma is found.
#  LIBLZMA_INCLUDE_DIRS      - Directory where liblzma headers are located.
#  LIBLZMA_LIBRARIES         - Lzma libraries to link against.
#  LIBLZMA_HAS_AUTO_DECODER  - True if lzma_auto_decoder() is found (required).
#  LIBLZMA_HAS_EASY_ENCODER  - True if lzma_easy_encoder() is found (required).
#  LIBLZMA_HAS_LZMA_PRESET   - True if lzma_lzma_preset() is found (required).


# Copyright (c) 2008, Per Øyvind Karlsen, <peroyvind@mandriva.org>
# Copyright (c) 2009, Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2009, Helio Chissini de Castro, <helio@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


FIND_PATH(LIBLZMA_INCLUDE_DIR lzma.h )
FIND_LIBRARY(LIBLZMA_LIBRARY lzma)

SET(LIBLZMA_LIBRARIES ${LIBLZMA_LIBRARY})
SET(LIBLZMA_INCLUDE_DIRS ${LIBLZMA_INCLUDE_DIR})


# We're using new code known now as XZ, even library still been called LZMA
# it can be found in https://tukaani.org/xz/
# Avoid using old codebase
IF (LIBLZMA_LIBRARIES)
   INCLUDE(CheckLibraryExists)
   CHECK_LIBRARY_EXISTS(${LIBLZMA_LIBRARIES} lzma_auto_decoder "" LIBLZMA_HAS_AUTO_DECODER)
   CHECK_LIBRARY_EXISTS(${LIBLZMA_LIBRARIES} lzma_easy_encoder "" LIBLZMA_HAS_EASY_ENCODER)
   CHECK_LIBRARY_EXISTS(${LIBLZMA_LIBRARIES} lzma_lzma_preset "" LIBLZMA_HAS_LZMA_PRESET)
ENDIF (LIBLZMA_LIBRARIES)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBLZMA  DEFAULT_MSG  LIBLZMA_INCLUDE_DIR 
                                                        LIBLZMA_LIBRARY
                                                        LIBLZMA_HAS_AUTO_DECODER
                                                        LIBLZMA_HAS_EASY_ENCODER
                                                        LIBLZMA_HAS_LZMA_PRESET
                                 )

MARK_AS_ADVANCED( LIBLZMA_INCLUDE_DIR LIBLZMA_LIBRARY )
