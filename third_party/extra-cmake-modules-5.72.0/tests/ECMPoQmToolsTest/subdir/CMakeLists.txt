# QMLOADER_FILES comes from parent CMakeLists.txt.  This is not something we
# want people to do, but it's unfortunately something projects have done and we
# need to keep them building
add_executable(tr_test_subdir ../tr_test.cpp ${QMLOADER_FILES})
target_link_libraries(tr_test_subdir PRIVATE Qt5::Core)
