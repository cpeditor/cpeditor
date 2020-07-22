#.rst:
# KDEClangFormat
# --------------------
#
# This module provides a functionality to format the source
# code of your repository according to a predefined KDE
# clang-format file.
#
# This module provides the following function:
#
# ::
#
#   kde_clang_format(<files>)
#
# Using this function will create a clang-format target that will format all
# ``<files>`` passed to the function with the predefined KDE clang-format style.
#
# Example usage:
#
# .. code-block:: cmake
#
#   include(KDEClangFormat)
#   file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
#   kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
#
# Since 5.64

#=============================================================================
# SPDX-FileCopyrightText: 2019 Christoph Cullmann <cullmann@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

# try to find clang-format in path
find_program(KDE_CLANG_FORMAT_EXECUTABLE clang-format)

# instantiate our clang-format file, must be in source directory for tooling if we have the tool
if(KDE_CLANG_FORMAT_EXECUTABLE)
    configure_file(${CMAKE_CURRENT_LIST_DIR}/clang-format.cmake ${CMAKE_CURRENT_SOURCE_DIR}/.clang-format @ONLY)
endif()

# formatting target
function(KDE_CLANG_FORMAT)
    # add target only if clang-format available
    if(KDE_CLANG_FORMAT_EXECUTABLE)
        add_custom_target(clang-format
        COMMAND
            ${KDE_CLANG_FORMAT_EXECUTABLE}
            -style=file
            -i
            ${ARGV}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT
            "Formatting sources in ${CMAKE_CURRENT_SOURCE_DIR} with ${KDE_CLANG_FORMAT_EXECUTABLE}..."
        )
    else()
        add_custom_target(clang-format
        COMMAND
            ${CMAKE_COMMAND} -E echo
            "Could not set up the clang-format target as the clang-format executable is missing."
        )
    endif()
endfunction()
