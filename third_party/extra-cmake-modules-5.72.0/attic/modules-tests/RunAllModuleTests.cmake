# Run this script via "cmake -P RunAllModuletests.cmake" to
# execute all module tests in one go.
# Run this script with "cmake -DMODULE=foo -P RunAllModuletests.cmake" to 
# test only the one specified module.
# To see only the results and not the other cmake output,
# you can grep for "::".
# To have it delete the previous test build dirs, run it with -DCLEAN_DIRS=TRUE

get_filename_component(currentDir "${CMAKE_CURRENT_LIST_FILE}" PATH)

function(execute_one_test name)
   set(workingDir "${currentDir}/${name}/build")
   if(CLEAN_DIRS)
      file(REMOVE_RECURSE "${workingDir}")
   endif()
   file(MAKE_DIRECTORY "${workingDir}")
   execute_process(COMMAND ${CMAKE_COMMAND} "${currentDir}/${name}"
                   WORKING_DIRECTORY "${workingDir}")
   # now the same with pkg-config disabled
   set(workingDir "${currentDir}/${name}/build-no-pkgconfig")
   if(CLEAN_DIRS)
      file(REMOVE_RECURSE "${workingDir}")
   endif()
   file(MAKE_DIRECTORY "${workingDir}")
   execute_process(COMMAND ${CMAKE_COMMAND} -DPKG_CONFIG_EXECUTABLE=echo "${currentDir}/${name}"
                   WORKING_DIRECTORY "${workingDir}")
endfunction(execute_one_test)

if(DEFINED MODULE)
   set(modulesToTest ${MODULE})
else()
   set(modulesToTest AGG 
                     BlueZ
                     ENCHANT
                     FFmpeg
                     Flac 
                     GObject 
                     LibArt 
                     PCRE 
                     QCA2 
                     QImageBlitz
                     Sqlite 
                     Strigi 
                     USB 
                     Xine 
                     Xmms)
endif()

foreach(currentModule ${modulesToTest})
   execute_one_test(${currentModule})
endforeach(currentModule ${modulesToTest})

