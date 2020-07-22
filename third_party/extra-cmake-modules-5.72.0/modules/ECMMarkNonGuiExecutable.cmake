#.rst:
# ECMMarkNonGuiExecutable
# -----------------------
#
# Marks an executable target as not being a GUI application.
#
# ::
#
#   ecm_mark_nongui_executable(<target1> [<target2> [...]])
#
# This will indicate to CMake that the specified targets should not be included
# in a MACOSX_BUNDLE and should not be WIN32_EXECUTABLEs.  On platforms other
# than MacOS X or Windows, this will have no effect.
#
# Since pre-1.0.0.

#=============================================================================
# SPDX-FileCopyrightText: 2012 Stephen Kelly <steveire@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause

function(ecm_mark_nongui_executable)
  foreach(_target ${ARGN})
    set_target_properties(${_target}
                            PROPERTIES
                            WIN32_EXECUTABLE FALSE
                            MACOSX_BUNDLE FALSE
                          )
  endforeach()
endfunction()
