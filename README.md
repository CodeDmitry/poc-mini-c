# poc-mini-c
Experiment - smallest C binary

runner.c is the windows version.
lrunner.c is the linux version.
empty.c illustrates an address independent do-nothing file to check if it crashes or not.
advanced.c illustrates an address independent "Hello, World!" program.

Use build_windows.sh to build on MinGW.
Use build_linux.sh to build on Linux.

The build generates empty_dump.txt and advanced_dump.txt. The test function in these files needs to be copied
over byte by byte to empty.bin and advanced.bin, which will be loaded by the runners(which in hindsight should probably be called loaders).
