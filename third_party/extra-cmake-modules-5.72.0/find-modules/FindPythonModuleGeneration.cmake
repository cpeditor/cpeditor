#.rst:
# FindPythonModuleGeneration
# --------------------------
#
# This module is experimental and internal.  The interface will likely
# change in the coming releases.
#
# Tools and macros for generating python bindings
#
# This will define the following public function:
#
#   ecm_generate_python_binding(TARGET <target>
#                               PYTHONNAMESPACE <namespace>
#                               MODULENAME <modulename>
#                               RULES_FILE <rulesfile>
#                               SIP_DEPENDS <dependencies>
#                               SIP_INCLUDES <includes>
#                               HEADERS <headers>)
#
# Invoking the function will create bindings for the <target> for python 2 and 3,
# if available.  The bindings will be put in the namespace <namespace> in python,
# and will be available from the module <modulename>.
#
# The optional rules file specifies the rules for creating the bindings
#
# A simple invocation would be:
#
#   ecm_generate_python_binding(TARGET KMyTarget
#     PYTHONNAMESPACE PyKF5
#     MODULENAME MyTarget
#     SIP_DEPENDS QtCore/QtCoremod.sip
#     HEADERS ${myTargetHeaders}
#   )
#
# which can then be used from python as
#
#   import PyKF5.MyTarget
#
# Inclusion of this module defines the following variables:
#
# ``KDE_INSTALL_PYTHON2DIR``, ``KDE_INSTALL_PYTHON3DIR``
#     destination for generated bindings
# ``KDE_INSTALL_FULL_PYTHON2DIR``, ``KDE_INSTALL_FULL_PYTHON3DIR``
#     corresponding absolute path
#
# If ``KDE_INSTALL_USE_PYTHON2_SYS_PATHS`` is set to TRUE before including this
# module, the default value for ``KDE_INSTALL_PYTHON2DIR`` is instead queried from
# pythons distutil.sysconfig.get_python_lib().
# If not set, it will default to TRUE if pythons ``sysconfig.PREFIX`` is the same
# as ``CMAKE_INSTALL_PREFIX``, otherwise it defaults to FALSE.
# This variable should NOT be set from within CMakeLists.txt files, instead it
# is intended to be set manually when configuring a project which uses this
# module (e.g. by packagers).
#
# Likewise for ``KDE_INSTALL_USE_PYTHON3_SYS_PATHS`` and ``KDE_INSTALL_PYTHON3DIR``.
#

#=============================================================================
# SPDX-FileCopyrightText: 2016 Stephen Kelly <steveire@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================


macro(_find_python version minor_version)
  set(_CURRENT_VERSION ${version}.${minor_version})
  find_library(GPB_PYTHON${version}_LIBRARY
    NAMES
      python${_CURRENT_VERSION}mu
      python${_CURRENT_VERSION}m
      python${_CURRENT_VERSION}u
      python${_CURRENT_VERSION}
    PATHS
      [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\${_CURRENT_VERSION}\\InstallPath]/libs
      [HKEY_CURRENT_USER\\SOFTWARE\\Python\\PythonCore\\${_CURRENT_VERSION}\\InstallPath]/libs
    # Avoid finding the .dll in the PATH.  We want the .lib.
    NO_SYSTEM_ENVIRONMENT_PATH
  )

  if(GPB_PYTHON${version}_LIBRARY)
    # Use the library's install prefix as a hint
    set(_Python_INCLUDE_PATH_HINT)
    get_filename_component(_Python_PREFIX ${GPB_PYTHON${version}_LIBRARY} PATH)
    get_filename_component(_Python_PREFIX ${_Python_PREFIX} PATH)
    if(_Python_PREFIX)
      set(_Python_INCLUDE_PATH_HINT ${_Python_PREFIX}/include)
    endif()
    unset(_Python_PREFIX)

    find_path(GPB_PYTHON${version}_INCLUDE_DIR
      NAMES Python.h
      HINTS
        ${_Python_INCLUDE_PATH_HINT}
      PATHS
        [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\${_CURRENT_VERSION}\\InstallPath]/include
        [HKEY_CURRENT_USER\\SOFTWARE\\Python\\PythonCore\\${_CURRENT_VERSION}\\InstallPath]/include
      PATH_SUFFIXES
        python${_CURRENT_VERSION}mu
        python${_CURRENT_VERSION}m
        python${_CURRENT_VERSION}u
        python${_CURRENT_VERSION}
    )
    # sip.h is version agnostic, it may be located in the version specific path
    # or one of the default locations
    find_path(GPB_PYTHON${version}_SIP_DIR
       NAMES sip.h
       PATHS
         ${GPB_PYTHON${version}_INCLUDE_DIR}
    )
  endif()

  find_program(GPB_PYTHON${version}_COMMAND python${version})
endmacro()

macro(_create_imported_python_target version)
  if(GPB_PYTHON${version}_LIBRARY AND GPB_PYTHON${version}_INCLUDE_DIR AND
     EXISTS "${GPB_PYTHON${version}_INCLUDE_DIR}/patchlevel.h" AND
     EXISTS "${GPB_PYTHON${version}_SIP_DIR}/sip.h")
    list(APPEND _pyversions ${version})

    file(STRINGS "${GPB_PYTHON${version}_INCLUDE_DIR}/patchlevel.h" python_version_define
         REGEX "^#define[ \t]+PY_MINOR_VERSION[ \t]+[0-9]+")
    string(REGEX REPLACE "^#define[ \t]+PY_MINOR_VERSION[ \t]+([0-9]+)" "\\1"
                         min_ver "${python_version_define}")
    unset(python_version_define)

    set(pyversion${version}_maj_min ${version}.${min_ver})

    add_library(Python::Libs${version} UNKNOWN IMPORTED)
    set_property(TARGET Python::Libs${version} PROPERTY IMPORTED_LOCATION ${GPB_PYTHON${version}_LIBRARY})
    set_property(TARGET Python::Libs${version} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GPB_PYTHON${version}_INCLUDE_DIR})

    set(_default_KDE_INSTALL_USE_PYTHON${version}_SYS_PATHS OFF)
    if(NOT DEFINED KDE_INSTALL_USE_PYTHON${version}_SYS_PATHS)
      execute_process (
        COMMAND "${GPB_PYTHON${version}_COMMAND}" -c "import sys; from distutils import sysconfig;sys.stdout.write(sysconfig.PREFIX)"
        OUTPUT_VARIABLE gpb_python${version}_prefix
      )
      if(gpb_python${version}_prefix STREQUAL "${CMAKE_INSTALL_PREFIX}")
        message(STATUS "Installing in the same prefix as Python${version}, adopting their path scheme.")
        set(_default_KDE_INSTALL_USE_PYTHON${version}_SYS_PATHS ON)
      else()
        message(STATUS "NOT installing in the same prefix as Python${version}.")
      endif()
    endif()

  endif()
endmacro()

macro(_report_NOT_FOUND message)
  if(NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
    set(_GPB_MESSAGE_TYPE STATUS)
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
      set(_GPB_MESSAGE_TYPE FATAL_ERROR)
    endif()
    message(${_GPB_MESSAGE_TYPE} ${message})
  endif()
  set(PythonModuleGeneration_FOUND FALSE)
  return()
endmacro()

if (NOT Qt5Core_FOUND)
  _report_NOT_FOUND("Qt 5 must be found before finding ${CMAKE_FIND_PACKAGE_NAME}.")
endif()

if (NOT GPB_PYTHON3_LIBRARY)
  set(_PYTHON3_MIN_VERSION 4)
  set(_PYTHON3_MAX_VERSION 10)

  _find_python(3 ${_PYTHON3_MAX_VERSION}) # Canary check

  if (GPB_PYTHON3_LIBRARY)
    message(FATAL_ERROR "The max python version in ${CMAKE_FIND_PACKAGE_NAME} must be updated.")
  endif()

  set(_PYTHON3_FIND_VERSION ${_PYTHON3_MAX_VERSION})

  while(NOT GPB_PYTHON3_LIBRARY
      AND NOT GPB_PYTHON3_INCLUDE_DIR
      AND NOT EXISTS "${GPB_PYTHON3_INCLUDE_DIR}/patchlevel.h"
      AND NOT _PYTHON3_FIND_VERSION EQUAL ${_PYTHON3_MIN_VERSION})
    math(EXPR _PYTHON3_FIND_VERSION "${_PYTHON3_FIND_VERSION} - 1")
    _find_python(3 ${_PYTHON3_FIND_VERSION})
  endwhile()
endif()
_create_imported_python_target(3)

_find_python(2 7)
_create_imported_python_target(2)

option (KDE_INSTALL_USE_PYTHON3_SYS_PATHS "Install Python3 bindings to the Python3 install dir"
        "${_default_KDE_INSTALL_USE_PYTHON3_SYS_PATHS}")
option (KDE_INSTALL_USE_PYTHON2_SYS_PATHS "Install Python2 bindings to the Python2 install dir"
        "${_default_KDE_INSTALL_USE_PYTHON2_SYS_PATHS}")

if (NOT _pyversions)
  _report_NOT_FOUND("At least one python version must be available to use ${CMAKE_FIND_PACKAGE_NAME}.")
endif()

foreach(pyversion ${_pyversions})

  if(KDE_INSTALL_PYTHON${pyversion}DIR)
     # Use dir from command line

  elseif(KDE_INSTALL_USE_PYTHON${pyversion}_SYS_PATHS)
    execute_process (
      COMMAND "${GPB_PYTHON${pyversion}_COMMAND}" -c "import sys; from distutils import sysconfig;sys.stdout.write(sysconfig.get_python_lib(plat_specific=True,standard_lib=False))"
      OUTPUT_VARIABLE KDE_INSTALL_PYTHON${pyversion}DIR
    )

  else()
    set(KDE_INSTALL_PYTHON${pyversion}DIR lib/python${pyversion${pyversion}_maj_min}/site-packages)
  endif()

  if(NOT IS_ABSOLUTE ${KDE_INSTALL_PYTHON${pyversion}DIR})
    set(KDE_INSTALL_FULL_PYTHON${pyversion}DIR
        "${CMAKE_INSTALL_PREFIX}/${KDE_INSTALL_PYTHON${pyversion}DIR}")
  else()
    set(KDE_INSTALL_FULL_PYTHON${pyversion}DIR "${KDE_INSTALL_PYTHON${pyversion}DIR}")
  endif()

endforeach()

find_program(GBP_SIP_COMMAND sip)

if (NOT GBP_SIP_COMMAND)
  _report_NOT_FOUND("The sip executable must be available to use ${CMAKE_FIND_PACKAGE_NAME}.")
endif()

if (NOT libclang_LIBRARY)
  set(_LIBCLANG_MAX_MAJOR_VERSION 7)
  set(_LIBCLANG_MIN_MAJOR_VERSION 5)

  find_library(libclang_LIBRARY clang-${_LIBCLANG_MAX_MAJOR_VERSION}.0)

  if (libclang_LIBRARY)
    message(FATAL_ERROR "The max libclang version in ${CMAKE_FIND_PACKAGE_NAME} must be updated.")
  endif()

  set(_LIBCLANG_FIND_VERSION ${_LIBCLANG_MAX_MAJOR_VERSION})
  while(NOT libclang_LIBRARY AND NOT _LIBCLANG_FIND_VERSION EQUAL _LIBCLANG_MIN_MAJOR_VERSION)
    math(EXPR _LIBCLANG_FIND_VERSION "${_LIBCLANG_FIND_VERSION} - 1")
    set(_GPB_CLANG_SUFFIX ${_LIBCLANG_FIND_VERSION}.0)
    find_library(libclang_LIBRARY clang-${_LIBCLANG_FIND_VERSION}.0)
  endwhile()

  if (NOT libclang_LIBRARY)
    find_library(libclang_LIBRARY clang)
  endif()
else()
  string(REGEX MATCH ".*clang-([0-9]+\\.[0-9]+).*" _GPB_CLANG_SUFFIX ${libclang_LIBRARY})
  set(_GPB_CLANG_SUFFIX ${CMAKE_MATCH_1})
endif()

if (NOT libclang_LIBRARY)
  _report_NOT_FOUND("Could not find libclang version 5.0 or greater.")
else()
  message(STATUS "Found ${libclang_LIBRARY}")
endif()

foreach (pyversion "2" "3")
  set(GPB_PYTHON_COMMAND ${GPB_PYTHON${pyversion}_COMMAND})
  if (NOT GPB_PYTHON_COMMAND)
    continue()
  endif()
  message(STATUS "Testing if ${GPB_PYTHON_COMMAND} can be used to run sip_generator")

  execute_process(
    COMMAND ${GPB_PYTHON_COMMAND} ${CMAKE_CURRENT_LIST_DIR}/sip_generator.py --self-check ${libclang_LIBRARY}
    RESULT_VARIABLE selfCheckErrors
    ERROR_QUIET
  )

  if (selfCheckErrors)
    message(STATUS "sip_generator self-check for ${GPB_PYTHON_COMMAND} failed")
    unset(GPB_PYTHON_COMMAND)
  else()
    message(STATUS "Self-check passed, Using ${GPB_PYTHON_COMMAND} to generate bindings")
    break()
  endif()
endforeach()

if (NOT GPB_PYTHON_COMMAND)
   _report_NOT_FOUND("No usable python version found to run sip_generator for the ${CMAKE_FIND_PACKAGE_NAME} Module.")
endif()

get_filename_component(libclang_file "${libclang_file}" REALPATH)

find_file(SIP_Qt5Core_Mod_FILE
  NAMES QtCoremod.sip
  HINTS ${KDE_INSTALL_PYTHON3DIR}/PyQt5/bindings/QtCore
  PATH_SUFFIXES share/sip/PyQt5/QtCore
)

if(NOT SIP_Qt5Core_Mod_FILE)
  _report_NOT_FOUND("PyQt module files not found for the ${CMAKE_FIND_PACKAGE_NAME} Module.")
endif()

set(PythonModuleGeneration_FOUND TRUE)

include(CMakeParseArguments)

set(GPB_MODULE_DIR ${CMAKE_CURRENT_LIST_DIR})

function(_compute_implicit_include_dirs)
  find_program(_GBP_CLANG_CXX_DRIVER_PATH NAMES clang++-${_GPB_CLANG_SUFFIX} clang++)
  if (NOT _GBP_CLANG_CXX_DRIVER_PATH)
    message(FATAL_ERROR "Failed to find clang driver corresponding to ${libclang_LIBRARY}")
  endif()
  execute_process(COMMAND ${_GBP_CLANG_CXX_DRIVER_PATH} -v -E -x c++ -
                  ERROR_VARIABLE _compilerOutput
                  OUTPUT_VARIABLE _compilerStdout
                  INPUT_FILE /dev/null)

  if( "${_compilerOutput}" MATCHES "> search starts here[^\n]+\n *(.+ *\n) *End of (search) list" )

    # split the output into lines and then remove leading and trailing spaces from each of them:
    string(REGEX MATCHALL "[^\n]+\n" _includeLines "${CMAKE_MATCH_1}")
    foreach(nextLine ${_includeLines})
      # on OSX, gcc says things like this:  "/System/Library/Frameworks (framework directory)", strip the last part
      string(REGEX REPLACE "\\(framework directory\\)" "" nextLineNoFramework "${nextLine}")
      # strip spaces at the beginning and the end
      string(STRIP "${nextLineNoFramework}" _includePath)
      list(APPEND _resultIncludeDirs "${_includePath}")
    endforeach()
  endif()

  set(_GPB_IMPLICIT_INCLUDE_DIRS ${_resultIncludeDirs} PARENT_SCOPE)
endfunction()

function(ecm_generate_python_binding)

    set(oneValueArgs TARGET PYTHONNAMESPACE MODULENAME RULES_FILE)
    set(multiValueArgs SIP_DEPENDS SIP_INCLUDES HEADERS)

    cmake_parse_arguments(GPB "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/sip/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/${GPB_MODULENAME}mod.sip"
          "
%Module ${GPB_PYTHONNAMESPACE}.${GPB_MODULENAME}

%ModuleHeaderCode
#pragma GCC visibility push(default)
%End\n\n")

    set(generator_depends "${GPB_MODULE_DIR}/sip_generator.py"
        "${GPB_MODULE_DIR}/rules_engine.py"
        "${GPB_MODULE_DIR}/FindPythonModuleGeneration.cmake"
        )

    if (NOT _GPB_IMPLICIT_INCLUDE_DIRS)
      _compute_implicit_include_dirs()
    endif()

    foreach(dep ${GPB_SIP_DEPENDS})
        if (IS_ABSOLUTE ${dep})
          list(APPEND generator_depends "${dep}")
        endif()
        file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/sip/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/${GPB_MODULENAME}mod.sip"
          "%Import ${dep}\n\n")
    endforeach()

    set(sip_files)
    set(commands)

    if (NOT GPB_RULES_FILE)
      set(GPB_RULES_FILE "${GPB_MODULE_DIR}/Qt5Ruleset.py")
    endif()

    list(APPEND generator_depends ${GPB_RULES_FILE})

    foreach(hdr ${GPB_HEADERS})

        get_filename_component(hdr_file ${hdr} ABSOLUTE)

        get_filename_component(hdr ${hdr} NAME_WE)
        string(TOLOWER ${hdr}.h hdr_filename)

        if (NOT EXISTS "${hdr_file}")
          message(FATAL_ERROR "File not found: ${hdr_file}")
        endif()

        set(sip_file "${CMAKE_CURRENT_BINARY_DIR}/sip/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/${hdr}.sip")
        list(APPEND sip_files ${sip_file})

        set(inc_dirs "-I$<JOIN:$<TARGET_PROPERTY:${GPB_TARGET},INTERFACE_INCLUDE_DIRECTORIES>,;-I>")
        set(sys_inc_dirs)
        foreach(d ${_GPB_IMPLICIT_INCLUDE_DIRS})
           list(APPEND sys_inc_dirs "-isystem" "${d}")
        endforeach()
        set(comp_defs "-D$<JOIN:$<TARGET_PROPERTY:${GPB_TARGET},INTERFACE_COMPILE_DEFINITIONS>,;-D>")

        # We might like to use $<TARGET_PROPERTY:${GPB_TARGET},CXX_STANDARD>, but
        # unfortunately CMake does not populate that property as a side-effect of evaluating
        # COMPILE_FEATURES (Qt specifies feature requirements in its INTERFACE_COMPILE_FEATURES, and
        # those are consumed to set the CXX_STANDARD internally in CMake, but evidently too late)
        set(stdFlag "-std=gnu++14")

        set(comp_flags "$<JOIN:$<TARGET_PROPERTY:${GPB_TARGET},INTERFACE_COMPILE_OPTIONS>;${stdFlag},;>")

        add_custom_command(OUTPUT ${sip_file}
            COMMAND ${GPB_PYTHON_COMMAND} ${GPB_MODULE_DIR}/sip_generator.py
              --flags " ${inc_dirs};${sys_inc_dirs};${comp_defs};${comp_flags}"
              --include_filename "${hdr_filename}"
              ${libclang_LIBRARY}
              ${GPB_RULES_FILE}
              "${hdr_file}"
              "${sip_file}"
            DEPENDS ${hdr_file} ${generator_depends}
            VERBATIM
        )

        file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/sip/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/${GPB_MODULENAME}mod.sip"
          "%Include ${hdr}.sip\n")
    endforeach()

    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/pybuild/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/module.sbf"
        "
target = ${GPB_MODULENAME}
sources = sip${GPB_MODULENAME}cmodule.cpp
headers = sipAPI${GPB_MODULENAME}
"
    )

    get_filename_component(SIP_PYQT5_DIR ${SIP_Qt5Core_Mod_FILE} PATH)
    get_filename_component(SIP_PYQT5_DIR ${SIP_PYQT5_DIR} PATH)

    set(sip_includes -I "${SIP_PYQT5_DIR}")
    if (GPB_SIP_INCLUDES)
      list(APPEND sip_includes -I "$<JOIN:${GPB_SIP_INCLUDES},-I>")
    endif()
    foreach(path ${CMAKE_PREFIX_PATH} ${CMAKE_INSTALL_PREFIX})
      if (EXISTS ${path}/share/sip)
        list(APPEND sip_includes -I "${path}/share/sip")
      endif()
    endforeach()

    add_custom_target(generate_${GPB_MODULENAME}_sip_files ALL DEPENDS ${sip_files})

    add_custom_command(OUTPUT
      "${CMAKE_CURRENT_BINARY_DIR}/pybuild/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/unified${GPB_MODULENAME}.cpp"
      COMMAND ${GPB_PYTHON_COMMAND} "${GPB_MODULE_DIR}/run-sip.py" --sip ${GBP_SIP_COMMAND}
       --unify "${CMAKE_CURRENT_BINARY_DIR}/pybuild/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/unified${GPB_MODULENAME}.cpp"
       --module-name "${GPB_MODULENAME}"
       -c "${CMAKE_CURRENT_BINARY_DIR}/pybuild/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}"
       -b "${CMAKE_CURRENT_BINARY_DIR}/pybuild/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/module.sbf"

       -x VendorID -x Py_v3

       -I "${CMAKE_CURRENT_BINARY_DIR}/sip/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}"
       ${sip_includes}
       "${CMAKE_CURRENT_BINARY_DIR}/sip/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/${GPB_MODULENAME}mod.sip"
       DEPENDS generate_${GPB_MODULENAME}_sip_files "${GPB_MODULE_DIR}/run-sip.py" ${generator_depends}
    )

    add_custom_target(sip_generated_${GPB_MODULENAME}_files ALL
          DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/pybuild/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/unified${GPB_MODULENAME}.cpp")

    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/sip/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}"
         "${CMAKE_CURRENT_BINARY_DIR}/pybuild/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}")

    foreach(pyversion ${_pyversions})
        message(STATUS "Found dependencies for python${pyversion}, generating bindings")
        message(STATUS "Python${pyversion} install dir: ${KDE_INSTALL_FULL_PYTHON${pyversion}DIR}")

        execute_process(COMMAND "${CMAKE_COMMAND}"
          "-DPYTHON_UMBRELLA_MODULE_FILE=${CMAKE_BINARY_DIR}/py${pyversion}/${GPB_PYTHONNAMESPACE}/__init__.py"
          -P "${GPB_MODULE_DIR}/GeneratePythonBindingUmbrellaModule.cmake"
        )

        add_library(Py${pyversion}KF5${GPB_MODULENAME} MODULE
          "${CMAKE_CURRENT_BINARY_DIR}/pybuild/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/unified${GPB_MODULENAME}.cpp"
        )
        add_dependencies(Py${pyversion}KF5${GPB_MODULENAME} sip_generated_${GPB_MODULENAME}_files)
        target_link_libraries(Py${pyversion}KF5${GPB_MODULENAME} PRIVATE ${GPB_TARGET} Python::Libs${pyversion})

        target_compile_options(Py${pyversion}KF5${GPB_MODULENAME} PRIVATE -fstack-protector-strong -Wno-deprecated-declarations -Wno-overloaded-virtual)
        target_include_directories(Py${pyversion}KF5${GPB_MODULENAME} PRIVATE ${GPB_SIP_INCLUDES})
        target_link_libraries(Py${pyversion}KF5${GPB_MODULENAME} PRIVATE -Wl,-Bsymbolic-functions -Wl,-z,relro)

        set_property(TARGET Py${pyversion}KF5${GPB_MODULENAME} PROPERTY AUTOMOC OFF)
        set_property(TARGET Py${pyversion}KF5${GPB_MODULENAME} PROPERTY PREFIX "")
        set_property(TARGET Py${pyversion}KF5${GPB_MODULENAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/py${pyversion}/${GPB_PYTHONNAMESPACE}")
        set_property(TARGET Py${pyversion}KF5${GPB_MODULENAME} PROPERTY
            OUTPUT_NAME "${GPB_MODULENAME}")

        if (GPB_SIP_DEPENDS MATCHES PyKF5)
          set(_kf5_python_prefix ${KDE_INSTALL_FULL_PYTHON${pyversion}DIR})
        else()
          set(_kf5_python_prefix ${CMAKE_BINARY_DIR}/py${pyversion})
        endif()
        add_test(NAME Py${pyversion}Test${GPB_MODULENAME} COMMAND
            ${GPB_PYTHON${pyversion}_COMMAND} "${CMAKE_SOURCE_DIR}/autotests/pythontest.py"
            ${_kf5_python_prefix}
        )

        install(DIRECTORY ${CMAKE_BINARY_DIR}/py${pyversion}/${GPB_PYTHONNAMESPACE}
          DESTINATION ${KDE_INSTALL_PYTHON${pyversion}DIR})
        install(FILES ${sip_files} "${CMAKE_CURRENT_BINARY_DIR}/sip/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}/${GPB_MODULENAME}mod.sip"
          DESTINATION share/sip/${GPB_PYTHONNAMESPACE}/${GPB_MODULENAME}
        )
    endforeach()
endfunction()
