#!/bin/bash

set -euo pipefail

if type clang-format >/dev/null 2>&1; then
    CLANGFORMAT="clang-format"
elif type clang-format-14 >/dev/null 2>&1; then
    CLANGFORMAT="clang-format-14"
else
    echo "clang-format not installed!"
    exit 1
fi

cd "$(dirname "$0")"/..

$CLANGFORMAT -style=file -i src/*.cpp src/*/*.cpp src/*.hpp src/*/*.hpp
