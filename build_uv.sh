#!/usr/bin/env sh

#build uv
cd deps/libuv
./autogen.sh
./configure
make
cd ../..
