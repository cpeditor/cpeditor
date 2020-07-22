#.rst:
# FindUDev
# --------
#
# Try to find the UDev library.
#
# This will define the following variables:
#
# ``UDev_FOUND``
#     System has UDev.
#
# ``UDev_INCLUDE_DIRS``
#     The libudev include directory.
#
# ``UDev_LIBRARIES``
#     The libudev libraries.
#
# ``UDev_VERSION``
#     The libudev version.
#
# If ``UDev_FOUND`` is TRUE, it will also define the following imported
# target:
#
# ``UDev::UDev``
#     The UDev library
#
# Since 5.57.0.

#=============================================================================
# SPDX-FileCopyrightText: 2010 Rafael Fernández López <ereslibre@kde.org>
# SPDX-FileCopyrightText: 2019 Volker Krause <vkrause@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

find_package(PkgConfig QUIET)
pkg_check_modules(PC_UDEV QUIET libudev)

find_path(UDev_INCLUDE_DIRS NAMES libudev.h HINTS ${PC_UDEV_INCLUDE_DIRS})
find_library(UDev_LIBRARIES NAMES udev HINTS ${PC_UDEV_LIBRARY_DIRS})

set(UDev_VERSION ${PC_UDEV_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UDev
    FOUND_VAR UDev_FOUND
    REQUIRED_VARS UDev_INCLUDE_DIRS UDev_LIBRARIES
    VERSION_VAR UDev_VERSION
)

mark_as_advanced(UDev_INCLUDE_DIRS UDev_LIBRARIES)

if(UDev_FOUND AND NOT TARGET UDev::UDev)
    add_library(UDev::UDev UNKNOWN IMPORTED)
    set_target_properties(UDev::UDev PROPERTIES
        IMPORTED_LOCATION "${UDev_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${UDev_INCLUDE_DIRS}"
        INTERFACE_COMPILE_DEFINITIONS "${PC_UDEV_CFLAGS_OTHER}"
    )
endif()

# backward compat variables, remove for KF6
set(UDEV_FOUND ${UDev_FOUND})
set(UDEV_LIBS ${UDev_LIBRARIES})
set(UDEV_INCLUDE_DIR ${UDev_INCLUDE_DIRS})
mark_as_advanced(UDEV_FOUND UDEV_LIBS UDEV_INCLUDE_DIR)

include(FeatureSummary)
set_package_properties(UDev PROPERTIES
    DESCRIPTION "API for enumerating and introspecting local devices (part of systemd)"
    URL "https://freedesktop.org/wiki/Software/systemd/"
)
