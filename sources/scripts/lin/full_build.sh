#!/usr/bin/env bash

rm -R -f ../../generated/Linia

cmake ../../CMakeLists.txt -B../../generated/Linia -G "CodeBlocks - Unix Makefiles"

./build.sh
