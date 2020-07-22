include("${CMAKE_CURRENT_SOURCE_DIR}/../TestFindPackage.cmake")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../modules" )

set(STRIGI_MIN_VERSION 0.6.0)

#find_package(Strigi 1.2.3)

test_find_package(Strigi  STRIGI  VERSION 
                                  INCLUDE_DIR 
                                  STREAMANALYZER_LIBRARY
                                  STREAMS_LIBRARY 
                                  STRIGIQTDBUSCLIENT_LIBRARY 
                                  NEEDS_SIGNED_CHAR 
                                  NEEDS_CHAR
                                  LINE_ANALYZER_PREFIX 
                                  THROUGH_ANALYZER_PREFIX )
