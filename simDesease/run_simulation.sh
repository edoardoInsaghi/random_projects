#!/bin/bash

g++ -std=c++17 main.cpp sim.cpp -o main

./main
rm main

Rscript plot_results.R