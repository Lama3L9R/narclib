#!/usr/bin/env bash

INPUT_FILE=$1

error() {
    echo "Error: $1"
    exit 1
}

if [ "${INPUT_FILE:0:1}" != "/" ]; then
  INPUT_FILE=$(realpath $INPUT_FILE)
fi

if [ -z $INPUT_FILE ]; then
  error "No input file is found"
fi

if ! command -v "llvm-strip" &> /dev/null; then
    error "llvm-strip is not found!"
fi

llvm-strip -d -U -s $INPUT_FILE