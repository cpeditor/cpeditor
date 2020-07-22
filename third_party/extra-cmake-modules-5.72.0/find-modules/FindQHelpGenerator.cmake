# WARNING: FOR ECM-INTERNAL USE ONLY, DO NOT USE IN OWN PROJECTS
# THIS FILE MIGHT DISAPPEAR IN FUTURE VERSIONS OF ECM.

# Finds the Qt5 QHelpGenerator
#
#  QHelpGenerator_FOUND     - True if QHelpGenerator found.
#  QHelpGenerator_EXECUTABLE - Path to executable

#=============================================================================
# SPDX-FileCopyrightText: 2016 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

find_package(Qt5Help QUIET)
if (TARGET Qt5::qhelpgenerator)
    get_target_property(QHelpGenerator_EXECUTABLE Qt5::qhelpgenerator LOCATION)
else()
    # assume same folder as qmake executable
    if (TARGET Qt5::qmake)
        get_target_property(_qmake_EXECUTABLE Qt5::qmake LOCATION)
        get_filename_component(_path ${_qmake_EXECUTABLE} DIRECTORY)
    else()
        set(_path)
    endif()
    find_program(QHelpGenerator_EXECUTABLE
        NAMES
            qhelpgenerator-qt5
            qhelpgenerator
        PATHS
            ${_path}
        NO_DEFAULT_PATH
    )
endif()
mark_as_advanced(QHelpGenerator_EXECUTABLE)

if(QHelpGenerator_EXECUTABLE)
    set(QHelpGenerator_FOUND TRUE)
else()
    set(QHelpGenerator_FOUND FALSE)
endif()

if(QHelpGenerator_FOUND)
  if(NOT QHelpGenerator_FIND_QUIETLY )
    message( STATUS "Found QHelpGenerator executable: ${QHelpGenerator_EXECUTABLE}")
  endif()
else()
  if(QHelpGenerator_FIND_REQUIRED)
    message( FATAL_ERROR "Could not find QHelpGenerator executable" )
  else()
    message( STATUS "Optional QHelpGenerator executable was not found" )
  endif()
endif()

