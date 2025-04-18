#!/bin/bash

# Run CMake commands
mkdir -p ./build
cd ./build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug .. || exit 1
cmake --build . || exit 1
