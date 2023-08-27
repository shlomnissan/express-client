#!/bin/bash

xargs rm < ./build/install_manifest.txt

rm -r build

mkdir build
cd build

cmake .. -DBUILD_SHARED_LIBS=1 -DBUILD_TESTS=0 -DBUILD_EXAMPLES=0
cmake --build .
cmake --install .
