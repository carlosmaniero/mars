#!/usr/bin/env python3

import subprocess
import os

test_dir = os.path.dirname(os.path.realpath(__file__))
project_dir = os.path.dirname(test_dir)
compiler_path = os.path.join(project_dir, "bin", "mc")

def compile(the_file):
    proc = subprocess.run(
        [
            "bash",
            compiler_path,
            os.path.join(test_dir, the_file + ".mars"),
            os.path.join(test_dir, the_file)
        ],
        capture_output=True,
        text=True
    )

    if proc.returncode != 0:
        print(proc.stdout)
        print(proc.stderr)
    assert proc.returncode == 0

def execute(executable):
    proc = subprocess.run(
        [os.path.join(test_dir, executable)],
        capture_output=True,
        text=True
    )

    if proc.returncode != 0:
        print(proc.stdout)
        print(proc.stderr)

    assert proc.returncode == 0

    return proc.stdout
