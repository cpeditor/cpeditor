#!/bin/bash

# Dependencies: cmake ninja
# Usage: tools/build.sh [options]
# Options: Additional arguments passed to cmake. Not required, can be used to pass arguments like "-D USE_CLANG_TIDY=On".

set -euo pipefail

cd "$(dirname "$0")"/..
cmake -S . -B build -G Ninja -D CMAKE_BUILD_TYPE=Debug -D CMAKE_EXPORT_COMPILE_COMMANDS=On "$@"
cmake --build build
