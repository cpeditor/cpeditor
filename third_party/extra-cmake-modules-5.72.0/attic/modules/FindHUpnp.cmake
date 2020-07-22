# - Try to find HUPnP library
#  Once done this will define
#
#  HUPNP_FOUND - system has HUPnP
#  HUPNP_INCLUDE_DIR - the LIBHUpnp include directory
#  HUPNP_LIBS - the LIBHUpnp libraries
#  HUPNP_VERSION_STRING - The version of HUpnp
#  HUPNP_VERSION_MAJOR - The major version of HUpnp
#  HUPNP_VERSION_MINOR - The minor version of HUpnp
#  HUPNP_VERSION_PATCH - The patch version of HUpnp
#
# Copyright (c) 2010, Paulo Romulo Alves Barros <paulo.romulo@kdemail.net>

find_path( HUPNP_INCLUDE_DIR HUpnpCore/HUpnp )

find_library( HUPNP_LIBS HUpnp )

if( HUPNP_INCLUDE_DIR AND EXISTS "${HUPNP_INCLUDE_DIR}/HUpnpCore/public/hupnpinfo.h" )
    file( STRINGS "${HUPNP_INCLUDE_DIR}/HUpnpCore/public/hupnpinfo.h" HUPNP_INFO_H REGEX "^#define HUPNP_CORE_.*_VERSION .*$" )
    string( REGEX REPLACE ".*HUPNP_CORE_MAJOR_VERSION ([0-9]+).*" "\\1" HUPNP_VERSION_MAJOR "${HUPNP_INFO_H}" )
    string( REGEX REPLACE ".*HUPNP_CORE_MINOR_VERSION ([0-9]+).*" "\\1" HUPNP_VERSION_MINOR "${HUPNP_INFO_H}" )
    string( REGEX REPLACE ".*HUPNP_CORE_PATCH_VERSION ([0-9]+).*" "\\1" HUPNP_VERSION_PATCH "${HUPNP_INFO_H}" )

    set( HUPNP_VERSION_STRING "${HUPNP_VERSION_MAJOR}.${HUPNP_VERSION_MINOR}.${HUPNP_VERSION_PATCH}" )
endif()

include( FindPackageHandleStandardArgs )

find_package_handle_standard_args( HUpnp DEFAULT_MSG HUPNP_INCLUDE_DIR HUPNP_LIBS )

mark_as_advanced( HUPNP_INCLUDE_DIR HUPNP_LIBS )
