#!/bin/bash

for i in SimplifiedChinese Russian; do
	lupdate src/* build/generated/* build/cpeditor_autogen/ui/* -ts translations/$i.ts
done
