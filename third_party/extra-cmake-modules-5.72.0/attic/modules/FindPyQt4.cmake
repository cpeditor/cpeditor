# Find PyQt4
# ~~~~~~~~~~
# Copyright (c) 2007-2008, Simon Edwards <simon@simonzone.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# PyQt4 website: https://riverbankcomputing.com/software/pyqt
#
# Find the installed version of PyQt4. FindPyQt4 should only be called after
# Python has been found.
#
# This file defines the following variables, which can also be overridden by
# users:
#
# PYQT4_VERSION - The version of PyQt4 found expressed as a 6 digit hex number
#     suitable for comparison as a string
#
# PYQT4_VERSION_STR - The version of PyQt4 as a human readable string.
#
# PYQT4_VERSION_TAG - The Qt4 version tag used by PyQt's sip files.
#
# PYQT4_SIP_DIR - The directory holding the PyQt4 .sip files. This can be unset
# if PyQt4 was built using its new build system and pyqtconfig.py is not
# present on the system, as in this case its value cannot be determined
# automatically.
#
# PYQT4_SIP_FLAGS - The SIP flags used to build PyQt.

IF(EXISTS PYQT4_VERSION)
  # Already in cache, be silent
  SET(PYQT4_FOUND TRUE)
ELSE(EXISTS PYQT4_VERSION)

  FIND_FILE(_find_pyqt_py FindPyQt.py PATHS ${CMAKE_MODULE_PATH})

  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} ${_find_pyqt_py} OUTPUT_VARIABLE pyqt_config)
  IF(pyqt_config)
    STRING(REGEX MATCH "^pyqt_version:([^\n]+).*$" _dummy ${pyqt_config})
    SET(PYQT4_VERSION "${CMAKE_MATCH_1}" CACHE STRING "PyQt4's version as a 6-digit hexadecimal number")

    STRING(REGEX MATCH ".*\npyqt_version_str:([^\n]+).*$" _dummy ${pyqt_config})
    SET(PYQT4_VERSION_STR "${CMAKE_MATCH_1}" CACHE STRING "PyQt4's version as a human-readable string")

    STRING(REGEX MATCH ".*\npyqt_version_tag:([^\n]+).*$" _dummy ${pyqt_config})
    SET(PYQT4_VERSION_TAG "${CMAKE_MATCH_1}" CACHE STRING "The Qt4 version tag used by PyQt4's .sip files")

    STRING(REGEX MATCH ".*\npyqt_sip_dir:([^\n]+).*$" _dummy ${pyqt_config})
    SET(PYQT4_SIP_DIR "${CMAKE_MATCH_1}" CACHE PATH "The base directory where PyQt4's .sip files are installed")

    STRING(REGEX MATCH ".*\npyqt_sip_flags:([^\n]+).*$" _dummy ${pyqt_config})
    SET(PYQT4_SIP_FLAGS "${CMAKE_MATCH_1}" CACHE STRING "The SIP flags used to build PyQt4")

    IF(NOT IS_DIRECTORY "${PYQT4_SIP_DIR}")
      MESSAGE(WARNING "The base directory where PyQt4's SIP files are installed could not be determined. This usually means PyQt4 was built with its new build system and pyqtconfig.py is not present.\n"
                      "Please set the PYQT4_SIP_DIR variable manually.")
    ELSE(NOT IS_DIRECTORY "${PYQT4_SIP_DIR}")
      SET(PYQT4_FOUND TRUE)
    ENDIF(NOT IS_DIRECTORY "${PYQT4_SIP_DIR}")
  ENDIF(pyqt_config)

  IF(PYQT4_FOUND)
    IF(NOT PYQT4_FIND_QUIETLY)
      MESSAGE(STATUS "Found PyQt4 version: ${PYQT4_VERSION_STR}")
    ENDIF(NOT PYQT4_FIND_QUIETLY)
  ELSE(PYQT4_FOUND)
    IF(PYQT4_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Python")
    ENDIF(PYQT4_FIND_REQUIRED)
  ENDIF(PYQT4_FOUND)

ENDIF(EXISTS PYQT4_VERSION)
