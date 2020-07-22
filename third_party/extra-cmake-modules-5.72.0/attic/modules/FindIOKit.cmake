# - Find IOKit on Mac
#
#  IOKIT_LIBRARY - the library to use IOKit
#  IOKIT_FOUND - true if IOKit has been found

# Copyright (c) 2009, Harald Fernengel <harry@kdevelop.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(CMakeFindFrameworks)

cmake_find_frameworks(IOKit)
cmake_find_frameworks(CoreFoundation)

if (IOKit_FRAMEWORKS)
   set(IOKIT_LIBRARY "-framework IOKit -framework CoreFoundation" CACHE FILEPATH "IOKit framework" FORCE)
   set(IOKIT_FOUND 1)
endif (IOKit_FRAMEWORKS)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IOKit DEFAULT_MSG IOKIT_LIBRARY)

