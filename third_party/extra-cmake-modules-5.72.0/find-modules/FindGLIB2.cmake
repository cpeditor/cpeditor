#.rst:
# FindGLIB2
# ---------
#
# Try to locate the GLib2 library.
# If found, this will define the following variables:
#
# ``GLIB2_FOUND``
#     True if the GLib2 library is available
# ``GLIB2_INCLUDE_DIRS``
#     The GLib2 include directories
# ``GLIB2_LIBRARIES``
#     The GLib2 libraries for linking
# ``GLIB2_INCLUDE_DIR``
#     Deprecated, use ``GLIB2_INCLUDE_DIRS``
# ``GLIB2_LIBRARY``
#     Deprecated, use ``GLIB2_LIBRARIES``
#
# If ``GLIB2_FOUND`` is TRUE, it will also define the following
# imported target:
#
# ``GLIB2::GLIB2``
#     The GLIB2 library
#
# Since 5.41.0.

#=============================================================================
# SPDX-FileCopyrightText: 2008 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

find_package(PkgConfig)
pkg_check_modules(PC_GLIB2 QUIET glib-2.0)

find_path(GLIB2_INCLUDE_DIRS
          NAMES glib.h
          HINTS ${PC_GLIB2_INCLUDEDIR}
          PATH_SUFFIXES glib-2.0)

find_library(GLIB2_LIBRARIES
             NAMES glib-2.0
             HINTS ${PC_GLIB2_LIBDIR}
)

# search the glibconfig.h include dir under the same root where the library is found
get_filename_component(glib2LibDir "${GLIB2_LIBRARIES}" PATH)

find_path(GLIB2_INTERNAL_INCLUDE_DIR glibconfig.h
          PATH_SUFFIXES glib-2.0/include
          HINTS ${PC_GLIB2_INCLUDEDIR} "${glib2LibDir}" ${CMAKE_SYSTEM_LIBRARY_PATH})

# not sure if this include dir is optional or required
# for now it is optional
if(GLIB2_INTERNAL_INCLUDE_DIR)
  list(APPEND GLIB2_INCLUDE_DIRS "${GLIB2_INTERNAL_INCLUDE_DIR}")
endif()

# Deprecated synonyms
set(GLIB2_INCLUDE_DIR "${GLIB2_INCLUDE_DIRS}")
set(GLIB2_LIBRARY "${GLIB2_LIBRARIES}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLIB2 DEFAULT_MSG GLIB2_LIBRARIES GLIB2_INCLUDE_DIRS)

if(GLIB2_FOUND AND NOT TARGET GLIB2::GLIB2)
  add_library(GLIB2::GLIB2 UNKNOWN IMPORTED)
  set_target_properties(GLIB2::GLIB2 PROPERTIES
                        IMPORTED_LOCATION "${GLIB2_LIBRARIES}"
			INTERFACE_INCLUDE_DIRECTORIES "${GLIB2_INCLUDE_DIRS}")
endif()

mark_as_advanced(GLIB2_INCLUDE_DIRS GLIB2_INCLUDE_DIR
                 GLIB2_LIBRARIES GLIB2_LIBRARY)

include(FeatureSummary)
set_package_properties(GLIB2 PROPERTIES
  URL "https://wiki.gnome.org/Projects/GLib"
  DESCRIPTION "Event loop and utility library")
