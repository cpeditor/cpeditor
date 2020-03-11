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
git merge "$BETAMINOR" --no-commit
echo "[$NEWMINOR]: Are the conflicts resolved?"
read
git commit -am "Merge $BETAMINOR: Release $BETANEW"
sed -i "s/UNRELEASED/UNRELEASED\n\n## $NEWNEW/" doc/CHANGELOG.md
sed -i "s/$ALPHAOLD/$NEWNEW/" CMakeLists.txt
git add doc/CHANGELOG.md CMakeLists.txt
git diff --cached
echo "Is this diff of $NEWNEW OK?"
read
git commit -m "Release $NEWNEW"
git tag "$NEWNEW"
git push
git push --tags

git switch master
git merge "$NEWMINOR" --no-commit
echo "[master]: Are the conflicts resolved?"
read
git commit -am "Merge $NEWMINOR: Release $NEWNEW"
sed -i "s/UNRELEASED/UNRELEASED\n\n## v$NEWMINOR/" doc/CHANGELOG.md
sed -i "s/$ALPHAOLD/$ALPHANEW/" CMakeLists.txt
sed -i "s/$BETAMINOR/$NEWMINOR/" README.md
sed -i "s/$BETAOLD/$NEWNEW/" README.md
sed -i "s/$STABLEMINOR/$BETAMINOR/" README.md
sed -i "s/$STABLEOLD/$BETANEW/" README.md
git add doc/CHANGELOG.md CMakeLists.txt README.md
git diff --cached
echo "Is this diff of the new release cycle OK?"
read
git commit -m "Start a new release cycle for $NEXTMINOR"
git push
