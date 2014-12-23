#!/usr/bin/env sh

cd libuv
./autogen.sh
./configure
make
cd ..
cp -r libuv/include deps/libuv
cp libuv/.libs/libuv.a deps/libuv/lib
rm -rf libuv
