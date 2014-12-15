#Fleaux

> Note: This project is not ready for public contribution, but feel free to fork

__Fleaux__ is a headless, plugin-based, cross-platform text editor

###Plans

####Core Plugins
There will be 2 built-in core plugins for Fleaux:

-   __Editor__

    The Editor plugin will be the base plugin for the editor API, which third-party plugins will have access to.

-   __Command Line__

    The Command Line plugin will allow third-party plugins to specify commands to be run as child processes of fleaux. The advantage to having a custom command line is then we can take simple text output from stdout or stderr and do something like turn it into an interactive menu. Plus commands will have access to any shared data structures in the rest of the fleaux api.

####Third-Party Plugin API
There will be an event-based api, where plugins can register callback functions to triggered by events coming from one of the core plugins

There will also be a command-based api where plugins can simply register scripts or executables to be run in the command-line core plugin

####Architecture
Fleaux will utilize libuv for the event system, and most cross-platform functions like file I/O, dynamic library loading, threading, and IPC. Because of libuv, I'll have to write very little platform-specific code.

The core plugins will be implemented as static libraries, but like all other plugin models, fleaux will expose a public interface for core plugins, so fleaux can easily be recompiled with new core plugins by anybody without affecting third-party plugins

The base plugin api will be implemented in C. I'm going with C so that I can easily create bindings in other languages like c++, python, and node.

Every plugin will run in it's own child process and communicates with the parent through a tcp socket or named pipe

####Build System

__Why Gradle?__

Yes, gradle's support for native projects is still incubating, but it's already pretty slick, and only going to get better. Before I started writing any code for Fleaux, I knew I was going to have to be heavily invested in whatever build system I chose, so I did some research. Previously, I only had experience with gnu make, which I learned nobody really uses by itself anymore, at least not professionally. I did some searching and came up with a few systems to read about:

-   GNU Autotools

    Autotools are definitely better than using the original gnu make, but it seemed like it was only a minor improve ment.

-   CMake

    CMake seemed to be the most ubiquitous, and I had used it in the past to build other people's projects. Because it is so widely used, it seemed promising, but then I starting looking deeper, and the imperative syntax was a bit disappointing. Plus, it's basically just a makefile generator. Not for me.

-   GYP

    GYP has a nicer declarative syntax to CMake, but it's not as widely used and, like CMake, is also basically just a makefile generator.

-   SCons

    SCons looked really cool, and I liked that fact that the scripts were written in python, plus it's not a makefile generator and it's cross-platform. I kept this one in my back pocket.

Just in case, I did some more searching, and somehow stumbled upon gradle. I had heard of gradle before, and knew it was a java-based build tool, so I was pretty reluctant to look deeper, but I did anyway. Gradle has support for the gcc, clang, and msvc toolchains, every architecture and platform I could ever need support for, and it can still generate visual studio project files like the other guys. Plus, it even comes with a wrapper funcitonality so anybody can build a gradle project without having gradle installed. I figured this could also be quite useful once I get into continuous integration. I also really liked gradle's plugin architecture and convention over configuration attitude. I will admit the learning curve with gradle is steep, considering I didn't know groovy, but the pain is definitely worth it.
