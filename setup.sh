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

sh ./build.sh

python3 -m venv build/venv
source build/venv/bin/activate
pip install cpplint
pip install pytest
