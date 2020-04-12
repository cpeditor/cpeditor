#!/bin/bash

if type clang-format > /dev/null 2>&1; then
    CLANGFORMAT="clang-format";
elif type clang-format-9 > /dev/null 2>&1; then
    CLANGFORMAT="clang-format-9";
else
    echo "clang-format not installed!";
    exit 1;
fi

$CLANGFORMAT -style=file -i src/*.cpp src/*/*.cpp include/*.hpp include/*/*.hpp;
