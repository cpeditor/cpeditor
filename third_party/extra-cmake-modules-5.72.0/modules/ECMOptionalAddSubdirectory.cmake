#.rst:
# ECMOptionalAddSubdirectory
# --------------------------
#
# Make subdirectories optional.
#
# ::
#
#   ecm_optional_add_subdirectory(<dir>)
#
# This behaves like add_subdirectory(), except that it does not complain if the
# directory does not exist.  Additionally, if the directory does exist, it
# creates an option to allow the user to skip it. The option will be named
# BUILD_<dir>.
#
# This is useful for "meta-projects" that combine several mostly-independent
# sub-projects.
#
# If the CMake variable DISABLE_ALL_OPTIONAL_SUBDIRECTORIES is set to TRUE for
# the first CMake run on the project, all optional subdirectories will be
# disabled by default (but can of course be enabled via the respective options).
# For example, the following will disable all optional subdirectories except the
# one named "foo":
#
# .. code-block:: sh
#
#   cmake -DDISABLE_ALL_OPTIONAL_SUBDIRECTORIES=TRUE -DBUILD_foo=TRUE myproject
#
# Since pre-1.0.0.

#=============================================================================
# SPDX-FileCopyrightText: 2007 Alexander Neundorf <neundorf@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

function(ECM_OPTIONAL_ADD_SUBDIRECTORY _dir)
  get_filename_component(_fullPath ${_dir} ABSOLUTE)
  if(EXISTS ${_fullPath}/CMakeLists.txt)
    if(DISABLE_ALL_OPTIONAL_SUBDIRECTORIES)
      set(_DEFAULT_OPTION_VALUE FALSE)
    else()
      set(_DEFAULT_OPTION_VALUE TRUE)
    endif()
    if(DISABLE_ALL_OPTIONAL_SUBDIRS  AND NOT DEFINED  BUILD_${_dir})
      set(_DEFAULT_OPTION_VALUE FALSE)
    endif()
    option(BUILD_${_dir} "Build directory ${_dir}" ${_DEFAULT_OPTION_VALUE})
    if(BUILD_${_dir})
      add_subdirectory(${_dir})
    endif()
  endif()
endfunction()
