#!/usr/bin/env sh

cd deps/libsdnbc/src
./gradlew releaseSdnbcStaticLibrary
cd ..
cp -r deps/libsdnbc/src/src/sdnbc/headers deps/libsdnbc/include/sdnbc
cp deps/libsdnbc/build/binaries/sdnbcStaticLibrary/release/libsdnbc.a deps/libsdnbc/lib
