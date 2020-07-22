cmake_minimum_required(VERSION 3.5)

project(plasma-%{APPNAMELC})
 
find_package(ECM 1.4.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(KF5Plasma REQUIRED)

plasma_install_package(package org.kde.%{APPNAMELC})
