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

if [ "$1" != "--skip-build" ]; then
    sh build.sh
fi

source ./build/venv/bin/activate

logStep "Running lexer tests"
cd build/libs/mclexer/test && ctest --output-on-failure && cd -
cd build/libs/mcparser/test && ctest --output-on-failure && cd -

pytest integration
