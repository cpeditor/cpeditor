#!/usr/bin/env python

import os, sys
import fnmatch
import subprocess

sipArgs = sys.argv[1:]

from PyQt5.Qt import PYQT_CONFIGURATION
sipArgs = PYQT_CONFIGURATION["sip_flags"].split(' ') + sipArgs

idx = sipArgs.index("--module-name")
modname = sipArgs[idx + 1]
del sipArgs[idx]
del sipArgs[idx]

idx = sipArgs.index("-c")
loc = sipArgs[idx + 1]

oldFilenames = fnmatch.filter(os.listdir(loc), "sip" + modname + "*.cpp")
try:
    oldFilenames.remove("sip" + modname + "cmodule.cpp")
except:
    pass
for f in oldFilenames:
    try:
        os.remove(os.path.join(loc, f))
    except OSError:
        pass

idx = sipArgs.index("--unify")
unified = sipArgs[idx + 1]
del sipArgs[idx]
del sipArgs[idx]

idx = sipArgs.index("--sip")
exe = sipArgs[idx + 1]
del sipArgs[idx]
del sipArgs[idx]

try:
    print(subprocess.check_output([exe] + sipArgs))
except subprocess.CalledProcessError:
    sys.exit(1)

newFilenames = fnmatch.filter(os.listdir(loc), "sip" + modname + "*.cpp")
unifiedString = '\n'.join(['#include "%s"' % f for f in newFilenames]) + '\n'

for fn in newFilenames:
    lines = []
    with open(os.path.join(loc, fn), "r") as f:
        lines = f.readlines()
        lines = [line for line in lines if not line.startswith("#line")]
    with open(os.path.join(loc, fn), "w") as f:
        f.write(''.join(lines))

with open(unified, "w") as f:
    f.write(unifiedString)
