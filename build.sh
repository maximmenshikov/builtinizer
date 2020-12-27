#!/bin/bash
set -e
set -o pipefail

mkdir -p build
cd build
cmake ..
cmake --build .