#!/bin/bash

${CLANG_TIDY:-clang-tidy} -fix-errors src/*.cpp -- -Iinclude
#clang-format -i src/*.cpp includes/*.hpp
