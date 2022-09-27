#!/bin/bash
gcc runner.c -o runner
gcc empty.c -o empty.obj -c -O3
gcc advanced.c -o advanced.obj -c -O3
objdump -D empty.obj > empty_dump.txt
objdump -D advanced.obj > advanced_dump.txt
echo It is up to you to turn empty_dump.txt into empty.bin.
echo It is up to you to turn advanced_dump.txt into advanced.bin.
