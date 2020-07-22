#.rst:
# FindLibGit2
# -----------
#
# Try to find libgit2 on a Unix system.
#
# This will define the following variables:
#
# ``LIBGIT2_FOUND``
#     True if (the requested version of) libgit2 is available
# ``LIBGIT2_VERSION``
#     The version of libgit2
# ``LIBGIT2_LIBRARIES``
#     This can be passed to target_link_libraries() instead of the ``LibGit2::LibGit2``
#     target
# ``LIBGIT2_INCLUDE_DIRS``
#     This should be passed to target_include_directories() if the target is not
#     used for linking
# ``LIBGIT2_DEFINITIONS``
#     This should be passed to target_compile_options() if the target is not
#     used for linking
#
# If ``LIBGIT2_FOUND`` is TRUE, it will also define the following imported target:
#
# ``LibGit2::LibGit2``
#     The libgit2 library
#
# In general we recommend using the imported target, as it is easier to use.
# Bear in mind, however, that if the target is in the link interface of an
# exported library, it must be made available by the package config file.
#
# Since 1.3.0.

#=============================================================================
# SPDX-FileCopyrightText: 2014 Alex Merry <alex.merry@kde.org>
# SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
# SPDX-FileCopyrightText: 2014 Christoph Cullmann <cullmann@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

include(${CMAKE_CURRENT_LIST_DIR}/ECMFindModuleHelpersStub.cmake)

ecm_find_package_version_check(LibGit2)

# Use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
find_package(PkgConfig)
pkg_check_modules(PKG_GIT2 QUIET git2)

set(LIBGIT2_DEFINITIONS ${PKG_GIT2_CFLAGS_OTHER})

find_path(LIBGIT2_INCLUDE_DIR
    NAMES
        git2.h
    HINTS
        ${PKG_GIT2_INCLUDE_DIRS}
)
find_library(LIBGIT2_LIBRARY
    NAMES
        git2
    HINTS
        ${PKG_GIT2_LIBRARY_DIRS}
)

# get version from header, should work on windows, too
if(LIBGIT2_INCLUDE_DIR)
    file(STRINGS "${LIBGIT2_INCLUDE_DIR}/git2/version.h" LIBGIT2_H REGEX "^#define LIBGIT2_VERSION \"[^\"]*\"$")

    string(REGEX REPLACE "^.*LIBGIT2_VERSION \"([0-9]+).*$" "\\1" LIBGIT2_VERSION_MAJOR "${LIBGIT2_H}")
    string(REGEX REPLACE "^.*LIBGIT2_VERSION \"[0-9]+\\.([0-9]+).*$" "\\1" LIBGIT2_VERSION_MINOR  "${LIBGIT2_H}")
    string(REGEX REPLACE "^.*LIBGIT2_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" LIBGIT2_VERSION_PATCH "${LIBGIT2_H}")
    set(LIBGIT2_VERSION "${LIBGIT2_VERSION_MAJOR}.${LIBGIT2_VERSION_MINOR}.${LIBGIT2_VERSION_PATCH}")

    set(LIBGIT2_MAJOR_VERSION "${LIBGIT2_VERSION_MAJOR}")
    set(LIBGIT2_MINOR_VERSION "${LIBGIT2_VERSION_MINOR}")
    set(LIBGIT2_PATCH_VERSION "${LIBGIT2_VERSION_PATCH}")

    unset(LIBGIT2_H)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibGit2
    FOUND_VAR
        LIBGIT2_FOUND
    REQUIRED_VARS
        LIBGIT2_LIBRARY
        LIBGIT2_INCLUDE_DIR
    VERSION_VAR
        LIBGIT2_VERSION
)

if(LIBGIT2_FOUND AND NOT TARGET LibGit2::LibGit2)
    add_library(LibGit2::LibGit2 UNKNOWN IMPORTED)
    set_target_properties(LibGit2::LibGit2 PROPERTIES
        IMPORTED_LOCATION "${LIBGIT2_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${LIBGIT2_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${LIBGIT2_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(LIBGIT2_LIBRARY LIBGIT2_INCLUDE_DIR)

set(LIBGIT2_LIBRARIES ${LIBGIT2_LIBRARY})
set(LIBGIT2_INCLUDE_DIRS ${LIBGIT2_INCLUDE_DIR})

include(FeatureSummary)
set_package_properties(LibGit2 PROPERTIES
    URL "https://libgit2.github.com/"
    DESCRIPTION "A plain C library to interface with the git version control system."
)
