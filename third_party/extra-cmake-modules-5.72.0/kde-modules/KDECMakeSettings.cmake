#.rst:
# KDECMakeSettings
# ----------------
#
# Changes various CMake settings to what the KDE community views as more
# sensible defaults.
#
# It is recommended to include this module with the NO_POLICY_SCOPE flag,
# otherwise you may get spurious warnings with some versions of CMake.
#
# It is split into three parts, which can be independently disabled if desired.
#
# Runtime Paths
# ~~~~~~~~~~~~~
#
# The default runtime path (used on Unix systems to search for
# dynamically-linked libraries) is set to include the location that libraries
# will be installed to (as set in LIB_INSTALL_DIR or, if the former is not set,
# KDE_INSTALL_LIBDIR), and also the linker search path.
#
# Note that ``LIB_INSTALL_DIR`` or alternatively ``KDE_INSTALL_LIBDIR`` needs
# to be set before including this module.
# Typically, this is done by including the :kde-module:`KDEInstallDirs` module.
#
# This section can be disabled by setting ``KDE_SKIP_RPATH_SETTINGS`` to TRUE
# before including this module.
#
#
# Testing
# ~~~~~~~
#
# Testing is enabled by default, and an option (BUILD_TESTING) is provided for
# users to control this. See the CTest module documentation in the CMake manual
# for more details.
#
# This section can be disabled by setting ``KDE_SKIP_TEST_SETTINGS`` to TRUE
# before including this module.
#
#
# Build Settings
# ~~~~~~~~~~~~~~
#
# Various CMake build defaults are altered, such as searching source and build
# directories for includes first, enabling automoc by default.
#
# When find_package(ECM 5.38) or higher is called, this also selects
# a layout for the build dir that helps running executables without installing:
# all executables are built into a toplevel "bin" dir, making it possible to find
# helper binaries, and to find uninstalled plugins (provided that you use
# kcoreaddons_add_plugin or set LIBRARY_OUTPUT_DIRECTORY as documented on
# https://community.kde.org/Guidelines_and_HOWTOs/Making_apps_run_uninstalled).
#
# This section can be disabled by setting ``KDE_SKIP_BUILD_SETTINGS`` to TRUE
# before including this module.
#
# This section also provides an "uninstall" target that can be individually
# disabled by setting ``KDE_SKIP_UNINSTALL_TARGET`` to TRUE before including
# this module.
#
# By default on OS X, X11 and XCB related detections are disabled. However if
# the need would arise to use these technologies, the detection can be enabled
# by setting ``APPLE_FORCE_X11`` to ``ON``.
#
# A warning is printed for the developer to know that the detection is disabled on OS X.
# This message can be turned off by setting ``APPLE_SUPPRESS_X11_WARNING`` to ``ON``.
#
# Since pre-1.0.0.
#
# ``ENABLE_CLAZY`` option is added (OFF by default) when clang is being used.
# Turning this option on will force clang to load the clazy plugins for richer
# warnings on Qt-related code.
#
# If clang is not being used, this won't have an effect.
# See https://commits.kde.org/clazy?path=README.md
#
# Since 5.17.0
#
# - Uninstall target functionality since 1.7.0.
# - ``APPLE_FORCE_X11`` option since 5.14.0 (detecting X11 was previously the default behavior)
# - ``APPLE_SUPPRESS_X11_WARNING`` option since 5.14.0
# - CMAKE_AUTORCC enabled by default when supported by cmake (>= 3.0) since 5.62.0
#
# Translations
# ~~~~~~~~~~~~
# A fetch-translations target will be set up that will download translations
# for projects using l10n.kde.org.
#
# ``KDE_L10N_BRANCH`` will be responsible for choosing which l10n branch to use
# for the translations.
#
# ``KDE_L10N_AUTO_TRANSLATIONS`` (OFF by default) will indicate whether translations
# should be downloaded when building the project.
#
# Since 5.34.0
#
# ``KDE_L10N_SYNC_TRANSLATIONS`` (OFF by default) will download the translations at configuration
# time instead of build time.
#
# Since 5.50.0
#
#

#=============================================================================
# SPDX-FileCopyrightText: 2014 Alex Merry <alex.merry@kde.org>
# SPDX-FileCopyrightText: 2013 Aleix Pol <aleixpol@kde.org>
# SPDX-FileCopyrightText: 2012-2013 Stephen Kelly <steveire@gmail.com>
# SPDX-FileCopyrightText: 2007 Matthias Kretz <kretz@kde.org>
# SPDX-FileCopyrightText: 2006-2007 Laurent Montel <montel@kde.org>
# SPDX-FileCopyrightText: 2006-2013 Alex Neundorf <neundorf@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

################# RPATH handling ##################################

if(NOT KDE_SKIP_RPATH_SETTINGS)

   # Set the default RPATH to point to useful locations, namely where the
   # libraries will be installed and the linker search path

   # First look for the old LIB_INSTALL_DIR, then fallback to newer KDE_INSTALL_LIBDIR
   if(NOT LIB_INSTALL_DIR)
      if(NOT KDE_INSTALL_LIBDIR)
         message(FATAL_ERROR "Neither KDE_INSTALL_LIBDIR nor LIB_INSTALL_DIR is set. Setting one is necessary for using the RPATH settings.")
      else()
         set(_abs_LIB_INSTALL_DIR "${KDE_INSTALL_LIBDIR}")
      endif()
   else()
      set(_abs_LIB_INSTALL_DIR "${LIB_INSTALL_DIR}")
   endif()

   if (NOT IS_ABSOLUTE "${_abs_LIB_INSTALL_DIR}")
      set(_abs_LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}")
   endif()

   if (UNIX)
      # for mac os: add install name dir in addition
      # check: is the rpath stuff below really required on mac os? at least it seems so to use a stock qt from qt.io
      if (APPLE)
         set(CMAKE_INSTALL_NAME_DIR ${_abs_LIB_INSTALL_DIR})
      endif ()

      # add our LIB_INSTALL_DIR to the RPATH (but only when it is not one of
      # the standard system link directories - such as /usr/lib on UNIX)
      list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${_abs_LIB_INSTALL_DIR}" _isSystemLibDir)
      list(FIND CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES      "${_abs_LIB_INSTALL_DIR}" _isSystemCxxLibDir)
      list(FIND CMAKE_C_IMPLICIT_LINK_DIRECTORIES        "${_abs_LIB_INSTALL_DIR}" _isSystemCLibDir)
      if("${_isSystemLibDir}" STREQUAL "-1"  AND  "${_isSystemCxxLibDir}" STREQUAL "-1"  AND  "${_isSystemCLibDir}" STREQUAL "-1")
         set(CMAKE_INSTALL_RPATH "${_abs_LIB_INSTALL_DIR}")
      endif()

      # Append directories in the linker search path (but outside the project)
      set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
   endif (UNIX)

endif()

################ Testing setup ####################################

find_program(APPSTREAMCLI appstreamcli)
function(appstreamtest)
    if(APPSTREAMCLI AND NOT appstreamtest_added)
        set(appstreamtest_added TRUE PARENT_SCOPE)
        add_test(NAME appstreamtest COMMAND ${CMAKE_COMMAND} -DAPPSTREAMCLI=${APPSTREAMCLI} -DINSTALL_FILES=${CMAKE_BINARY_DIR}/install_manifest.txt -P ${CMAKE_CURRENT_LIST_DIR}/appstreamtest.cmake)
    else()
        message(STATUS "Could not set up the appstream test. appstreamcli is missing.")
    endif()
endfunction()

if(NOT KDE_SKIP_TEST_SETTINGS)

   # If there is a CTestConfig.cmake, include CTest.
   # Otherwise, there will not be any useful settings, so just
   # fake the functionality we care about from CTest.

   if (EXISTS ${CMAKE_SOURCE_DIR}/CTestConfig.cmake)
      include(CTest)
   else()
      option(BUILD_TESTING "Build the testing tree." ON)
      if(BUILD_TESTING)
         enable_testing()
         appstreamtest()
      endif ()
   endif ()

endif()



################ Build-related settings ###########################

if(NOT KDE_SKIP_BUILD_SETTINGS)

   # Always include srcdir and builddir in include path
   # This saves typing ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} in about every subdir
   # since cmake 2.4.0
   set(CMAKE_INCLUDE_CURRENT_DIR ON)

   # put the include dirs which are in the source or build tree
   # before all other include dirs, so the headers in the sources
   # are preferred over the already installed ones
   # since cmake 2.4.1
   set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

   # Add the src and build dir to the BUILD_INTERFACE include directories
   # of all targets. Similar to CMAKE_INCLUDE_CURRENT_DIR, but transitive.
   # Since CMake 2.8.11
   set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

   # When a shared library changes, but its includes do not, don't relink
   # all dependencies. It is not needed.
   # Since CMake 2.8.11
   set(CMAKE_LINK_DEPENDS_NO_SHARED ON)

   # Default to shared libs for KDE, if no type is explicitly given to add_library():
   set(BUILD_SHARED_LIBS TRUE CACHE BOOL "If enabled, shared libs will be built by default, otherwise static libs")

   # Enable automoc in cmake
   # Since CMake 2.8.6
   set(CMAKE_AUTOMOC ON)

   # Enable autorcc and in cmake so qrc files get generated.
   # Since CMake 3.0
   # TODO KF6: discuss enabling AUTOUIC and note porting requirements. autouic
   #   acts on all #include "ui_*.h" assuming *.ui exists
   if(NOT CMAKE_VERSION VERSION_LESS 3.0)
      set(CMAKE_AUTORCC ON)
   endif()

   # By default, create 'GUI' executables. This can be reverted on a per-target basis
   # using ECMMarkNonGuiExecutable
   # Since CMake 2.8.8
   set(CMAKE_WIN32_EXECUTABLE ON)
   set(CMAKE_MACOSX_BUNDLE ON)

   # By default, don't put a prefix on MODULE targets. add_library(MODULE) is basically for plugin targets,
   # and in KDE plugins don't have a prefix.
   set(CMAKE_SHARED_MODULE_PREFIX "")

   unset(EXECUTABLE_OUTPUT_PATH)
   unset(LIBRARY_OUTPUT_PATH)
   unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
   unset(CMAKE_LIBRARY_OUTPUT_DIRECTORY)
   unset(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

   # under Windows, output all executables and dlls into
   # one common directory, and all static|import libraries and plugins
   # into another one. This way test executables can find their dlls
   # even without installation.

   # We do the same under Unix to make it possible to run tests and apps without installing
   if (WIN32 OR NOT ("${ECM_GLOBAL_FIND_VERSION}" VERSION_LESS "5.38.0"))
       set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
       set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
       set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
   endif()

   if (APPLE)
       # Disable detection of X11 and related package on OS X because when using
       # brew or macports, X11 can be installed and thus is detected.
       option(APPLE_FORCE_X11 "Force enable X11 related detection on OS X" OFF)
       option(APPLE_SUPPRESS_X11_WARNING "Suppress X11 and related technologies search disabling warning on OS X" OFF)

       if(NOT APPLE_FORCE_X11)
           if (NOT APPLE_SUPPRESS_X11_WARNING)
               message(WARNING "Searching for X11 and related technologies is disabled on Apple systems. Set APPLE_FORCE_X11 to ON to change this behaviour. Set APPLE_SUPPRESS_X11_WARNING to ON to hide this warning.")
           endif()
           set(CMAKE_DISABLE_FIND_PACKAGE_X11 true)
           set(CMAKE_DISABLE_FIND_PACKAGE_XCB true)
           set(CMAKE_DISABLE_FIND_PACKAGE_Qt5X11Extras true)
       endif()
    endif()

    option(KDE_SKIP_UNINSTALL_TARGET "Prevent an \"uninstall\" target from being generated." OFF)
   if(NOT KDE_SKIP_UNINSTALL_TARGET)
       include("${ECM_MODULE_DIR}/ECMUninstallTarget.cmake")
   endif()

endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    option(ENABLE_CLAZY "Enable Clazy warnings" OFF)

    if(ENABLE_CLAZY)
        find_library(CLAZY_v1_5_FOUND ClazyPlugin${CMAKE_SHARED_LIBRARY_SUFFIX})
        if(CLAZY_v1_5_FOUND) # clazy >= 1.5
            set(CMAKE_CXX_COMPILE_OBJECT "${CMAKE_CXX_COMPILE_OBJECT} -Xclang -load -Xclang ClazyPlugin${CMAKE_SHARED_LIBRARY_SUFFIX} -Xclang -add-plugin -Xclang clazy")
        else() # clazy < 1.5
            set(CMAKE_CXX_COMPILE_OBJECT "${CMAKE_CXX_COMPILE_OBJECT} -Xclang -load -Xclang ClangLazy${CMAKE_SHARED_LIBRARY_SUFFIX} -Xclang -add-plugin -Xclang clang-lazy")
        endif()
    endif()
endif()

###################################################################
# Download translations

function(_repository_name reponame dir)
    execute_process(COMMAND git config --get remote.origin.url
        OUTPUT_VARIABLE giturl
        RESULT_VARIABLE exitCode
        WORKING_DIRECTORY "${dir}")

    if(exitCode EQUAL 0)
        string(REGEX MATCHALL ".+[:\\/]([-A-Za-z0-9]+)(.git)?\\s*" "" ${giturl})
        set(${reponame} ${CMAKE_MATCH_1})
    endif()

    if(NOT ${reponame})
        set(${reponame} ${CMAKE_PROJECT_NAME})
    endif()
    set(${reponame} ${${reponame}} PARENT_SCOPE)
endfunction()

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/po AND NOT TARGET fetch-translations)
    option(KDE_L10N_AUTO_TRANSLATIONS "Automatically 'make fetch-translations`" OFF)
    option(KDE_L10N_SYNC_TRANSLATIONS "Fetch translations when KDECMakeSettings.cmake is processed." OFF)
    set(KDE_L10N_BRANCH "trunk" CACHE STRING "Branch from l10n.kde.org to fetch from: trunk | stable | lts | trunk_kde4 | stable_kde4")

    if(KDE_L10N_AUTO_TRANSLATIONS AND NOT KDE_L10N_SYNC_TRANSLATIONS)
        set(_EXTRA_ARGS "ALL")
    else()
        set(_EXTRA_ARGS)
    endif()

    set(_reponame "")
    _repository_name(_reponame "${CMAKE_SOURCE_DIR}")

    set(releaseme_clone_commands
        COMMAND git clone --depth 1 https://anongit.kde.org/releaseme.git
    )
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/releaseme"
        ${releaseme_clone_commands}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Fetching releaseme scripts to download translations..."
    )

    set(_l10n_po_dir "${CMAKE_BINARY_DIR}/po")
    set(_l10n_poqm_dir "${CMAKE_BINARY_DIR}/poqm")

    if(CMAKE_VERSION VERSION_GREATER 3.2)
        set(extra BYPRODUCTS ${_l10n_po_dir} ${_l10n_poqm_dir})
    endif()

    set(fetch_commands
        COMMAND ruby "${CMAKE_BINARY_DIR}/releaseme/fetchpo.rb"
            --origin ${KDE_L10N_BRANCH}
            --project "${_reponame}"
            --output-dir "${_l10n_po_dir}"
            --output-poqm-dir "${_l10n_poqm_dir}"
            "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    add_custom_target(fetch-translations ${_EXTRA_ARGS}
        COMMENT "Downloading translations for ${_reponame} branch ${KDE_L10N_BRANCH}..."
        COMMAND git -C "${CMAKE_BINARY_DIR}/releaseme" pull
        COMMAND cmake -E remove_directory ${_l10n_po_dir}
        COMMAND cmake -E remove_directory ${_l10n_poqm_dir}
        ${fetch_commands}
        ${extra}
        DEPENDS "${CMAKE_BINARY_DIR}/releaseme"
    )

    if (KDE_L10N_SYNC_TRANSLATIONS AND (NOT EXISTS ${_l10n_po_dir} OR NOT EXISTS ${_l10n_poqm_dir}))
        execute_process(${releaseme_clone_commands})
        execute_process(${fetch_commands})
    endif()
endif()
