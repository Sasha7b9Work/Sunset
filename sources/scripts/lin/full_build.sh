#!/usr/bin/env bash

rm -R -f ../../generated/Linia

cmake ../../CMakeLists.txt -B../../generated/Linia -G "CodeBlocks - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake

./build.sh
