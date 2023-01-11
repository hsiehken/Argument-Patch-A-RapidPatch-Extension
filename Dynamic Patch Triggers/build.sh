#!/bin/bash
set -eo pipefail


mkdir -p build/ || true 2>&1
cp Makefile build/
cp *.c *.h *.asm build/

cd build/
echo $(pwd)

sudo make clean all
