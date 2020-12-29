#!/bin/bash
# This script generates constexpressions out of builtins.def in the custom
# format.
# Serves as an example of using builtinizer project
set -e
set -o pipefail

url=https://raw.githubusercontent.com/llvm/llvm-project/main/clang/include/clang/Basic/Builtins.def

top_folder="$(cd "$(dirname "$(which "$0")")"/.. ; pwd -P)"
. "${top_folder}/scripts/lib.sh"

pushd "${top_folder}/src" > /dev/null
	# Download Builtins.def
	wget -q "${url}" -O Builtins.def
popd > /dev/null

pushd "${top_folder}" > /dev/null
	out=$(./scripts/build.sh)
	if [ "$?" != "0" ] ; then
		fail "${out}"
	fi
	./build/builtinizer --constexpr \
						--constexpr-math \
						--ignore-half \
						--replace ptrdiff_t:__PTRDIFF_T__ \
						--ignore __builtin___CFStringMakeConstantString \
						--ignore __builtin___NSStringMakeConstantString \
						--ignore __builtin_convertvector
popd > /dev/null