#!/bin/bash

cd .ci/aur/stable
makepkg --printsrcinfo > .SRCINFO
cd ../git
makepkg --printsrcinfo > .SRCINFO
