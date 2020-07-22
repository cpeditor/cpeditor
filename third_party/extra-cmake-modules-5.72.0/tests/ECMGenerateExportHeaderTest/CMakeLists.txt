set(installation_path "${CMAKE_CURRENT_BINARY_DIR}/installation/")

macro(add_generate_export_header_library_test _exclude_deprecated_before_and_at)
    if("${ARGV1}" STREQUAL "WITH_GROUP")
        set(_set_group_option "-DSET_GROUP=TRUE")
        set(_library_buildname "library-${_exclude_deprecated_before_and_at}-group")
    else()
        set(_library_buildname "library-${_exclude_deprecated_before_and_at}")
    endif()
    add_test(NAME ecm_generate_export_header-build-${_library_buildname}
        COMMAND ${CMAKE_CTEST_COMMAND}
            --build-and-test
            "${CMAKE_CURRENT_SOURCE_DIR}/library"
            "${CMAKE_CURRENT_BINARY_DIR}/${_library_buildname}"
            --build-two-config
            --build-generator "${CMAKE_GENERATOR}"
            --build-makeprogram ${CMAKE_MAKE_PROGRAM}
            --build-project library
            --build-target install
            --build-options
                "-DEXCLUDE_DEPRECATED_BEFORE_AND_AT=${_exclude_deprecated_before_and_at}"
                "-DCMAKE_INSTALL_PREFIX:PATH=${installation_path}/${_library_buildname}"
                ${_set_group_option}
            --test-command dummy
    )
endmacro()

macro(add_generate_export_header_consumer_test _test_variant _exclude_deprecated_before_and_at _group_test_mode _consumer_build)
    set(_extra_build_options ${ARGN})
    if(_group_test_mode STREQUAL "GROUPLESS")
        set(_library "library-${_exclude_deprecated_before_and_at}")
    else()
        set(_library "library-${_exclude_deprecated_before_and_at}-group")
    endif()
    add_test(NAME ecm_generate_export_header-${_consumer_build}
        COMMAND ${CMAKE_CTEST_COMMAND}
            --build-and-test
            "${CMAKE_CURRENT_SOURCE_DIR}/consumer"
            "${CMAKE_CURRENT_BINARY_DIR}/${_consumer_build}"
            --build-two-config
            --build-generator "${CMAKE_GENERATOR}"
            --build-makeprogram ${CMAKE_MAKE_PROGRAM}
            --build-project consumer
            --build-options
                "-DTEST_VARIANT:STRING=${_test_variant}"
                "-DDEPRECATED_EXCLUDED_BEFORE_AND_AT:STRING=${_exclude_deprecated_before_and_at}"
                "-DLIBRARY:STRING=${_library}"
                "-DGROUP_MODE:STRING=${_group_test_mode}"
                ${_extra_build_options}
            --test-command dummy
    )
endmacro()

macro(add_generate_export_header_consumer_disable_deprecated_before_and_at_test
    _disable_deprecated_before_and_at
    _exclude_deprecated_before_and_at
    _group_test_mode
    )
    set(_consumer_build "consumer-${_disable_deprecated_before_and_at}-${_exclude_deprecated_before_and_at}-${_group_test_mode}")
    set(_extra_build_options
        "-DLIBRARY_DISABLE_DEPRECATED_BEFORE_AND_AT:STRING=${_disable_deprecated_before_and_at}"
    )
    add_generate_export_header_consumer_test(DISABLE_DEPRECATED_BEFORE_AND_AT
        ${_exclude_deprecated_before_and_at}
        ${_group_test_mode}
        ${_consumer_build}
        ${_extra_build_options}
    )
endmacro()

macro(add_generate_export_header_consumer_no_deprecated_test _exclude_deprecated_before_and_at _group_test_mode)
    set(_consumer_build "consumer-NO-DEPRECATED-${_exclude_deprecated_before_and_at}-${_group_test_mode}")
    add_generate_export_header_consumer_test(NO_DEPRECATED
        ${_exclude_deprecated_before_and_at}
        ${_group_test_mode}
        ${_consumer_build}
    )
endmacro()

# prepare list of versions
set(library_versions 0 2.0.0 CURRENT)
list(LENGTH library_versions library_versions_count)
math(EXPR _last_index ${library_versions_count}-1)

# test generating the library with different EXCLUDE_DEPRECATED_BEFORE_AND_AT values
# als install the generated libraries together incl. exported cmake targets, for use in tests below
# TODO: wariant with DEPRECATED_BASE_VERSION
foreach(_group_arg "" "WITH_GROUP")
    foreach(_index RANGE ${_last_index})
        list(GET library_versions ${_index} _exclude_deprecated_before_and_at)
        add_generate_export_header_library_test(${_exclude_deprecated_before_and_at} ${_group_arg})
    endforeach()
endforeach()


set(group_test_modes "GROUPLESS" "GROUP_USE_DIRECT" "GROUP_USE_GROUP")

# test using the library, built with different EXCLUDE_DEPRECATED_BEFORE_AND_AT values,
# while using different DISABLE_DEPRECATED_BEFORE_AND_AT values
# TODO: test DEPRECATED_WARNINGS_SINCE
foreach(_group_test_mode ${group_test_modes})
    foreach(_exclude_index RANGE ${_last_index})
        list(GET library_versions ${_exclude_index} _exclude_deprecated_before_and_at)
        # using disabled API limit below the excluded API limit is not supported and
        # caught by the code generated from the ecm_generate_export_header,
        # so testing those combination will not work, so start from the excluded API limit
        foreach(_disable_index RANGE ${_exclude_index} ${_last_index})
            list(GET library_versions ${_disable_index} _disable_deprecated_before_and_at)
            add_generate_export_header_consumer_disable_deprecated_before_and_at_test(${_disable_deprecated_before_and_at} ${_exclude_deprecated_before_and_at} ${_group_test_mode})
        endforeach()
    endforeach()
endforeach()

# test with NO_DEPRECATED
foreach(_group_test_mode ${group_test_modes})
    foreach(_exclude_index RANGE ${_last_index})
        list(GET library_versions ${_exclude_index} _exclude_deprecated_before_and_at)
        add_generate_export_header_consumer_no_deprecated_test(${_exclude_deprecated_before_and_at} ${_group_test_mode})
    endforeach()
endforeach()
