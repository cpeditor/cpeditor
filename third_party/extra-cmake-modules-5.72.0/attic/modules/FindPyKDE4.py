# Copyright (c) 2014, Raphael Kubo da Costa <rakuco@FreeBSD.org>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

import PyKDE4.kdecore

if __name__ == '__main__':
    try:
        import PyKDE4.pykdeconfig
        pykdecfg = PyKDE4.pykdeconfig.Configuration()
        sip_dir = pykdecfg.pykde_sip_dir
        sip_flags = pykdecfg.pykde_kde_sip_flags
    except ImportError:
        # PyQt4 >= 4.10.0 was built with configure-ng.py instead of
        # configure.py, so pyqtconfig.py and pykdeconfig.py are not installed.
        sip_dir = PyKDE4.kdecore.PYKDE_CONFIGURATION['sip_dir']
        sip_flags = PyKDE4.kdecore.PYKDE_CONFIGURATION['sip_flags']

    print('pykde_version:%06.x' % PyKDE4.kdecore.version())
    print('pykde_version_str:%s' % PyKDE4.kdecore.versionString())
    print('pykde_sip_dir:%s' % sip_dir)
    print('pykde_sip_flags:%s' % sip_flags)
