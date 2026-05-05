#!/usr/bin/env bash

dir=$PWD
cd ../../generated/Linia
cmake --build . -- -j$(nproc)
cd $dir

