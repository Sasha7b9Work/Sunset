#!/usr/bin/env bash

git pull
dir=$PWD
cd ../../generated/Linia
cmake . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . -- -j$(nproc)
cd $dir

