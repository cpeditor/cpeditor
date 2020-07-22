#.rst:
# KDEFrameworkCompilerSettings
# ----------------------------
#
# Set stricter compile and link flags for KDE Frameworks modules.
#
# The KDECompilerSettings module is included and, in addition, various
# defines that affect the Qt libraries are set to enforce certain
# conventions.
#
# For example, constructions like QString("foo") are prohibited, instead
# forcing the use of QLatin1String or QStringLiteral, and some
# Qt-defined keywords like signals and slots will not be defined.
#
# NB: it is recommended to include this module with the NO_POLICY_SCOPE
# flag, otherwise you may get spurious warnings with some versions of CMake.
#
# Since pre-1.0.0.

#=============================================================================
# SPDX-FileCopyrightText: 2013 Albert Astals Cid <aacid@kde.org>
# SPDX-FileCopyrightText: 2007 Matthias Kretz <kretz@kde.org>
# SPDX-FileCopyrightText: 2006-2007 Laurent Montel <montel@kde.org>
# SPDX-FileCopyrightText: 2006-2013 Alex Neundorf <neundorf@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

include(KDECompilerSettings NO_POLICY_SCOPE)

add_definitions(-DQT_NO_CAST_TO_ASCII
                -DQT_NO_CAST_FROM_ASCII
                -DQT_NO_URL_CAST_FROM_STRING
                -DQT_NO_CAST_FROM_BYTEARRAY
                -DQT_NO_SIGNALS_SLOTS_KEYWORDS
                -DQT_USE_QSTRINGBUILDER
                -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
               )

if (NOT WIN32)
    # Strict iterators can't be used on Windows, they lead to a link error
    # when application code iterates over a QVector<QPoint> for instance, unless
    # Qt itself was also built with strict iterators.
    # See example at https://bugreports.qt.io/browse/AUTOSUITE-946
    add_definitions(-DQT_STRICT_ITERATORS)
endif()

add_definitions(
    -DQT_DEPRECATED_WARNINGS_SINCE=0x060000
    -DKF_DEPRECATED_WARNINGS_SINCE=0x060000
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0.0")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wzero-as-null-pointer-constant" )
   endif()
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
   if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0.0")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wzero-as-null-pointer-constant" )
   endif()
endif()
