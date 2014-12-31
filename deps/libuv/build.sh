#!/usr/bin/env sh

cd deps/libuv/src
./autogen.sh
./configure
make
cd ../../..
cp -r deps/libuv/src/include deps/libuv
cp deps/libuv/src/.libs/libuv.a deps/libuv/lib
