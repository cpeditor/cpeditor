# Create an executable symlink to a Python script.
# This also sets the target script's permission bits.

MESSAGE(STATUS "Symlinking $ENV{DESTDIR}/${LINK_NAME} to ${TARGET}")

GET_FILENAME_COMPONENT(abs_link_name $ENV{DESTDIR}/${LINK_NAME} ABSOLUTE)
GET_FILENAME_COMPONENT(link_path $ENV{DESTDIR}/${LINK_NAME} PATH)
GET_FILENAME_COMPONENT(abs_link_path ${link_path} ABSOLUTE)
FILE(MAKE_DIRECTORY ${abs_link_path})

GET_FILENAME_COMPONENT(abs_target ${TARGET} ABSOLUTE)
IF(UNIX)
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E create_symlink ${abs_target} ${abs_link_name})
    EXECUTE_PROCESS(COMMAND chmod a+x $ENV{DESTDIR}/${abs_target})
ENDIF(UNIX)
# FIXME: WIN32 support
