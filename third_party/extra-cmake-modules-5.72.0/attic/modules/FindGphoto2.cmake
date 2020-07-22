# cmake macro to test if we use gphoto2
#  GPHOTO2_FOUND - system has the GPHOTO2 library
#  GPHOTO2_INCLUDE_DIR - the GPHOTO2 include directory
#  GPHOTO2_LIBRARIES - The libraries needed to use GPHOTO2

# Copyright (c) 2006, 2007 Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


option(KDE4_GPHOTO2 "do not build kamera (gphoto2 required)" ON)
if (KDE4_GPHOTO2 )
	IF (GPHOTO2_LIBRARIES AND GPHOTO2_INCLUDE_DIR)
  	# in cache already
  		SET(GPHOTO2_FOUND TRUE)
	ELSE (GPHOTO2_LIBRARIES AND GPHOTO2_INCLUDE_DIR)
	
  		FIND_PROGRAM(GHOTO2CONFIG_EXECUTABLE NAMES gphoto2-config 
  		)

  		FIND_PROGRAM(GHOTO2PORTCONFIG_EXECUTABLE NAMES gphoto2-port-config
  		)
 		set(GPHOTO2_LIBRARIES)
		set(GPHOTO2_INCLUDE_DIRS)
  		# if gphoto2-port-config and gphoto2-config have been found
  		IF (GHOTO2PORTCONFIG_EXECUTABLE AND GHOTO2CONFIG_EXECUTABLE)
			EXEC_PROGRAM(${GHOTO2PORTCONFIG_EXECUTABLE} ARGS --libs RETURN_VALUE _return_VALUE OUTPUT_VARIABLE GPHOTO2PORT_LIBRARY)
			EXEC_PROGRAM(${GHOTO2CONFIG_EXECUTABLE} ARGS --libs RETURN_VALUE _return_VALUE OUTPUT_VARIABLE GPHOTO2_LIBRARY)
			
            		EXEC_PROGRAM(${GHOTO2PORTCONFIG_EXECUTABLE} ARGS --cflags RETURN_VALUE _return_VALUE OUTPUT_VARIABLE _GPHOTO2PORT_RESULT_INCLUDE_DIR)
            		EXEC_PROGRAM(${GHOTO2CONFIG_EXECUTABLE} ARGS --cflags RETURN_VALUE _return_VALUE OUTPUT_VARIABLE _GPHOTO2_RESULT_INCLUDE_DIR)

			set(GPHOTO2_LIBRARIES ${GPHOTO2PORT_LIBRARY} ${GPHOTO2_LIBRARY})
  			# the cflags for poppler-qt4 can contain more than one include path
    			separate_arguments(_GPHOTO2_RESULT_INCLUDE_DIR)
      			foreach(_includedir ${_GPHOTO2_RESULT_INCLUDE_DIR})
	          		string(REGEX REPLACE "-I(.+)" "\\1" _includedir "${_includedir}")
		      		set(GPHOTO2_INCLUDE_DIR ${GPHOTO2_INCLUDE_DIR} ${_includedir})
		        endforeach(_includedir)
                        separate_arguments(_GPHOTO2PORT_RESULT_INCLUDE_DIR)
                        foreach(_includedir ${_GPHOTO2PORT_RESULT_INCLUDE_DIR})
                                string(REGEX REPLACE "-I(.+)" "\\1" _includedir "${_includedir}")
                                set(GPHOTO2PORT_INCLUDE_DIR ${GPHOTO2PORT_INCLUDE_DIR} ${_includedir})
                        endforeach(_includedir)

			

			set(GPHOTO2_INCLUDE_DIRS ${GPHOTO2PORT_INCLUDE_DIR} ${GPHOTO2_INCLUDE_DIR} )
		endif()

		IF (GPHOTO2_LIBRARIES AND GPHOTO2_INCLUDE_DIRS)
      		SET(GPHOTO2_FOUND TRUE)
      		message(STATUS "Found gphoto2: ${GPHOTO2_LIBRARIES}")
    	ENDIF (GPHOTO2_LIBRARIES AND GPHOTO2_INCLUDE_DIRS)
	ENDIF (GPHOTO2_LIBRARIES AND GPHOTO2_INCLUDE_DIR)
	MARK_AS_ADVANCED(GPHOTO2_LIBRARIES GPHOTO2_INCLUDE_DIRS)

endif()

