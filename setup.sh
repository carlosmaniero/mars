#!/usr/bin/env sh

cmake -S . -B build
cmake --build build

cp ./build/mc ./bin/mc-llvm

pip install cpplint
pip install pytest
