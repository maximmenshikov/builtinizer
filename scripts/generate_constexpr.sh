#!/bin/bash
# This script generates constexpressions out of builtins.def in the custom
# format.
# Serves as an example of using builtinizer project
set -e
set -o pipefail

url=https://raw.githubusercontent.com/llvm/llvm-project/main/clang/include/clang/Basic/Builtins.def
params="--constexpr --constexpr-math --ignore-half --ignore-lib
        --replace ptrdiff_t:__PTRDIFF_TYPE__
        --ignore __builtin___CFStringMakeConstantString
        --ignore __builtin___NSStringMakeConstantString
        --ignore __builtin_convertvector
        --ignore __builtin_addressof"
params_list="--list"
out_file="constexpr-builtins.hookdef"
out_file_list="constexpr-builtins.gcc.compdef"

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

    # Signatures of constexpr builtins
    echo "#if defined(__GNUG__) && !defined(__clang__)" > "${out_file}"
    ./build/builtinizer $params >> "${out_file}"
    echo "#endif" >> "${out_file}"

    # List of constexpr builtins
    echo "undefine-builtin: $(./build/builtinizer $params $params_list)" \
        >> "${out_file_list}"

popd > /dev/null