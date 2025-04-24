#!/bin/bash

# Usage:
# ./build.sh -o myprog extra/foo.cpp extra/bar.cpp

OUT="Game"
EXTRA_SRCS=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        -o)
            OUT="$2"
            shift 2
            ;;
        *)
            EXTRA_SRCS+=("$1")
            shift
            ;;
    esac
done

EXTRA_SRCS_STR=$(printf " %s" "${EXTRA_SRCS[@]}")
make EXTRA_SRCS="$EXTRA_SRCS_STR" OUT="$OUT"