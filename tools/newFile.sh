#!/bin/bash

if (($# != 1)); then
  echo 'newFile.sh <file name>

e.g. newFile.sh Core/Compiler'
  exit 1
fi

set -e

head="/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CP Editor.
 *
 * CP Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CP Editor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is \"Software\" and it isn't immune to bugs.
 *
 */
"

filepath=${1%/*}
filename=${1#$filepath/}
define=${filename}_hpp
DEFINE=${define^^}

mkdir -p "src/$filepath"

echo "$head
#ifndef $DEFINE
#define $DEFINE

#endif // $DEFINE" > "src/$1.hpp"

echo "$head
#include \"$1.hpp\"" > "src/$1.cpp"
