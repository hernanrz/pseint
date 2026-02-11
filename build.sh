#!/bin/bash

cd pseint
# make clean ARCH=lnx
make GPP=emcc   CXXFLAGS="-O3 -DNDEBUG -std=c++17 -DUSE_ZOCKETS"   LDFLAGS="-s EXPORT_ALL=1 -s INVOKE_RUN=0 -s NO_EXIT_RUNTIME=1 -s EXPORTED_FUNCTIONS=['_main'] -s EXPORTED_RUNTIME_METHODS=['callMain','ccall','cwrap']"   ARCH=lnx
cd ..
cp bin/bin/pseint ../pseint-web/public/pseint.js
cp bin/bin/pseint.wasm ../pseint-web/public/pseint.wasm
