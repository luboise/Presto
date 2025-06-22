#!/bin/bash

# Run CMake commands
mkdir -p ./build
cd ./build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang .. || exit 1
cmake --build . || exit 1
