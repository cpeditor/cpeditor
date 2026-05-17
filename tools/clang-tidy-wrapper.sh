#!/bin/bash
# Wrapper for clang-tidy that filters out GCC-specific flags
# that clang doesn't understand

args=()
for arg in "$@"; do
    if [[ "$arg" == "-mno-direct-extern-access" ]]; then
        continue
    fi
    args+=("$arg")
done

exec clang-tidy "${args[@]}"
