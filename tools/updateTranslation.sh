#!/bin/bash

for i in SimplifiedChinese Russian; do
	lupdate src/* build/generated/* build/cpeditor_autogen/ui/* build/third_party/QtFindReplaceDialog/dialogs/QtFindReplaceDialog_autogen/include/*.h -ts translations/$i.ts
done
