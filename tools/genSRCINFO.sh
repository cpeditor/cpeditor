#!/bin/bash

set -euo pipefail

cd "$(dirname "$0")"/../dist/aur/stable
makepkg --printsrcinfo >.SRCINFO
cd ../git
makepkg --printsrcinfo >.SRCINFO
