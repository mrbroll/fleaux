#LibFleaux

> Note: This project is not ready for public contribution, but feel free to fork

__LibFleaux__ is a c++ library that contains the core data structures used by the Fleaux project

###Build Instructions

```
git clone https://github.com/mrbroll/libfleaux.git <dir>
cd <dir>
./gradlew releaseFleauxStaticLibrary
```

To build and run tests:

```
./gradlew releaseTestExecutable
./gradlew test
```

So far, I have only built this on OSX 10.10 and Linux Ubuntu 14.04, but it should theoretically build on Windows too, as long as you have the MSVC compiler installed and in your path.

There isn't any platform-specific code in here, but it does require that your compiler have c++11 support.

So far the only component implemented is the editor, and it only supports very basic CRUD operations (insert/remove text and moving the cursor). More features will be added in the future.
