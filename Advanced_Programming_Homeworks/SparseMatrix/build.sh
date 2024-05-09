#!/bin/bash

FLAGS="-std=c++17 -Wall -Wpedantic"
EXECUTABLE="main.cpp"
SOURCE_FILE="SparseMatrix.cpp"

g++ $FLAGS $EXECUTABLE $SOURCE_FILE -o main

if [ $? -eq 0 ]; then
    echo "Compilation successful. "

    echo "Running main file. "
    ./main

else
    echo "Compilation failed. "

fi