#!/bin/bash

# Run CMake commands
mkdir -p ./build
cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug .. || exit 1
cmake --build . -j 11 || exit 1
