#!/bin/bash

echo "Warning: Building only the library.";
echo "If you want to build the library AND the executable, run build-all.sh";
echo "";
sleep 1.5;

cmake -S . -B build-libtufs/ -DBUILD_TUFSIMPL=OFF;
cmake --build build-libtufs/;