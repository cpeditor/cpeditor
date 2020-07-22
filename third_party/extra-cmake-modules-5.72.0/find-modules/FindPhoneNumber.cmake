#.rst:
# FindPhoneNumber
# ---------------
#
# Try to find PhoneNumber.
#
# This is a component-based find module, which makes use of the COMPONENTS and
# OPTIONAL_COMPONENTS arguments to find_module.  The following components are
# available::
#
#   PhoneNumber  GeoCoding
#
# If no components are specified, this module will act as though all components
# were passed to OPTIONAL_COMPONENTS.
#
# This module will define the following variables, independently of the
# components searched for or found:
#
# ``PhoneNumber_FOUND``
#     True if (the requestion version of) PhoneNumber is available
#
# For each searched-for components, ``PhoneNumber_<component>_FOUND`` will be set to
# TRUE if the corresponding library was found, and FALSE otherwise.  If
# ``PhoneNumber_<component>_FOUND`` is TRUE, the imported target ``PhoneNumber::<component>``
# will be defined.
#
# Since 5.54.0.

#=============================================================================
# SPDX-FileCopyrightText: 2017 Klaralvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
# SPDX-FileCopyrightText: 2018 Volker Krause <vkrause@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

include(ECMFindModuleHelpersStub)

ecm_find_package_version_check(PhoneNumber)

set(PhoneNumber_known_components
    PhoneNumber
    GeoCoding
)
set(PhoneNumber_default_components ${PhoneNumber_known_components})

set(PhoneNumber_PhoneNumber_lib phonenumber)
set(PhoneNumber_PhoneNumber_header phonenumbers/phonenumberutil.h)
set(PhoneNumber_GeoCoding_lib geocoding)
set(PhoneNumber_GeoCoding_header phonenumbers/geocoding/phonenumber_offline_geocoder.h)

ecm_find_package_parse_components(PhoneNumber
    RESULT_VAR PhoneNumber_components
    KNOWN_COMPONENTS ${PhoneNumber_known_components}
    DEFAULT_COMPONENTS ${PhoneNumber_default_components}
)
ecm_find_package_handle_library_components(PhoneNumber
    COMPONENTS ${PhoneNumber_components}
)
find_package_handle_standard_args(PhoneNumber
    FOUND_VAR
        PhoneNumber_FOUND
    REQUIRED_VARS
        PhoneNumber_LIBRARIES
    VERSION_VAR
        PhoneNumber_VERSION
    HANDLE_COMPONENTS
)
set(PhoneNumber_VERSION 0) # there is no observable version number in the installed files

include(FeatureSummary)
set_package_properties(PhoneNumber PROPERTIES
  URL "https://github.com/googlei18n/libphonenumber"
  DESCRIPTION "Library for parsing, formatting, and validating international phone numbers")
