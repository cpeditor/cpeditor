#!/bin/bash

for i in SimplifiedChinese Russian; do
	lupdate -no-obsolete src/* src/*/*.cpp build/generated/* build/cpeditor_autogen/ui/* build/third_party/QtFindReplaceDialog/dialogs/QtFindReplaceDialog_autogen/include/*.h -ts translations/$i.ts
done
