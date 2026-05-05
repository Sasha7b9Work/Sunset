#!/usr/bin/env bash

git pull
dir=$PWD
cd ../../generated/Linia
cmake --build . -- -j$(nproc) -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
cd $dir

