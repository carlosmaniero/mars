#!/usr/bin/env sh

cmake -S . -B build
cmake --build build

cd build/test && ctest || cd -
