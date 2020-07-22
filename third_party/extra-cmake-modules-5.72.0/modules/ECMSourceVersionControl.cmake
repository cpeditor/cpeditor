#.rst:
# ECMSourceVersionControl
# --------------------------
#
# Tries to determine whether the source is under version control (git clone,
# svn checkout, etc).
#
# ``ECM_SOURCE_UNDER_VERSION_CONTROL`` is set when indication is found that
# CMAKE_SOURCE_DIR is under version control.
#
# Since 5.63

#=============================================================================
# SPDX-FileCopyrightText: 2019 Harald Sitter <sitter@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

if(EXISTS "${CMAKE_SOURCE_DIR}/.git" OR
   EXISTS "${CMAKE_SOURCE_DIR}/.svn" OR
   EXISTS "${CMAKE_SOURCE_DIR}/.hg" OR
   EXISTS "${CMAKE_SOURCE_DIR}/.bzr")
    set(ECM_SOURCE_UNDER_VERSION_CONTROL TRUE)
else()
    set(ECM_SOURCE_UNDER_VERSION_CONTROL FALSE)
endif()
