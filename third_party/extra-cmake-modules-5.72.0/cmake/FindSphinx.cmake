#.rst:
# FindSphinx
# ----------
#
# Try to find the Sphinx documentation builder.
#
# This will define the following variables:
#
# ``Sphinx_FOUND``
#     True if (the requested version of) Sphinx is available
# ``Sphinx_VERSION``
#     The version of the Sphinx documentation builder.
# ``Sphinx_BUILD_EXECUTABLE``
#     The path to the Sphinx documentation builder executable.
#
# If ``Sphinx_FOUND`` is TRUE, it will also define the following imported target:
#
# ``Sphinx::Build``
#     The Sphinx documentation builder.
#
# In general we recommend using the imported target, as it is easier to use.
#
# Since 5.17.0.

#=============================================================================
# SPDX-FileCopyrightText: 2015 Alex Merry <alex.merry@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

# Distros sometimes rename Python executables to allow for parallel
# installation of Python2 and Python3 versions
find_program(Sphinx_BUILD_EXECUTABLE
    NAMES
        sphinx-build
        sphinx-build2
        sphinx-build3
    DOC "Sphinx Documentation Builder (https://www.sphinx-doc.org/)"
)

if (Sphinx_BUILD_EXECUTABLE)
    if(NOT TARGET Sphinx::Build)
        add_executable(Sphinx::Build IMPORTED)
        set_target_properties(Sphinx::Build PROPERTIES
            IMPORTED_LOCATION "${Sphinx_BUILD_EXECUTABLE}"
        )
    endif()

    execute_process(
        COMMAND "${Sphinx_BUILD_EXECUTABLE}" --version
        OUTPUT_VARIABLE _Sphinx_version_raw
        ERROR_VARIABLE _Sphinx_version_raw
    )
    if (_Sphinx_version_raw MATCHES "^Sphinx \\([^)]*\\) ([0-9]+(\\.[0-9]+)*)")
        set(Sphinx_VERSION "${CMAKE_MATCH_1}")
    endif()
    unset(_Sphinx_version_raw)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sphinx
    FOUND_VAR
        Sphinx_FOUND
    REQUIRED_VARS
        Sphinx_BUILD_EXECUTABLE
    VERSION_VAR
        Sphinx_VERSION
)

mark_as_advanced(Sphinx_BUILD_EXECUTABLE)
