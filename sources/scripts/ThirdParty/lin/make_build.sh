#!/usr/bin/env bash

rm -R -f ./../../../generated/wxWidgets

cmake ../../../ThirdParty/wxWidgets/CMakeLists.txt -B../../../generated/wxWidgets -G "CodeBlocks - Unix Makefiles" -DwxUSE_GTK=OFF  -DwxUSE_X11=ON -DwxBUILD_SAMPLES=OFF -DwxBUILD_SHARED=OFF -DwxBUILD_STRIPPED_RELEASE=ON -DCMAKE_CXX_FLAGS_INIT="-pipe" -DCMAKE_C_FLAGS_INIT="-pipe"

cd ../../../generated/wxWidgets/
cmake --build . -- -j$(nproc)
