#.rst:
# ECMInstallIcons
# ---------------
#
# Installs icons, sorting them into the correct directories according to the
# FreeDesktop.org icon naming specification.
#
# ::
#
#   ecm_install_icons(ICONS <icon> [<icon> [...]]
#                     DESTINATION <icon_install_dir>
#                     [LANG <l10n_code>]
#                     [THEME <theme>])
#
# The given icons, whose names must match the pattern::
#
#   <size>-<group>-<name>.<ext>
#
# will be installed to the appropriate subdirectory of DESTINATION according to
# the FreeDesktop.org icon naming scheme. By default, they are installed to the
# "hicolor" theme, but this can be changed using the THEME argument.  If the
# icons are localized, the LANG argument can be used to install them in a
# locale-specific directory.
#
# ``<size>`` is a numeric pixel size (typically 16, 22, 32, 48, 64, 128 or 256)
# or ``sc`` for scalable (SVG) files, ``<group>`` is one of the standard
# FreeDesktop.org icon groups (actions, animations, apps, categories, devices,
# emblems, emotes, intl, mimetypes, places, status) and ``<ext>`` is one of
# ``.png``, ``.mng`` or ``.svgz``.
#
# The typical installation directory is ``share/icons``.
#
# .. code-block:: cmake
#
#   ecm_install_icons(ICONS 22-actions-menu_new.png
#                     DESTINATION share/icons)
#
# The above code will install the file ``22-actions-menu_new.png`` as
# ``${CMAKE_INSTALL_PREFIX}/share/icons/<theme>/22x22/actions/menu_new.png``
#
# Users of the :kde-module:`KDEInstallDirs` module would normally use
# ``${KDE_INSTALL_ICONDIR}`` as the DESTINATION, while users of the GNUInstallDirs
# module should use ``${CMAKE_INSTALL_DATAROOTDIR}/icons``.
#
# An old form of arguments will also be accepted::
#
#   ecm_install_icons(<icon_install_dir> [<l10n_code>])
#
# This matches files named like::
#
#   <theme><size>-<group>-<name>.<ext>
#
# where ``<theme>`` is one of
#  * ``hi`` for hicolor
#  * ``lo`` for locolor
#  * ``cr`` for the Crystal icon theme
#  * ``ox`` for the Oxygen icon theme
#  * ``br`` for the Breeze icon theme
#
# With this syntax, the file ``hi22-actions-menu_new.png`` would be installed
# into ``<icon_install_dir>/hicolor/22x22/actions/menu_new.png``
#
# Since pre-1.0.0.

#=============================================================================
# SPDX-FileCopyrightText: 2014 Alex Merry <alex.merry@kde.org>
# SPDX-FileCopyrightText: 2013 David Edmundson <kde@davidedmundson.co.uk>
# SPDX-FileCopyrightText: 2008 Chusslove Illich <caslav.ilic@gmx.net>
# SPDX-FileCopyrightText: 2006 Alex Neundorf <neundorf@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

include(CMakeParseArguments)

# A "map" of short type names to the directories.
# Unknown names produce a warning.
set(_ECM_ICON_GROUP_mimetypes  "mimetypes")
set(_ECM_ICON_GROUP_places     "places")
set(_ECM_ICON_GROUP_devices    "devices")
set(_ECM_ICON_GROUP_apps       "apps")
set(_ECM_ICON_GROUP_actions    "actions")
set(_ECM_ICON_GROUP_categories "categories")
set(_ECM_ICON_GROUP_status     "status")
set(_ECM_ICON_GROUP_emblems    "emblems")
set(_ECM_ICON_GROUP_emotes     "emotes")
set(_ECM_ICON_GROUP_animations "animations")
set(_ECM_ICON_GROUP_intl       "intl")

# For the "compatibility" syntax: a "map" of short theme names to the theme
# directory
set(_ECM_ICON_THEME_br "breeze")
set(_ECM_ICON_THEME_ox "oxygen")
set(_ECM_ICON_THEME_cr "crystalsvg")
set(_ECM_ICON_THEME_lo "locolor")
set(_ECM_ICON_THEME_hi "hicolor")

macro(_ecm_install_icons_v1 _defaultpath)
   # the l10n-subdir if language given as second argument (localized icon)
   set(_lang ${ARGV1})
   if(_lang)
      set(_l10n_SUBDIR l10n/${_lang})
   else()
      set(_l10n_SUBDIR ".")
   endif()

   set(_themes)

   # first the png icons
   file(GLOB _icons *.png)
   foreach (_current_ICON ${_icons} )
      # since CMake 2.6 regex matches are stored in special variables CMAKE_MATCH_x, if it didn't match, they are empty
      string(REGEX MATCH "^.*/([a-zA-Z]+)([0-9]+)\\-([a-z]+)\\-(.+\\.png)$" _dummy  "${_current_ICON}")
      set(_type  "${CMAKE_MATCH_1}")
      set(_size  "${CMAKE_MATCH_2}")
      set(_group "${CMAKE_MATCH_3}")
      set(_name  "${CMAKE_MATCH_4}")

      set(_theme_GROUP ${_ECM_ICON_THEME_${_type}})
      if( _theme_GROUP)
         list(APPEND _themes "${_theme_GROUP}")
         _ECM_ADD_ICON_INSTALL_RULE(${CMAKE_CURRENT_BINARY_DIR}/install_icons.cmake
                    ${_defaultpath}/${_theme_GROUP}/${_size}x${_size}
                    ${_group} ${_current_ICON} ${_name} ${_l10n_SUBDIR})
      endif()
   endforeach (_current_ICON)

   # mng icons
   file(GLOB _icons *.mng)
   foreach (_current_ICON ${_icons} )
      # since CMake 2.6 regex matches are stored in special variables CMAKE_MATCH_x, if it didn't match, they are empty
      string(REGEX MATCH "^.*/([a-zA-Z]+)([0-9]+)\\-([a-z]+)\\-(.+\\.mng)$" _dummy  "${_current_ICON}")
      set(_type  "${CMAKE_MATCH_1}")
      set(_size  "${CMAKE_MATCH_2}")
      set(_group "${CMAKE_MATCH_3}")
      set(_name  "${CMAKE_MATCH_4}")

      set(_theme_GROUP ${_ECM_ICON_THEME_${_type}})
      if( _theme_GROUP)
         list(APPEND _themes "${_theme_GROUP}")
         _ECM_ADD_ICON_INSTALL_RULE(${CMAKE_CURRENT_BINARY_DIR}/install_icons.cmake
                ${_defaultpath}/${_theme_GROUP}/${_size}x${_size}
                ${_group} ${_current_ICON} ${_name} ${_l10n_SUBDIR})
      endif()
   endforeach (_current_ICON)

   # and now the svg icons
   file(GLOB _icons *.svgz)
   foreach (_current_ICON ${_icons} )
      # since CMake 2.6 regex matches are stored in special variables CMAKE_MATCH_x, if it didn't match, they are empty
      string(REGEX MATCH "^.*/([a-zA-Z]+)sc\\-([a-z]+)\\-(.+\\.svgz)$" _dummy "${_current_ICON}")
      set(_type  "${CMAKE_MATCH_1}")
      set(_group "${CMAKE_MATCH_2}")
      set(_name  "${CMAKE_MATCH_3}")

      set(_theme_GROUP ${_ECM_ICON_THEME_${_type}})
      if( _theme_GROUP)
         list(APPEND _themes "${_theme_GROUP}")
          _ECM_ADD_ICON_INSTALL_RULE(${CMAKE_CURRENT_BINARY_DIR}/install_icons.cmake
                            ${_defaultpath}/${_theme_GROUP}/scalable
                            ${_group} ${_current_ICON} ${_name} ${_l10n_SUBDIR})
      endif()
   endforeach (_current_ICON)

   if (_themes)
       list(REMOVE_DUPLICATES _themes)
       foreach(_theme ${_themes})
           _ecm_update_iconcache("${_defaultpath}" "${_theme}")
       endforeach()
   else()
       message(AUTHOR_WARNING "No suitably-named icons found")
   endif()

endmacro()

# only used internally by _ecm_install_icons_v1
macro(_ecm_add_icon_install_rule _install_SCRIPT _install_PATH _group _orig_NAME _install_NAME _l10n_SUBDIR)

   # if the string doesn't match the pattern, the result is the full string, so all three have the same content
   if (NOT ${_group} STREQUAL ${_install_NAME} )
      set(_icon_GROUP  ${_ECM_ICON_GROUP_${_group}})
      if(NOT _icon_GROUP)
         message(WARNING "Icon ${_install_NAME} uses invalid category ${_group}, setting to 'actions'")
         set(_icon_GROUP "actions")
      endif()
#      message(STATUS "icon: ${_current_ICON} size: ${_size} group: ${_group} name: ${_name} l10n: ${_l10n_SUBDIR}")
      install(FILES ${_orig_NAME} DESTINATION ${_install_PATH}/${_icon_GROUP}/${_l10n_SUBDIR}/ RENAME ${_install_NAME} )
   endif (NOT ${_group} STREQUAL ${_install_NAME} )

endmacro()

# Updates the mtime of the icon theme directory, so caches that
# watch for changes to the directory will know to update.
# If present, this also runs gtk-update-icon-cache (which despite the name is also used by Qt).
function(_ecm_update_iconcache installdir theme)
    find_program(GTK_UPDATE_ICON_CACHE_EXECUTABLE NAMES gtk-update-icon-cache)
    # We don't always have touch command (e.g. on Windows), so instead
    # create and delete a temporary file in the theme dir.
    install(CODE "
    set(DESTDIR_VALUE \"\$ENV{DESTDIR}\")
    if (NOT DESTDIR_VALUE)
        execute_process(COMMAND \"${CMAKE_COMMAND}\" -E touch \"${CMAKE_INSTALL_PREFIX}/${installdir}/${theme}\")
        set(HAVE_GTK_UPDATE_ICON_CACHE_EXEC ${GTK_UPDATE_ICON_CACHE_EXECUTABLE})
        if (HAVE_GTK_UPDATE_ICON_CACHE_EXEC)
            execute_process(COMMAND ${GTK_UPDATE_ICON_CACHE_EXECUTABLE} -q -t -i . WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}/${installdir}/${theme}\")
        endif ()
    endif (NOT DESTDIR_VALUE)
    ")
endfunction()

function(ecm_install_icons)
    set(options)
    set(oneValueArgs DESTINATION LANG THEME)
    set(multiValueArgs ICONS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARG_ICONS AND NOT ARG_DESTINATION)
        message(AUTHOR_WARNING "ecm_install_icons() with no ICONS argument is deprecated")
        _ecm_install_icons_v1(${ARGN})
        return()
    endif()
    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unexpected arguments to ecm_install_icons: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    if(NOT ARG_DESTINATION)
        message(FATAL_ERROR "No DESTINATION argument given to ecm_install_icons")
    endif()
    if(NOT ARG_THEME)
        set(ARG_THEME "hicolor")
    endif()
    if(ARG_LANG)
        set(l10n_subdir "l10n/${ARG_LANG}/")
    endif()

    foreach(icon ${ARG_ICONS})
        get_filename_component(filename "${icon}" NAME)
        string(REGEX MATCH "([0-9sc]+)\\-([a-z]+)\\-([^/]+)\\.([a-z]+)$"
                           complete_match "${filename}")
        set(size  "${CMAKE_MATCH_1}")
        set(group "${CMAKE_MATCH_2}")
        set(name  "${CMAKE_MATCH_3}")
        set(ext   "${CMAKE_MATCH_4}")
        if(NOT size OR NOT group OR NOT name OR NOT ext)
            message(WARNING "${icon} is not named correctly for ecm_install_icons - ignoring")
        elseif(NOT size STREQUAL "sc" AND NOT size GREATER 0)
            message(WARNING "${icon} size (${size}) is invalid - ignoring")
        else()
            if (NOT complete_match STREQUAL filename)
                # We can't stop accepting filenames with leading characters,
                # because that would break existing projects, so just warn
                # about them instead.
                message(AUTHOR_WARNING "\"${icon}\" has characters before the size; it should be renamed to \"${size}-${group}-${name}.${ext}\"")
            endif()
            if(NOT _ECM_ICON_GROUP_${group})
                message(WARNING "${icon} group (${group}) is not recognized")
            endif()
            if(size STREQUAL "sc")
                if(NOT ext STREQUAL "svg" AND NOT ext STREQUAL "svgz")
                    message(WARNING "Scalable icon ${icon} is not SVG or SVGZ")
                endif()
                set(size_dir "scalable")
            else()
                if(NOT ext STREQUAL "png" AND NOT ext STREQUAL "mng" AND NOT ext STREQUAL "svg" AND NOT ext STREQUAL "svgz")
                    message(WARNING "Fixed-size icon ${icon} is not PNG/MNG/SVG/SVGZ")
                endif()
                set(size_dir "${size}x${size}")
            endif()
            install(
                FILES "${icon}"
                DESTINATION "${ARG_DESTINATION}/${ARG_THEME}/${size_dir}/${group}/${l10n_subdir}"
                RENAME "${name}.${ext}"
            )
        endif()
    endforeach()
    _ecm_update_iconcache("${ARG_DESTINATION}" "${ARG_THEME}")
endfunction()
