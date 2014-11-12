#!/usr/bin/env sh

#build uv
cd uv
./autogen.sh
./configure
make
cd ..

#build jansson
cd jansson
if [ ! -d "build" ]; then
    mkdir build
fi
cd build
cmake ..
make
cd ../..
