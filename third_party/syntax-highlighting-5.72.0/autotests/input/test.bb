# syntax test file for Bitbake receipes

SUMMARY = "GammaRay Qt introspection probe"
HOMEPAGE = "http://www.kdab.com/gammaray"

LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE.GPL.txt;md5=2abfe5daa817fd4346b6d55293941415"

inherit cmake_qt5

SRC_URI = "git://github.com/KDAB/GammaRay;branch=master"

SRCREV = "139e003174f48b0c883fc6c200ef2efb7467bff1"
PV = "2.4.0+git${SRCPV}"

DEPENDS = "qtdeclarative"

S = "${WORKDIR}/git"

EXTRA_OECMAKE += " -DGAMMARAY_BUILD_UI=OFF"

FILES_${PN}-dev += " \
    /usr/lib/cmake/* \
    /usr/mkspecs/modules/* \
"
FILES_${PN}-dbg += " \
    /usr/lib/.debug/* \
    /usr/lib/gammaray/*/*/.debug \
    /usr/lib/gammaray/*/*/styles/.debug \
"

do_install_append() {
    # multiline string ok here
    AVAR="this is
right"
}

def python_function():
    # some python code
    pass

# error: unterinated string
VAR *= "abc
  this is wrong
