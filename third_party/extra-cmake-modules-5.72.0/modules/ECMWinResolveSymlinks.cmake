#.rst:
# ECMWinResolveSymlinks
# --------------------------
#
# Resolve pseudo-symlinks created by git when cloning on Windows.
#
# ::
#
#   ecm_win_resolve_symlinks(<dir>)
#
# When git checks out a repository with UNIX symlinks on Windows machine,
# it creates a text file for each symlink, containing a relative path to the
# real file.
# This function would recursively walk over specified directory and replace
# pseudo-symlinks with corresponding real file's contents. It would then run
# git update-index --assume-unchanged on them to trick git.
#
# This is useful for projects like "breeze-icons" that contain many identical
# icons implemented as symlinks.
#
# Since 5.28

#=============================================================================
# SPDX-FileCopyrightText: 2016 Gleb Popov <6yearold@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================
# (To distribute this file outside of extra-cmake-modules, substitute the full
#  License text for the above reference.)

function(ECM_WIN_RESOLVE_SYMLINKS _dir)
  get_filename_component(dir ${_dir} ABSOLUTE)
  find_program(GIT_EXECUTABLE git)
  if(NOT GIT_EXECUTABLE)
    message(SEND_ERROR "Git executable not found!")
  endif()

  message(STATUS "Resolving symlinks in ${dir}...")
  _find_symlinks(${dir} symlinks)
  _portioned_list(symlinks ${symlinks})
  foreach(s IN LISTS symlinks)
    string(REPLACE ":" ";" s ${s})
    _assume_unchanged(NO ${dir} "${s}")
    _checkout_symlinks(${dir} "${s}")
    _resolve_symlinks(${dir} "${s}")
    _assume_unchanged(YES ${dir} "${s}")
  endforeach()
  message(STATUS "Resolving symlinks in ${dir}... Done.")

  # touch cache every build to force CMake to re-run these functions every time
  if(NOT TARGET wrs_touch_cache)
    add_custom_target(wrs_touch_cache ALL
      COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/CMakeCache.txt
    )
  endif()
endfunction()

function(_assume_unchanged mode dir symlinks)
  if(mode)
    set(flag --assume-unchanged)
  else()
    set(flag --no-assume-unchanged)
  endif()

  execute_process(COMMAND ${GIT_EXECUTABLE} update-index ${flag} ${symlinks}
    WORKING_DIRECTORY ${dir}
  )
endfunction()

function(_find_symlinks dir outvar)
  execute_process(COMMAND ${GIT_EXECUTABLE} ls-files -s
    WORKING_DIRECTORY ${dir}
    OUTPUT_VARIABLE out
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  set(${outvar})
  if(out)
    string(REPLACE "\n" ";" out ${out})

    foreach(f IN LISTS out)
      # 120000 0db97052931e18484b6705f3bc644484ef9403b0 0 <tab> icons-dark/actions/16/CVnamespace.svg
      string(REPLACE "\t" ";" f "${f}")
      string(REPLACE " " ";" f "${f}")
      list(GET f 0 mode)
      if(mode STREQUAL "120000")
        list(GET f 3 symlink)
        list(APPEND ${outvar} ${symlink})
      endif()
    endforeach()
  endif()
  set(${outvar} ${${outvar}} PARENT_SCOPE)
endfunction()

# In functions like _checkout_symlinks() the command line can become too lengthy for Windows.
# So we partition it, but in a hacky way due to CMake doesn't have list of lists.
function(_portioned_list outvar)
  list(LENGTH ARGN arglen)

  if(arglen EQUAL 0)
    set(${outvar} "" PARENT_SCOPE)
    return()
  endif()

  set(init)
  set(tail)
  math(EXPR range "${arglen} - 1")
  foreach(i RANGE ${range})
    list(GET ARGN ${i} v)
    string(LENGTH "${init}" initlen)
    string(LENGTH ${v} vlen)
    math(EXPR sumlen "${initlen} + ${vlen}")
    if(sumlen LESS 8192)
      list(APPEND init ${v})
    else()
      list(APPEND tail ${v})
    endif()
  endforeach()

  _portioned_list(tail_portioned ${tail})
  string(REPLACE ";" ":" init "${init}") # Generally this is not safe, because filepath can contain ':' character. But not on Windows. Phew.
  set(${outvar} ${init} ${tail_portioned} PARENT_SCOPE)
endfunction()

function(_checkout_symlinks dir symlinks)
  execute_process(COMMAND ${GIT_EXECUTABLE} checkout ${symlinks}
    WORKING_DIRECTORY ${dir}
  )
endfunction()

function(_resolve_symlinks dir symlinks)
  foreach(s IN LISTS symlinks)
    file(READ ${dir}/${s} sdata)
    get_filename_component(sdir ${dir}/${s} DIRECTORY)
    set(f "${sdir}/${sdata}")
    file(READ ${f} fdata)
    file(WRITE ${dir}/${s} ${fdata})
  endforeach()
endfunction()
