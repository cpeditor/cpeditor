# - Find Carbon on Mac
#
#  CARBON_LIBRARY - the library to use Carbon
#  CARBON_FOUND - true if Carbon has been found

# Copyright (c) 2006, Benjamin Reed, <ranger@befunk.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(CMakeFindFrameworks)

cmake_find_frameworks(Carbon)

if (Carbon_FRAMEWORKS)
   set(CARBON_LIBRARY "-framework Carbon" CACHE FILEPATH "Carbon framework" FORCE)
   set(CARBON_FOUND 1)
endif (Carbon_FRAMEWORKS)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Carbon DEFAULT_MSG CARBON_LIBRARY)

