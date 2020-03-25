#!/bin/sh

if (($# != 8)); then
  echo 'newReleaseCycle.sh <stable major> <stable minor> <stable patch> <beta major> <beta minor> <beta patch> <new major> <new minor>'
  exit 1
fi

set -e

STABLEMINOR=v$1.$2
STABLEOLD=$1.$2.$3

BETAMINOR=v$4.$5
BETAOLD=$4.$5.$6
BETANEW=$4.$5.$(($6 + 1))

NEWMINOR=v$7.$8
NEWNEW=$7.$8.1

ALPHAOLD=$4.$(($5 + 1)).0
ALPHANEW=$7.$(($8 + 1)).0

NEXTMINOR=v$7.$(($8 + 1))

cat << endOfMessage
The old versions:
Alpha:	master	$ALPHAOLD
Beta:	$BETAMINOR	$BETAOLD
Stable:	$STABLEMINOR	$STABLEOLD

The new versions:
Alpha:	master	$ALPHANEW
Stable:	$BETAMINOR	$BETANEW
Beta:	$NEWMINOR	$NEWNEW

Continue?
endOfMessage

read

git switch "$BETAMINOR"
sed -i "s/UNRELEASED/UNRELEASED\n\n## $BETANEW/" doc/CHANGELOG.md
sed -i "s/$BETAOLD/$BETANEW/" CMakeLists.txt
git add doc/CHANGELOG.md CMakeLists.txt
git diff --cached
echo "Is this diff of $BETANEW OK?"
read
git commit -m "Release $BETANEW"
git tag "$BETANEW"
git push
git push --tags

git switch -c "$NEWMINOR" master
sed -i "s/UNRELEASED/UNRELEASED\n\n## $NEWNEW (Beta)/" doc/CHANGELOG.md
sed -i "s/$ALPHAOLD/$NEWNEW/" CMakeLists.txt
git add doc/CHANGELOG.md CMakeLists.txt
git diff --cached
echo "Is this diff of $NEWNEW OK?"
read
git commit -m "Release $NEWNEW"
git tag "$NEWNEW"
git push -u origin "$NEWMINOR"
git push --tags

git switch master
sed -i "s/UNRELEASED/UNRELEASED\n\n## $NEWMINOR/" doc/CHANGELOG.md
sed -i "s/$ALPHAOLD/$ALPHANEW/" CMakeLists.txt
sed -i "s/$BETAMINOR/$NEWMINOR/g" README.md
sed -i "s/$BETAOLD/$NEWNEW/g" README.md
sed -i "s/$STABLEMINOR/$BETAMINOR/g" README.md
sed -i "s/$STABLEOLD/$BETANEW/g" README.md
sed -i "s/$BETAMINOR/$NEWMINOR/g" README_zh-CN.md
sed -i "s/$BETAOLD/$NEWNEW/g" README_zh-CN.md
sed -i "s/$STABLEMINOR/$BETAMINOR/g" README_zh-CN.md
sed -i "s/$STABLEOLD/$BETANEW/g" README_zh-CN.md
git add doc/CHANGELOG.md CMakeLists.txt README.md README_zh-CN.md
git diff --cached
echo "Is this diff of the new release cycle OK?"
read
git commit -m "Start a new release cycle for $NEXTMINOR"
git push
