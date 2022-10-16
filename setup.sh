#!/usr/bin/env sh

cmake -S . -B build
cmake --build build

cp ./build/mc ./bin/mc-llvm

python3 -m venv build/venv
source build/venv/bin/activate
pip install cpplint
pip install pytest
