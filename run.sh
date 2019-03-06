#!/bin/bash

echo "[ ] compiling"
g++ program.cpp src/sampler_tx2.cpp src/vectorn.cpp src/pathn.cpp src/profiler.cpp -std=c++11 -pthread
echo "[x] compiled"