project(ECMInstallIconsTest)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../modules)

# make sure the test install dir is clean
file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}")

include(ECMInstallIcons)

add_subdirectory(v1-syntax)
add_subdirectory(v1-syntax-l10n)
add_subdirectory(v1-syntax-no-icons)

ecm_install_icons(
    ICONS 16-actions-computer.png
    DESTINATION single-file-test
)

ecm_install_icons(
    ICONS
        16-actions-computer.png
        16-animations-loading.mng
        subdir/16-apps-cmake.png
        16-categories-system-help.mng
        16-emotes-face-smile.png
        16-intl-something.png
        16-status-user-online.png
        128-devices-drive-removable-media-usb-pendrive.png
        128-emblems-emblem-mounted.png
        128-places-home.mng
        sc-mimetypes-fonts-package.svgz
    DESTINATION multi-file-test
)

ecm_install_icons(
    ICONS
        16-actions-computer.png
        16-animations-loading.mng
        subdir/16-apps-cmake.png
        16-categories-system-help.mng
        16-emotes-face-smile.png
        16-intl-something.png
        16-status-user-online.png
        128-devices-drive-removable-media-usb-pendrive.png
        128-emblems-emblem-mounted.png
        128-places-home.mng
        sc-mimetypes-fonts-package.svgz
    DESTINATION themed-test
    THEME theme-name-2
)

ecm_install_icons(
    ICONS
        16-actions-computer.png
        16-animations-loading.mng
        subdir/16-apps-cmake.png
        16-categories-system-help.mng
        16-emotes-face-smile.png
        16-intl-something.png
        16-status-user-online.png
        128-devices-drive-removable-media-usb-pendrive.png
        128-emblems-emblem-mounted.png
        128-places-home.mng
        sc-mimetypes-fonts-package.svgz
    DESTINATION lang-test
    LANG en_US
)

ecm_install_icons(
    ICONS
        16-actions-computer.png
        16-animations-loading.mng
        subdir/16-apps-cmake.png
        16-categories-system-help.mng
        16-emotes-face-smile.png
        16-intl-something.png
        16-status-user-online.png
        128-devices-drive-removable-media-usb-pendrive.png
        128-emblems-emblem-mounted.png
        128-places-home.mng
        sc-mimetypes-fonts-package.svgz
    DESTINATION themed-lang-test
    THEME oxygen
    LANG en_US
)

# all these should be warned about
ecm_install_icons(
    ICONS
        aa-actions-badsize.png          # ignored
        badlynamedfile.png              # ignored
        16-actions-badext.txt           # copied
        hi16-actions-old-style-name.png # copied
    DESTINATION badly-named-files-test
)

find_program(icon_cache_generator NAMES gtk-update-icon-cache)
if (icon_cache_generator)
    set(GENERATE_ICON_CACHE TRUE)
else()
    set(GENERATE_ICON_CACHE FALSE)
endif()

# this will be run by CTest
configure_file(check_tree.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/check_tree.cmake" @ONLY)
