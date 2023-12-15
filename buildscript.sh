#!/bin/bash

REBUILD=$1

if [[ $REBUILD -gt 0 ]]
then
    ./clangformatter.sh

    #flatc --cpp --strict-json --scoped-enums --gen-name-strings --gen-object-api --gen-compare --cpp-std c++17 --gen-mutable --force-empty -o schemas schemas/*.fbs

    rm -rf output/
    mkdir output/
    cmake -S . -B output/
    cmake --build output/ -j 6
    rm -rf runfiles/
    mkdir runfiles/
    cp -rf output/unit_tests runfiles/
else
    echo "normal build\n"
    cmake --build output/ -j 6
    rm -rf runfiles/
    mkdir runfiles/
    cp -rf output/unit_tests runfiles/
fi
