#!/bin/bash

set -euo pipefail

cd "$(dirname "$0")"/..
cmake -S . -B build -G Ninja -D CMAKE_BUILD_TYPE=Debug -D CMAKE_EXPORT_COMPILE_COMMANDS=On "$@"
cmake --build build
