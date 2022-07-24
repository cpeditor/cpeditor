#!/bin/bash

set -eo pipefail

cd "$(dirname "$0")"/..

if [[ $1 == 'r' ]]; then
	LOCATIONS=relative
elif [[ $1 == 'a' ]]; then
	LOCATIONS=absolute
else
	LOCATIONS=none
fi

SOURCES=(src build/generated build/cpeditor_autogen/ui build/third_party/QtFindReplaceDialog/dialogs/QtFindReplaceDialog_autogen)

for i in el_GR pt_BR ru_RU zh_CN zh_TW; do
	lupdate -no-obsolete "${SOURCES[@]}" -locations "$LOCATIONS" -tr-function-alias tr+=TRKEY -ts translations/$i.ts
done
