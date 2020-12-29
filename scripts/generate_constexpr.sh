#!/bin/bash
# This script generates constexpressions out of builtins.def in the custom
# format.
# Serves as an example of using builtinizer project
set -e
set -o pipefail

top_folder="$(cd "$(dirname "$(which "$0")")"/.. ; pwd -P)"
. "${top_folder}/scripts/lib.sh"

pushd "${top_folder}/src"
	# Download Builtins.def
	wget -q https://raw.githubusercontent.com/llvm/llvm-project/main/clang/include/clang/Basic/Builtins.def -O Builtins.def
popd

pushd "${top_folder}"
	./scripts/build.sh
	./build/builtinizer --constexpr \
						--constexpr-math \
						--ignore-half \
						--replace ptrdiff_t:__PTRDIFF_T__ \
						--ignore __builtin___CFStringMakeConstantString \
						--ignore __builtin___NSStringMakeConstantString \
						--ignore __builtin_convertvector
popd