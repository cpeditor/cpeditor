# - Try to find the FAM directory notification library
# Once done this will define
#
#  FAM_FOUND - system has FAM
#  FAM_INCLUDE_DIR - the FAM include directory
#  FAM_LIBRARIES - The libraries needed to use FAM

# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


FIND_PATH(FAM_INCLUDE_DIR fam.h)

FIND_LIBRARY(FAM_LIBRARIES NAMES fam )

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FAM DEFAULT_MSG FAM_INCLUDE_DIR FAM_LIBRARIES )

MARK_AS_ADVANCED(FAM_INCLUDE_DIR FAM_LIBRARIES)

