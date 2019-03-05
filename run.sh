#!/bin/bash

echo "[ ] compiling"
g++ program.cpp src/sampler_tx2.cpp src/vectorn.cpp -std=c++11
echo "[x] compiled"