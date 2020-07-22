cmake_minimum_required(VERSION 3.2)

project(GenerateSipBindings)

find_package(Qt5Core REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(CMAKE_CXX_STANDARD 14)

add_library(ExternalLib SHARED external_lib.cpp)
target_link_libraries(ExternalLib PUBLIC Qt5::Core)
target_compile_features(ExternalLib PUBLIC cxx_nullptr)

add_library(CppLib SHARED
  cpplib.cpp
  subdir/subdirfile.cpp
)
target_link_libraries(CppLib
    PUBLIC Qt5::Core
    PRIVATE ExternalLib
)
target_compile_features(CppLib PUBLIC cxx_nullptr)
target_include_directories(CppLib PUBLIC subdir)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../find-modules)

find_package(PythonModuleGeneration REQUIRED)

ecm_generate_python_binding(
  TARGET ExternalLib
  PYTHONNAMESPACE PyTest
  MODULENAME ExternalLib
  SIP_DEPENDS
    QtCore/QtCoremod.sip
  HEADERS
    external_lib.h
)

ecm_generate_python_binding(
  TARGET CppLib
  PYTHONNAMESPACE PyTest
  MODULENAME CppLib
  RULES_FILE "${CMAKE_CURRENT_SOURCE_DIR}/rules_SipTest.py"
  SIP_DEPENDS
    QtCore/QtCoremod.sip
  HEADERS
    cpplib.h
    subdir/subdirfile.h
)
