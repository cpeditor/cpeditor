#!/bin/bash

# Simple Translation Update Script for CP Editor
# This script updates .ts files from source code without requiring a full build

set -eo pipefail

# Change to project root directory
cd "$(dirname "$0")"/..

# Check if lupdate is available
if ! command -v lupdate &> /dev/null; then
    echo "Error: lupdate is not installed or not in PATH"
    echo "Please install Qt development tools"
    exit 1
fi

# Determine location format from command line argument
if [[ $1 == 'r' ]]; then
    LOCATIONS=relative
    echo "Using relative location format"
elif [[ $1 == 'a' ]]; then
    LOCATIONS=absolute
    echo "Using absolute location format"
else
    LOCATIONS=none
    echo "Using no location information"
fi

# Define source directories that always exist
BASIC_SOURCES=(src ui)

# Define build directories that may exist
BUILD_SOURCES=()
if [[ -d "build/generated" ]]; then
    BUILD_SOURCES+=(build/generated)
    echo "Found build/generated - including in scan"
fi
if [[ -d "build/cpeditor_autogen/ui" ]]; then
    BUILD_SOURCES+=(build/cpeditor_autogen/ui)
    echo "Found build/cpeditor_autogen/ui - including in scan"
fi
if [[ -d "build/third_party/QtFindReplaceDialog/dialogs/QtFindReplaceDialog_autogen" ]]; then
    BUILD_SOURCES+=(build/third_party/QtFindReplaceDialog/dialogs/QtFindReplaceDialog_autogen)
    echo "Found QtFindReplaceDialog autogen - including in scan"
fi

# Combine all sources
ALL_SOURCES=("${BASIC_SOURCES[@]}" "${BUILD_SOURCES[@]}")

echo "Scanning directories: ${ALL_SOURCES[*]}"
echo "Updating translation files..."

# Update each translation file
for locale in el_GR es_MX pt_BR ru_RU zh_CN zh_TW; do
    echo "Updating translations/$locale.ts..."
    lupdate -no-obsolete "${ALL_SOURCES[@]}" \
        -locations "$LOCATIONS" \
        -tr-function-alias tr+=TRKEY \
        -ts "translations/$locale.ts"
done

echo ""
echo "Translation update complete!"
echo ""
echo "Summary:"
echo "- Updated 6 translation files"
echo "- Source directories scanned: ${#ALL_SOURCES[@]}"
echo "- Location format: $LOCATIONS"
echo ""
echo "Next steps:"
echo "1. Use Qt Linguist to edit translation files:"
echo "   linguist translations/zh_CN.ts"
echo ""
echo "2. Build the project to generate .qm files:"
echo "   mkdir -p build && cd build"
echo "   cmake .. && make"
echo ""
echo "3. Test translations in the application"
