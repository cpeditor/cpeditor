project(extra-cmake-modules)
cmake_minimum_required(VERSION 3.5)

set(ECM_FIND_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../find-modules)
set(ECM_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../modules)
set(CMAKE_MODULE_PATH "${ECM_FIND_MODULE_DIR}" "${ECM_MODULE_DIR}")

include(ECMQMLModules)

ecm_find_qmlmodule(QtQuick 2.1)

add_executable(dummy main.c)
