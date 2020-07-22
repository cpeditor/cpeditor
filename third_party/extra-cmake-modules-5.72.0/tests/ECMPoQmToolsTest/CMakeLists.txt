project(ECMPoQmToolsTest)
cmake_minimum_required(VERSION 3.5)
set(ECM_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../modules")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../modules)

# make sure the test install dir is clean
file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}")

include(ECMPoQmTools)

include(../test_helpers.cmake)


##############################
#                            #
# ecm_process_po_files_as_qm #
#                            #
##############################


# Should create a process-and-install.qm file and install it
ecm_process_po_files_as_qm(fr ALL
    INSTALL_DESTINATION share/locale
    PO_FILES process-and-install.po
)

# Should create a only-process.qm file, without installing it
ecm_process_po_files_as_qm(fr ALL
    PO_FILES only-process.po
)



##############################
#                            #
# ecm_install_po_files_as_qm #
#                            #
##############################

# Should create a bunch of .qm files and install them in share/locale.
# Should ignore files directly under po/ as well as directories under po/ which
# do not contain any .po files.
ecm_install_po_files_as_qm(po)


# Should create a bunch of .qm files and install them in
# ${CMAKE_INSTALL_LOCALEDIR}
set(CMAKE_INSTALL_LOCALEDIR custom-dir1)
ecm_install_po_files_as_qm(po-custom-dir1)


# Should create a bunch of .qm files and install them in
# ${LOCALE_INSTALL_DIR}
set(LOCALE_INSTALL_DIR custom-dir2)
ecm_install_po_files_as_qm(po-custom-dir2)

unset(CMAKE_INSTALL_LOCALEDIR)
unset(LOCALE_INSTALL_DIR)



########################
#                      #
# ecm_create_qm_loader #
#                      #
########################

find_package(Qt5Core CONFIG REQUIRED)
ecm_install_po_files_as_qm(tr_test-po)


#
# single-threaded test
#
set(tr_test_SRCS
    tr_test.cpp
)
ecm_create_qm_loader(tr_test_SRCS catalog)
add_executable(tr_test ${tr_test_SRCS})
target_link_libraries(tr_test PRIVATE Qt5::Core)


#
# single-threaded test (different catalog name, automoc)
#
# This is to check we don't overwrite previously-generated files.
set(tr_test_2_SRCS
    tr_test.cpp
)
ecm_create_qm_loader(tr_test_2_SRCS catalog2)
add_executable(tr_test_2 ${tr_test_2_SRCS})
set_target_properties(tr_test_2 PROPERTIES AUTOMOC ON)
target_include_directories(tr_test_2 PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(tr_test_2 PRIVATE Qt5::Core)


#
# module for tr_thread_test
#
add_library(tr_thread_module MODULE tr_thread_test_module.cpp ${QMLOADER_FILES})
target_link_libraries(tr_thread_module PRIVATE Qt5::Core)


#
# loading a module on a thread other than the main thread
# (automoc)
#
set(tr_thread_test_SRCS
    tr_thread_test.cpp
)
ecm_create_qm_loader(tr_thread_test_SRCS catalog)
add_executable(tr_thread_test ${tr_thread_test_SRCS})
set_target_properties(tr_thread_test PROPERTIES AUTOMOC ON)
target_include_directories(tr_thread_test PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_definitions(tr_thread_test PRIVATE "MODULE_PATH=\"$<TARGET_FILE:tr_thread_module>\"")
target_link_libraries(tr_thread_test PRIVATE Qt5::Core)


#
# loading a module on a thread other than the main thread
# (different catalog, no AUTOMOC)
#
# make sure the moc file is only visible to this test/target
set(MOC_DIR "${CMAKE_CURRENT_BINARY_DIR}/tr_thread_test_2_moc")
qt5_generate_moc(tr_thread_test.cpp "${MOC_DIR}/tr_thread_test.moc")
# Unset SKIP_AUTOMOC again, to not interfer with AUTOMOC as set for tr_thread_test
set_source_files_properties(tr_thread_test PROPERTIES SKIP_AUTOMOC OFF)

set(tr_thread_test_2_SRCS
    tr_thread_test.cpp
    "${MOC_DIR}/tr_thread_test.moc"
)
ecm_create_qm_loader(tr_thread_test_2_SRCS catalog2)
add_executable(tr_thread_test_2 ${tr_thread_test_2_SRCS})
set_target_properties(tr_thread_test_2 PROPERTIES AUTOMOC OFF)
target_include_directories(tr_thread_test_2 PRIVATE "${CMAKE_CURRENT_BINARY_DIR}" "${MOC_DIR}")
target_compile_definitions(tr_thread_test_2 PRIVATE "MODULE_PATH=\"$<TARGET_FILE:tr_thread_module>\"")
target_link_libraries(tr_thread_test_2 PRIVATE Qt5::Core)


#
# call to ecm_create_qm_loader is in a different CMakeLists.txt to where
# the target it is added to is defined
#
# This is not something we want people to do, but it's unfortunately something
# projects have done and we need to keep them building.
unset(QMLOADER_FILES)
ecm_create_qm_loader(QMLOADER_FILES catalog)
assert_var_defined(QMLOADER_FILES)
add_subdirectory(subdir)



file(GENERATE
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/check_conf.cmake"
    INPUT "${CMAKE_CURRENT_SOURCE_DIR}/check_conf.cmake.in"
)
configure_file(check.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/check.cmake" @ONLY)
