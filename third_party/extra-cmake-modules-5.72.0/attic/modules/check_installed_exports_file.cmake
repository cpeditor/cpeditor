
# This file is executed via install(SCRIPT).
# This means it is include()d into the cmake_install.cmake file
# Due to this the following variables already have the correct value:
# CMAKE_INSTALL_PREFIX
# CMAKE_CURRENT_BINARY_DIR
#
# Additionally the following two variables have to be set:
# EXPORT_INSTALL_DIR - set it to the install destination
# EXPORT_FILES  - the filenames of the exports file
#
# Alex


# put all the code into a function so all variables used here are local
# which makes sure including this file multiple times in a cmake_install.cmake works
function(CHECK_INSTALLED_EXPORTS_FILE _filename)

   # get the absolute install directory, consider absolute and relative paths and also DESTDIR
   if(IS_ABSOLUTE "${EXPORT_INSTALL_DIR}")
      set(installDir "$ENV{DESTDIR}${EXPORT_INSTALL_DIR}")
   else()
      set(installDir "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/${EXPORT_INSTALL_DIR}")
   endif()

   set(installedExportsFile "${installDir}/${_filename}")

   #message(STATUS "************ bin dir: ${CMAKE_CURRENT_BINARY_DIR}")
   #message(STATUS "************ prefix: ${CMAKE_INSTALL_PREFIX}")
   #message(STATUS "************ exportsfile: ${installedExportsFile}")

   # if the file already exists at the install location, and if we can
   # find the exports file in the build dir, read both, and if their contents differ,
   # remove all configuration-specific exports files from the install dir, since
   # they may create conflicts if the new targets have been added/targets have been 
   # removed/ targets have been renamed/ the namespace for the exported targets has changed
   if(EXISTS "${installedExportsFile}")
      if (${EXPORT_INSTALL_DIR} MATCHES "^(/)(.+)$")
         set(binaryDirExportFileDir "_${CMAKE_MATCH_2}")
         set(binaryDirExportsFile "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/Export/${binaryDirExportFileDir}/${_filename}")
      else (${EXPORT_INSTALL_DIR} MATCHES "^(/)(.+)$")
         set(binaryDirExportsFile "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/Export/${EXPORT_INSTALL_DIR}/${_filename}")
      endif (${EXPORT_INSTALL_DIR} MATCHES "^(/)(.+)$")

   #   message(STATUS "************* binaryDirExportsFile: ${binaryDirExportsFile}")

      if(EXISTS "${binaryDirExportsFile}") 
         file(READ "${installedExportsFile}" installedExportsFileContents)
         file(READ "${binaryDirExportsFile}" binaryDirExportsFileContents)

         if(NOT "${installedExportsFileContents}" STREQUAL "${binaryDirExportsFileContents}")

            if("${_filename}" MATCHES "^(.+)(\\.cmake)$")
               message(STATUS "XXX Installed and new ${_filename} differ, removing installed ${CMAKE_MATCH_1}-*.cmake files")
               file(GLOB exportFiles "${installDir}/${CMAKE_MATCH_1}-*.cmake")
#               message(STATUS "XXX files: ${exportFiles}")
               file(REMOVE ${exportFiles})
            endif()
         else()
#            message(STATUS "XXX FILES ${_filename} are the same")
         endif()

      endif() 

   endif()

endfunction(CHECK_INSTALLED_EXPORTS_FILE)

# call the function for each exports file
foreach(_currentExportsFile ${EXPORT_FILES})
   check_installed_exports_file("${_currentExportsFile}")
endforeach(_currentExportsFile ${EXPORT_FILES})

