#.rst:
# FindCanberra
# ------------
#
# Try to find Canberra event sound library.
#
# This will define the following variables:
#
# ``Canberra_FOUND``
#     True if (the requested version of) Canberra is available
# ``Canberra_VERSION``
#     The version of Canberra
# ``Canberra_LIBRARIES``
#     The libraries of Canberra for use with target_link_libraries()
# ``Canberra_INCLUDE_DIRS``
#     The include dirs of Canberra for use with target_include_directories()
#
# If ``Canberra_FOUND`` is TRUE, it will also define the following imported
# target:
#
# ``Canberra::Canberra``
#     The Canberra library
#
# In general we recommend using the imported target, as it is easier to use.
# Bear in mind, however, that if the target is in the link interface of an
# exported library, it must be made available by the package config file.
#
# Since 5.56.0.

#=============================================================================
# SPDX-FileCopyrightText: 2012 Raphael Kubo da Costa <rakuco@FreeBSD.org>
# SPDX-FileCopyrightText: 2019 Harald Sitter <sitter@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

find_package(PkgConfig QUIET)
pkg_check_modules(PC_Canberra libcanberra QUIET)

find_library(Canberra_LIBRARIES
    NAMES canberra
    HINTS ${PC_Canberra_LIBRARY_DIRS}
)

find_path(Canberra_INCLUDE_DIRS
    NAMES canberra.h
    HINTS ${PC_Canberra_INCLUDE_DIRS}
)

set(Canberra_VERSION ${PC_Canberra_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Canberra
    FOUND_VAR
        Canberra_FOUND
    REQUIRED_VARS
        Canberra_LIBRARIES
        Canberra_INCLUDE_DIRS
    VERSION_VAR
        Canberra_VERSION
)

if(Canberra_FOUND AND NOT TARGET Canberra::Canberra)
    add_library(Canberra::Canberra UNKNOWN IMPORTED)
    set_target_properties(Canberra::Canberra PROPERTIES
        IMPORTED_LOCATION "${Canberra_LIBRARIES}"
        INTERFACE_COMPILE_OPTIONS "${PC_Canberra_CFLAGS}"
        INTERFACE_INCLUDE_DIRECTORIES "${Canberra_INCLUDE_DIRS}"
    )
endif()

mark_as_advanced(Canberra_LIBRARIES Canberra_INCLUDE_DIRS Canberra_VERSION)

include(FeatureSummary)
set_package_properties(Canberra PROPERTIES
    DESCRIPTION "Event sound library"
    URL "http://0pointer.de/lennart/projects/libcanberra"
)

# Compatibility variables. In a previous life FindCanberra lived
# in a number of different repos: don't break them if they use ECM but have not
# been updated for this finder.
set(CANBERRA_FOUND ${Canberra_FOUND})
set(CANBERRA_VERSION ${Canberra_VERSION})
set(CANBERRA_LIBRARIES ${Canberra_LIBRARIES})
set(CANBERRA_INCLUDE_DIRS ${Canberra_INCLUDE_DIRS})
mark_as_advanced(CANBERRA_VERSION CANBERRA_LIBRARIES CANBERRA_INCLUDE_DIRS)
