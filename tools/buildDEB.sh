#!/bin/bash

# This script creates a fake DEB for test.
# You should use linuxdeployqt to copy necessary libraries first.
# Run '${PATH_TO_LINUXDEPLOYQT} ./cpeditor -appimage' under build directory.
# The real build is in .github/workflow/release.yml

pkgname="cpeditor-linux-amd64"
root="$(pwd)"

mkdir -p ${pkgname}/usr/share/
cd ${pkgname}/usr/share/ || exit

mkdir applications/
cp "$root/dist/linux/cpeditor.desktop" ./applications

mkdir -p cpeditor/bin/
cp "$root/dist/DEB/cpeditor.sh" ./cpeditor/
cp -r "$root/build/plugins/" ./cpeditor
cp "$root/build/cpeditor" ./cpeditor/bin/

mkdir -p doc/cpeditor/
cp "$root/LICENSE" ./doc/cpeditor/copyright

mkdir icons/
cp "$root/resources/icon.png" ./icons/cpeditor.png

cp -r "$root/build/translations/" ./cpeditor

cd ..
mkdir -p lib/cpeditor/
cp "$root"/build/lib/* ./lib/cpeditor

cd ..
cp -r "$root/dist/DEB/DEBIAN" .

md5deep -rl usr/ > ./DEBIAN/md5sums
cd DEBIAN || exit
chmod 775 p*
sed -i "s/\${ver}/6.9.9/" control
SIZE=$(du -s ../../${pkgname} | cut -f1)
sed -i "s/\${size}/${SIZE}/" control

cd "$root/" || exit
fakeroot dpkg-deb --build ${pkgname}