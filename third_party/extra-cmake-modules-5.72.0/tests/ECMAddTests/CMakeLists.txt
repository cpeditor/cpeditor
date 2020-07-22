macro(add_check NAME)
    string(REPLACE "." "/" dir "${NAME}")
    string(REGEX REPLACE "[^.]*\\." "" proj "${NAME}")
    add_test(
        NAME ecm_add_tests-${NAME}
        COMMAND
            ${CMAKE_CTEST_COMMAND}
            --build-and-test
            "${CMAKE_CURRENT_SOURCE_DIR}/${dir}"
            "${CMAKE_CURRENT_BINARY_DIR}/${dir}"
            --build-two-config
            --build-generator ${CMAKE_GENERATOR}
            --build-makeprogram ${CMAKE_MAKE_PROGRAM}
            --build-project ${proj}
            --build-options -DBUILD_TESTING:BOOL=ON
            ${${NAME}_EXTRA_OPTIONS}
            --test-command "${CMAKE_CTEST_COMMAND}"
        )
    add_test(
        NAME ecm_add_tests_did_run-${NAME}
        COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/check_files.cmake" ${ARGN}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${dir}"
        )
    set_property(TEST ecm_add_tests_did_run-${NAME}
        APPEND
        PROPERTY DEPENDS "ecm_add_tests-${NAME}"
        )
endmacro()

add_check(single_tests
    test1.txt
    test2.txt
    test3.txt
    test4.txt
    test5.txt
    test6.txt
    )
add_check(multi_tests
    test1.txt
    test2.txt
    test3.txt
    test4.txt
    test5.txt
    test6.txt
    test7.txt
    test8.txt
    test9.txt
    )

