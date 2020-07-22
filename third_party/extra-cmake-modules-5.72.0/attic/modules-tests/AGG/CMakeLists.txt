include("${CMAKE_CURRENT_SOURCE_DIR}/../TestFindPackage.cmake")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../modules" )

test_find_package(AGG  AGG  INCLUDE_DIR LIBRARIES DEFINITIONS)
