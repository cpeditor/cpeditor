string(REPLACE / ";" processedArgs "${ARG_UNPARSED_ARGUMENTS}")

execute_process(COMMAND qmlplugindump -nonrelocatable ${processedArgs} ${KDE_INSTALL_QMLDIR} > ${CMAKE_CURRENT_SOURCE_DIR}/plugins.qmltypes RESULT_VARIABLE code)
if(code)
    message(FATAL_ERROR "Could not call qmlplugindump successfully. Exited with code ${code}")
endif()

execute_process(COMMAND diff -I "//*" ${INPUT} ${CMAKE_CURRENT_SOURCE_DIR}/plugins.qmltypes RESULT_VARIABLE code OUTPUT_VARIABLE out)
if(code)
    message(FATAL_ERROR "both plugin dumps differ:\n${out}")
endif()
