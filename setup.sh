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

logStep "Creating python virtual environment"

python3 -m venv build/venv
source build/venv/bin/activate
pip install cpplint
