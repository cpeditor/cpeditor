#.rst:
# FindIcoTool
# -----------
#
# Try to find icotool.
#
# If the icotool executable is not in your PATH, you can provide
# an alternative name or full path location with the ``IcoTool_EXECUTABLE``
# variable.
#
# This will define the following variables:
#
# ``IcoTool_FOUND``
#     True if icotool is available.
#
# ``IcoTool_EXECUTABLE``
#     The icotool executable.
#
# If ``IcoTool_FOUND`` is TRUE, it will also define the following imported
# target:
#
# ``IcoTool::IcoTool``
#     The icotool executable.
#
# Since 5.49.

#=============================================================================
# SPDX-FileCopyrightText: 2017 Vincent Pinon <vpinon@kde.org>
# SPDX-FileCopyrightText: 2014 Alex Merry <alex.merry@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

include(${CMAKE_CURRENT_LIST_DIR}/ECMFindModuleHelpersStub.cmake)
ecm_find_package_version_check(IcoTool)
find_program(IcoTool_EXECUTABLE NAMES icotool)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IcoTool
    FOUND_VAR
        IcoTool_FOUND
    REQUIRED_VARS
        IcoTool_EXECUTABLE
)
mark_as_advanced(IcoTool_EXECUTABLE)

if (IcoTool_FOUND)
    if (NOT TARGET IcoTool::IcoTool)
        add_executable(IcoTool::IcoTool IMPORTED)
        set_target_properties(IcoTool::IcoTool PROPERTIES
            IMPORTED_LOCATION "${IcoTool_EXECUTABLE}"
        )
    endif()
endif()

include(FeatureSummary)
set_package_properties(IcoTool PROPERTIES
    URL "https://www.nongnu.org/icoutils/"
    DESCRIPTION "Executable that converts a collection of PNG files into a Windows icon file"
)
