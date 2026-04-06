#!/usr/bin/env bash

git pull
dir=$PWD
cd ../../generated/Linia
cmake --build . -- -j$(nproc)
cd $dir

