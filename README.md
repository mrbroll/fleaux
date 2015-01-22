#LibFleaux

> Note: This project is not ready for public contribution, but feel free to fork

__LibFleaux__ is a c++ library that contains the core data structures used by the Fleaux project

###Build Instructions

    git clone https://github.com/mrbroll/libfleaux.git <dir>
    cd <dir>
    ./gradlew releaseFleauxStaticLibrary

To build and run tests:

    ./gradlew releaseTestExecutable
    ./gradlew test

###Documentation
See the docs [here](https://mrbroll.github.io/libfleaux)

__Notes:__
This has only been built on OSX 10.10 and Linux Ubuntu 14.04. Getting this to compile on Windows is really just a matter of setting up the dep install scripts, but I don't have access to a Windows machine right now to do this.

I don't believe there is any c++11 specific code in here yet, but it's very possible that c++11 support will be required as more functionality is added, thus the standard flag in the build script.
