#!/usr/bin/env sh

repeat(){
  for i in {1..79}; do echo -n "$1"; done
}

printLine () {
    echo -n "+"
    repeat "-"
    echo
}

logStep () {
    printLine
    echo "| $1"
    printLine
}

logStep "Generating build files..."
cmake -S . -B build

logStep "Building..."
cmake --build build

cp ./build/mc ./bin/mc-llvm
