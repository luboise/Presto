#!/bin/bash

./build.sh || exit 1

cd ./build/editor || exit 1

./PrestoEditor || exit 1
