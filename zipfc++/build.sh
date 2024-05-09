#!/bin/bash

g++ -std=c++17 zipf.cpp main.cpp -o main

./main
rm main
Rscript zipf.R
