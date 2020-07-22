#.rst:
# FindQCollectionGenerator
# ------------------------
#
# Try to find the Qt help collection generator.
#
# This will define the following variables:
#
# ``QCollectionGenerator_FOUND``
#     True if (the requested version of) Sphinx is available
# ``QCollectionGenerator_VERSION``
#     The version of the Qt help collection generator. Note that this not the
#     same as the version of Qt it is provided by.
# ``QCollectionGenerator_QT_VERSION``
#     The version of Qt that the Qt help collection generator is from.
# ``QCollectionGenerator_EXECUTABLE``
#     The path to the Qt help collection generator executable.
#
# If ``QCollectionGenerator_FOUND`` is TRUE, it will also define the following
# imported target:
#
# ``QCollectionGenerator::Generator``
#     The Qt help collection generator.
#
# In general we recommend using the imported target, as it is easier to use.
#
# Since 5.17.0.

#=============================================================================
# SPDX-FileCopyrightText: 2015 Alex Merry <alex.merry@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

find_program(QCollectionGenerator_EXECUTABLE
    NAMES
        qcollectiongenerator
        qcollectiongenerator-qt4
        qcollectiongenerator-qt5
    DOC "Qt help collection generator"
)

if (QCollectionGenerator_EXECUTABLE)
    if(NOT TARGET QCollectionGenerator::Generator)
        add_executable(QCollectionGenerator::Generator IMPORTED)
        set_target_properties(QCollectionGenerator::Generator PROPERTIES
            IMPORTED_LOCATION "${QCollectionGenerator_EXECUTABLE}"
        )
    endif()

    execute_process(
        COMMAND "${QCollectionGenerator_EXECUTABLE}" -v
        OUTPUT_VARIABLE _QCollectionGenerator_version_raw
        ERROR_VARIABLE _QCollectionGenerator_version_raw
    )
    if (_QCollectionGenerator_version_raw MATCHES "^Qt Collection Generator version ([0-9]+(\\.[0-9]+)*) \\(Qt ([0-9]+(\\.[0-9]+)*)\\)")
        set(QCollectionGenerator_VERSION "${CMAKE_MATCH_1}")
        set(QCollectionGenerator_QT_VERSION "${CMAKE_MATCH_3}")
    endif()
    unset(_QCollectionGenerator_version_raw)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QCollectionGenerator
    FOUND_VAR
        QCollectionGenerator_FOUND
    REQUIRED_VARS
        QCollectionGenerator_EXECUTABLE
    VERSION_VAR
        QCollectionGenerator_VERSION
)

mark_as_advanced(QCollectionGenerator_EXECUTABLE)
