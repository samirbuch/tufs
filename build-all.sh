#!/bin/bash

cmake -S . -B build/
cmake --build build/

#The  -S  flag specifies the source directory, and the  -B  flag specifies the build directory.
#The  --build  flag is used to build the project.
#Now, you can run the script to build the project.
#$ chmod +x build-all.sh