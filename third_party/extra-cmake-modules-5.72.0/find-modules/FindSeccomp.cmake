#.rst:
# FindSeccomp
# -----------
#
# Try to locate the libseccomp library.
#
# This will define the following variables:
#
# ``Seccomp_FOUND``
#     True if the seccomp library is available
# ``Seccomp_INCLUDE_DIRS``
#     The seccomp include directories
# ``Seccomp_LIBRARIES``
#     The seccomp libraries for linking
#
# If ``Seccomp_FOUND`` is TRUE, it will also define the following
# imported target:
#
# ``Seccomp::Seccomp``
#     The Seccomp library
#
# Since 5.44.0.

#=============================================================================
# SPDX-FileCopyrightText: 2017 Martin Flöser <mgraesslin@kde.org>
# SPDX-FileCopyrightText: 2017 David Kahles <david.kahles96@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

find_package(PkgConfig QUIET)
pkg_check_modules(PKG_Libseccomp QUIET libseccomp)

find_path(Seccomp_INCLUDE_DIRS
    NAMES
        seccomp.h
    HINTS
        ${PKG_Libseccomp_INCLUDE_DIRS}
)
find_library(Seccomp_LIBRARIES
    NAMES
        seccomp
    HINTS
        ${PKG_Libseccomp_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Seccomp
    FOUND_VAR
        Seccomp_FOUND
    REQUIRED_VARS
        Seccomp_LIBRARIES
        Seccomp_INCLUDE_DIRS
)

if (Seccomp_FOUND AND NOT TARGET Seccomp::Seccomp)
    add_library(Seccomp::Seccomp UNKNOWN IMPORTED)
    set_target_properties(Seccomp::Seccomp PROPERTIES
        IMPORTED_LOCATION "${Seccomp_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${Seccomp_INCLUDE_DIRS}"
    )
endif()

mark_as_advanced(Seccomp_LIBRARIES Seccomp_INCLUDE_DIRS)

include(FeatureSummary)
set_package_properties(Seccomp PROPERTIES
    URL "https://github.com/seccomp/libseccomp"
    DESCRIPTION "The enhanced seccomp library."
)
