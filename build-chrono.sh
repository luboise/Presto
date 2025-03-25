#!/bin/bash

mkdir -p ./build-chrono
cd ./build-chrono

cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_DEMOS=OFF -DBUILD_SHARED_LIBS=OFF \
    -DCH_ENABLE_MODULE_MULTICORE=OFF \
    ../ext/chrono || exit 1
cmake --build . || exit 1
sudo cmake --install . || exit 1
