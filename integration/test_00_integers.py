#!/usr/bin/env python3
from .helper import compile, execute

compile("test_00_integers")

def test_basic():
    assert execute("test_00_integers") == "13\n"
