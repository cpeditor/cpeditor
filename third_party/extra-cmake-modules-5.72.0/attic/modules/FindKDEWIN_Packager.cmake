#
# KDEWIN packager  http://www.winkde.org/pub/kde/ports/win32/installer
# 
# The kdewin packager is searched in the following pathes and order 
# 
#    path specified by the environment dir KDEWIN_PACKAGER_DIR
#    <ProgramFiles>/kdewin-packager 
#    <ProgramFiles>/kdewin-installer
#
# The macro KDEWIN_PACKAGER provides package building support and should be 
# added to the top level CMakeLists.txt as shown below
# 
# if (KDEWIN_PACKAGER_FOUND)
#    KDEWIN_PACKAGER(
#       "projectname"
#       "version"
#       "description" 
#       "additional options"
#   )
# endif (KDEWIN_PACKAGER_FOUND)

# The macro KDEWIN_PACKAGER adds three additional makefile targets, which could be 
# called using the regular make tool 
#   
#    kdewin_package 
#           - build kdewin package with currently used compiler and build type 
#    kdewin_package_debug_and_release 
#           - build kdewin debug and release package with currently used compiler
#    kdewin_package_debug_and_release_mingw_and_msvc
#           - build kdewin debug and release package with mingw and msvc compiler
#
#
#
# Copyright (c) 2006-2007, Ralf Habacker
#
# Redistribution and use is allowed according to the terms of the BSD license.
#

if (WIN32)
    if(NOT KDEWIN_PACKAGER_DIR)
        # check for environment variable
    	file(TO_CMAKE_PATH "$ENV{KDEWIN_PACKAGER_DIR}" KDEWIN_PACKAGER_DIR)
    	if(NOT KDEWIN_PACKAGER_DIR)
    		# search in the default program install folder
    		file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" _progFiles)
    		find_file(KDEWIN_PACKAGER_DIR_tmp kdewin-packager kdewin-installer
       			PATHS
       			"${_progFiles}"
    		)
    		set(KDEWIN_PACKAGER_DIR ${KDEWIN_PACKAGER_DIR_tmp})
    	endif()
    endif()

    if (KDEWIN_PACKAGER_DIR)
    
        find_program(KDEWIN_PACKAGER_EXECUTABLE kdewin-packager
            PATHS
                ${KDEWIN_PACKAGER_DIR}/bin
                ${CMAKE_INSTALL_PREFIX}/bin
        )
        
       set(KDEWIN_PACKAGER_FOUND TRUE)
    else (KDEWIN_PACKAGER_DIR)
       set(KDEWIN_PACKAGER_FOUND)
    endif (KDEWIN_PACKAGER_DIR)
    
    if (KDEWIN_PACKAGER_FOUND)
      if (NOT KDEWIN_Packager_FIND_QUIETLY)
        message(STATUS "Found KDEWIN Packager: ${KDEWIN_PACKAGER_DIR}")
      endif (NOT KDEWIN_Packager_FIND_QUIETLY)
    else (KDEWIN_PACKAGER_FOUND)
      if (KDEWIN_Packager_FIND_REQUIRED)
        message(FATAL_ERROR "Could NOT find KDEWIN PAckager")
      endif (KDEWIN_Packager_FIND_REQUIRED)
    endif (KDEWIN_PACKAGER_FOUND)

endif (WIN32)

MACRO (KDEWIN_PACKAGER _name _version _notes _options)
    if (KDEWIN_PACKAGER_FOUND)
        if (MSVC)
            set (type "msvc")
        else (MSVC)
            set (type "mingw")
        endif (MSVC)
    
        add_custom_target(kdewin_package
            COMMAND ${CMAKE_COMMAND} 
                -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/temp -P ${CMAKE_BINARY_DIR}/cmake_install.cmake 
            COMMAND ${KDEWIN_PACKAGER_EXECUTABLE} 
                -name ${_name}
                -root ${CMAKE_BINARY_DIR}/temp
                -srcroot ${CMAKE_SOURCE_DIR}
                -version ${_version} 
                -notes "${_notes}"
                -type ${type}
                ${_options}               
            # FIXME: cleanup does not work 
            #COMMAND rmdir /Q /S ${CMAKE_BINARY_DIR}\temp
        )
        # prelimary target
        add_custom_target(kdewin_package_without_compiler
            COMMAND ${CMAKE_COMMAND} 
                -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/temp -P ${CMAKE_BINARY_DIR}/cmake_install.cmake 
            COMMAND ${KDEWIN_PACKAGER_EXECUTABLE} 
                -name ${_name}
                -root ${CMAKE_BINARY_DIR}/temp
                -srcroot ${CMAKE_SOURCE_DIR}
                -version ${_version} 
                -notes "${_notes}"
                ${_options}               
            # FIXME: cleanup does not work 
            #COMMAND rmdir /Q /S ${CMAKE_BINARY_DIR}\temp
        )
        add_custom_target(kdewin_package_debug_and_release
            COMMAND ${CMAKE_COMMAND} 
                -H$(CMAKE_SOURCE_DIR) 
                -B$(CMAKE_BINARY_DIR)
                -DCMAKE_BUILD_TYPE=Debug
            COMMAND ${CMAKE_MAKE_PROGRAM} 
                clean
            COMMAND ${CMAKE_MAKE_PROGRAM} 
                install/local
            COMMAND ${CMAKE_COMMAND} 
                -H$(CMAKE_SOURCE_DIR) 
                -B$(CMAKE_BINARY_DIR)
                -DCMAKE_BUILD_TYPE=Release
            COMMAND ${CMAKE_MAKE_PROGRAM} 
                clean
            COMMAND ${CMAKE_MAKE_PROGRAM} 
                install/local
            COMMAND ${KDEWIN_PACKAGER_EXECUTABLE} 
                -name ${_name}
                -root ${CMAKE_BINARY_DIR}/temp
                -srcroot ${CMAKE_SOURCE_DIR}
                -version ${_version} 
                -notes "${_notes}"
                -type ${type}
                ${_options}               
            # FIXME: cleanup does not work 
            #COMMAND rmdir /Q /S ${CMAKE_BINARY_DIR}\temp
        )
set (CONTENT "
if \"%1\" == \"package_only\" goto package_only

del CMakeCache.txt
rmdir /Q /S ${CMAKE_BINARY_DIR}/temp 
${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} -G \"MinGW Makefiles\" -DCMAKE_BUILD_TYPE=Debug
mingw32-make
${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/temp -P cmake_install.cmake
${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} -G \"MinGW Makefiles\" -DCMAKE_BUILD_TYPE=Release
mingw32-make
${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/temp -P cmake_install.cmake
del CMakeCache.txt
${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} -G \"NMake Makefiles\" -DCMAKE_BUILD_TYPE=Debug
nmake
${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/temp -P cmake_install.cmake
${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} -G \"NMake Makefiles\" -DCMAKE_BUILD_TYPE=Release
nmake
${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/temp -P cmake_install.cmake
:package_only
${KDEWIN_PACKAGER_EXECUTABLE} -name ${_name} -root ${CMAKE_BINARY_DIR}/temp -srcroot ${CMAKE_SOURCE_DIR} -version ${_version}  -notes \"${_notes}\" ${_options} 
"
)
        write_file(${CMAKE_BINARY_DIR}/kdewin_package_debug_and_release_mingw_and_msvc.bat "${CONTENT}")
        add_custom_target(kdewin_package_debug_and_release_mingw_and_msvc
            COMMAND ${CMAKE_BINARY_DIR}/kdewin_package_debug_and_release_mingw_and_msvc.bat
        )
    endif (KDEWIN_PACKAGER_FOUND)
ENDMACRO (KDEWIN_PACKAGER)

