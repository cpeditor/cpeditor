#!/bin/bash

if [[ $1 == 'r' ]]; then
	LOCATIONS=relative
elif [[ $1 == 'a' ]]; then
	LOCATIONS=absolute
else
	LOCATIONS=none
fi

SOURCES=(src build/generated build/cpeditor_autogen/ui build/third_party/QtFindReplaceDialog/dialogs/QtFindReplaceDialog_autogen)

for i in zh_CN ru_RU; do
	lupdate -no-obsolete "${SOURCES[@]}" -locations "$LOCATIONS" -tr-function-alias tr+=TRKEY -ts translations/$i.ts
done
