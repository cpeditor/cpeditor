#!/bin/bash

cd .ci/aur
makepkg --printsrcinfo > .SRCINFO
cd ../../cmake/aur
cp PKGBUILD.in PKGBUILD
makepkg --printsrcinfo > .SRCINFO.in
rm PKGBUILD
