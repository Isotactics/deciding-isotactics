#!/bin/bash

if [ "$1" == "clean" ]; then
  rm -rf bin
  rm -rf build
  rm -rf doc
  exit 0;
fi

# create dirs
mkdir --parents bin
mkdir --parents build

cd build
cmake ..
make -j 16
