#!/usr/bin/env sh

sh ./build.sh

python3 -m venv build/venv
source build/venv/bin/activate
pip install cpplint
pip install pytest
