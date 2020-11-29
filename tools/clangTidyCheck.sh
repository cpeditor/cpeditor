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
mkdir -p build
cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
mv compile_commands.json ..
cd ..

$CLANGTIDY src/*.cpp src/*/*.cpp
