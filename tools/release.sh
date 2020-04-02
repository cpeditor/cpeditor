#!/bin/bash

if (($# != 3)); then
  echo 'release.sh <last version> <new version> <is beta>'
  exit 1
fi

set -e

if (($3 == true)); then
  sed -i "s/UNRELEASED/UNRELEASED\n\n## $2 (Beta)/" doc/CHANGELOG.md
else
  sed -i "s/UNRELEASED/UNRELEASED\n\n## $2/" doc/CHANGELOG.md
fi
sed -i "s/$1/$2/" CMakeLists.txt
git add doc/CHANGELOG.md CMakeLists.txt
git diff --cached
echo "Is this diff OK?"
read
git commit -m "Release $2"
git tag "$2"
git push
git push --tags

echo "Is the GitHub release ready?"
read
git switch master
sed -i "s/$1/$2" README.md
git add README.md
git diff --cached --word-diff
echo "Is this diff OK?"
read
git commit -m "docs(README.md): Update version badges"
git push
