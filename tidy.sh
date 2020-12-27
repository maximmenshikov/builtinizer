#!/bin/bash

${CLANG_FORMAT:-clang-format} -i src/*.cpp includes/*.hpp
${CLANG_TIDY:-clang-tidy} -fix-errors src/*.cpp -- -Iinclude
