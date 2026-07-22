#!/bin/bash

set -euo pipefail

if (($# != 1)); then
  echo 'newFile.sh <file name>

e.g. newFile.sh Core/Compiler'
  exit 1
fi

filepath=${1%/*}
filename=${1#$filepath/}
define=${filename}_hpp
DEFINE=$(echo ${define} | tr "[:lower:]" "[:upper:]")

cd "$(dirname "$0")"/..

mkdir -p "src/$filepath"

echo "#ifndef $DEFINE
#define $DEFINE

#endif // $DEFINE" > "src/$1.hpp"

echo "#include \"$1.hpp\"" > "src/$1.cpp"
