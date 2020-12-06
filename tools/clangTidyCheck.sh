#!/bin/bash

if type clang-tidy > /dev/null 2>&1; then
	CLANGTIDY="clang-tidy";
elif type clang-tidy > /dev/null 2>&1; then
	CLANGTIDY="clang-tidy-9";
else
	echo "clang-tidy not installed!";
	exit 1;
fi

# Update compile_commands.json file
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1
mv build/compile_commands.json .

$CLANGTIDY src/*.cpp src/*/*.cpp
