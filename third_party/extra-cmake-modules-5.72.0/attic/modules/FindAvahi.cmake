# Find Avahi. Only avahi-common/defs.h is really needed

# Copyright (c) 2007, Jakub Stachowski, <qbast@go2.pl>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


find_path(AVAHI_INCLUDE_DIR avahi-common/defs.h)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Avahi DEFAULT_MSG
                                  AVAHI_INCLUDE_DIR)

mark_as_advanced(AVAHI_INCLUDE_DIR)

