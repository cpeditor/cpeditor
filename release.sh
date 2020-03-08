#!/bin/bash

if (($# != 2)); then
  echo 'release.sh <last version> <new version>'
  exit 1
fi

sed -i "s/UNRELEASED/UNRELEASED\n\n## $2/" doc/CHANGELOG.md
sed -i "s/$1/$2/" CMakeLists.txt
sed -i "s/$1/$2/" README.md
git add doc/CHANGELOG.md CMakeLists.txt
git diff --cached
echo "Continue?"
read
git commit -m "Release $2" && git tag $2 && git push && git push --tags
