#!/bin/bash

if [ "$1" == "clean" ]; then
  rm -rf bin
  rm -rf build
  rm -rf doc
  exit 0;
fi

# create dirs
mkdir -p bin
mkdir -p build

cd build
cmake ..
make -j 8
