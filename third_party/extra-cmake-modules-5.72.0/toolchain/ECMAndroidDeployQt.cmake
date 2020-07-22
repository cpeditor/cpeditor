cmake_minimum_required (VERSION 3.7 FATAL_ERROR)
find_package(Qt5Core REQUIRED)

function(ecm_androiddeployqt QTANDROID_EXPORTED_TARGET ECM_ADDITIONAL_FIND_ROOT_PATH)
    set(EXPORT_DIR "${CMAKE_BINARY_DIR}/${QTANDROID_EXPORTED_TARGET}_build_apk/")
    if (Qt5Core_VERSION VERSION_LESS 5.14.0)
        set(EXECUTABLE_DESTINATION_PATH "${EXPORT_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/lib${QTANDROID_EXPORTED_TARGET}.so")
    else()
        set(EXECUTABLE_DESTINATION_PATH "${EXPORT_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/lib${QTANDROID_EXPORTED_TARGET}_${CMAKE_ANDROID_ARCH_ABI}.so")
    endif()
    set(QML_IMPORT_PATHS "")
    foreach(prefix ${ECM_ADDITIONAL_FIND_ROOT_PATH})
        # qmlimportscanner chokes on symlinks, so we need to resolve those first
        get_filename_component(qml_path "${prefix}/lib/qml" REALPATH)
        if(EXISTS ${qml_path})
            if (QML_IMPORT_PATHS)
                set(QML_IMPORT_PATHS "${QML_IMPORT_PATHS},${qml_path}")
            else()
                set(QML_IMPORT_PATHS "${qml_path}")
            endif()
        endif()
    endforeach()
    if (QML_IMPORT_PATHS)
        set(DEFINE_QML_IMPORT_PATHS "\"qml-import-paths\": \"${QML_IMPORT_PATHS}\",")
    endif()

    set(EXTRA_PREFIX_DIRS "")
    foreach(prefix ${ECM_ADDITIONAL_FIND_ROOT_PATH})
        if (EXTRA_PREFIX_DIRS)
            set(EXTRA_PREFIX_DIRS "${EXTRA_PREFIX_DIRS}, \"${prefix}\"")
        else()
            set(EXTRA_PREFIX_DIRS "\"${prefix}\"")
        endif()
    endforeach()

    if (Qt5Core_VERSION VERSION_LESS 5.14.0)
        set(_deployment_file_template "${_CMAKE_ANDROID_DIR}/deployment-file.json.in")
    else()
        set(_deployment_file_template "${_CMAKE_ANDROID_DIR}/deployment-file-qt514.json.in")
    endif()
    string(TOLOWER "${CMAKE_HOST_SYSTEM_NAME}" _LOWER_CMAKE_HOST_SYSTEM_NAME)
    configure_file("${_deployment_file_template}" "${CMAKE_BINARY_DIR}/${QTANDROID_EXPORTED_TARGET}-deployment.json.in1")
    file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/${QTANDROID_EXPORTED_TARGET}-deployment.json.in2"
                  INPUT  "${CMAKE_BINARY_DIR}/${QTANDROID_EXPORTED_TARGET}-deployment.json.in1")

    if (CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        set(arguments "\\$(ARGS)")
    endif()

    function(havestl var access VALUE)
        if (NOT VALUE STREQUAL "")
            # look for ++ and .so as in libc++.so
            string (REGEX MATCH "\"[^ ]+\\+\\+[^ ]*\.so\"" OUT ${VALUE})
            file(WRITE ${CMAKE_BINARY_DIR}/stl "${OUT}")
        endif()
    endfunction()
    function(haveranlib var access VALUE)
        if (NOT VALUE STREQUAL "")
            file(WRITE ${CMAKE_BINARY_DIR}/ranlib "${VALUE}")
        endif()
    endfunction()
    variable_watch(CMAKE_CXX_STANDARD_LIBRARIES havestl)
    variable_watch(CMAKE_RANLIB haveranlib)

    if (NOT TARGET create-apk)
        add_custom_target(create-apk)
    endif()

    if (NOT DEFINED ANDROID_APK_OUTPUT_DIR)
        set(ANDROID_APK_OUTPUT_DIR ${EXPORT_DIR})
    endif()

    set(CREATEAPK_TARGET_NAME "create-apk-${QTANDROID_EXPORTED_TARGET}")
    add_custom_target(${CREATEAPK_TARGET_NAME}
        COMMAND cmake -E echo "Generating $<TARGET_NAME:${QTANDROID_EXPORTED_TARGET}> with $<TARGET_FILE_DIR:Qt5::qmake>/androiddeployqt"
        COMMAND cmake -E remove_directory "${EXPORT_DIR}"
        COMMAND cmake -E copy_directory "$<TARGET_PROPERTY:create-apk-${QTANDROID_EXPORTED_TARGET},ANDROID_APK_DIR>" "${EXPORT_DIR}"
        COMMAND cmake -E copy "$<TARGET_FILE:${QTANDROID_EXPORTED_TARGET}>" "${EXECUTABLE_DESTINATION_PATH}"
        COMMAND LANG=C cmake "-DTARGET=$<TARGET_FILE:${QTANDROID_EXPORTED_TARGET}>" -P ${_CMAKE_ANDROID_DIR}/hasMainSymbol.cmake
        COMMAND LANG=C cmake -DINPUT_FILE="${QTANDROID_EXPORTED_TARGET}-deployment.json.in2" -DOUTPUT_FILE="${QTANDROID_EXPORTED_TARGET}-deployment.json" "-DTARGET=$<TARGET_FILE:${QTANDROID_EXPORTED_TARGET}>" "-DOUTPUT_DIR=$<TARGET_FILE_DIR:${QTANDROID_EXPORTED_TARGET}>" "-DEXPORT_DIR=${CMAKE_INSTALL_PREFIX}" "-DECM_ADDITIONAL_FIND_ROOT_PATH=\"${ECM_ADDITIONAL_FIND_ROOT_PATH}\"" "-DANDROID_EXTRA_LIBS=\"${ANDROID_EXTRA_LIBS}\"" -P ${_CMAKE_ANDROID_DIR}/specifydependencies.cmake
        COMMAND $<TARGET_FILE_DIR:Qt5::qmake>/androiddeployqt --gradle --input "${QTANDROID_EXPORTED_TARGET}-deployment.json" --apk "${ANDROID_APK_OUTPUT_DIR}/${QTANDROID_EXPORTED_TARGET}-${CMAKE_ANDROID_ARCH_ABI}.apk" --output "${EXPORT_DIR}" --deployment bundled ${arguments}
    )

    add_custom_target(install-apk-${QTANDROID_EXPORTED_TARGET}
        COMMAND adb install -r "${ANDROID_APK_OUTPUT_DIR}/${QTANDROID_EXPORTED_TARGET}-${CMAKE_ANDROID_ARCH_ABI}.apk"
    )
    add_dependencies(create-apk ${CREATEAPK_TARGET_NAME})
endfunction()
