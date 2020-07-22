execute_process(COMMAND nm --dynamic ${TARGET} ERROR_VARIABLE nm_errors OUTPUT_VARIABLE out RESULT_VARIABLE result)

if (NOT result EQUAL 0)
    message(FATAL_ERROR "nm failed on ${TARGET} exit(${result}): ${nm_errors}")
endif()

string(FIND ${out} " T main\n" found)
if(found LESS 0)
    message(FATAL_ERROR "Could not find a main() symbol on ${TARGET}")
endif()
