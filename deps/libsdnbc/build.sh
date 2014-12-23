#!/usr/bin/env sh

cd libsdnbc
./gradlew releaseSdnbcStaticLibrary
cd ..
cp -r libsdnbc/src/sdnbc/headers deps/libsdnbc/include/sdnbc
cp libsdnbc/build/binaries/sdnbcStaticLibrary/release/libsdnbc.a deps/libsdnbc/lib
rm -rf libsdnbc
