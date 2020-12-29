#!/bin/bash
set -e
set -o pipefail

top_folder="$(cd "$(dirname "$(which "$0")")"/.. ; pwd -P)"
. "${top_folder}/scripts/lib.sh"

pushd "${top_folder}"
	test -f CMakeLists.txt || fail "Top folder is not detected properly"

	mkdir -p build
	pushd build
		cmake ..
		cmake --build .
	popd
popd
