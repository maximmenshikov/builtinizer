#!/bin/bash

top_folder="$(cd "$(dirname "$(which "$0")")"/.. ; pwd -P)"
. "${top_folder}/scripts/lib.sh"

pushd "${top_folder}"
	test -f CMakeLists.txt || fail "Please run script from top folder"

	${CLANG_FORMAT:-clang-format} -i src/*.cpp include/*.hpp
	${CLANG_TIDY:-clang-tidy} -fix-errors src/*.cpp -- -Iinclude
popd