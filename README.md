<p align="center"><a href="https://www.vecteezy.com"><img src="./doc/display.svg" style="display: inline-text;" height="300px" /></a></p>

Ninetails is a game engine targeted towards indie game developers and hobbyists alike.
What separates Ninetails from general-purpose game engines like Unity or Godot is that
it targets developers who want deeper control over how their engine behaves. This is
accomplished by presenting a set of low-level platform APIs which can be used to develop
higher order game engine components. Such components are provided for use, but are not
required to develop with Ninetails.

For those who want a fresh project but do not want to trudge through the grunt work of developing
the platform abstractions needed to work with the hardware will be right at home with
this game engine since most of that work is done for you.

This engine has two goals in mind. The first is software quality. User experience and
responsive behavior is a critical aspect of this engine, so careful work has been made
to make this process easier for the developer. Secondly, this engine makes zero assumptions
about what you intend to do with it. Think of it like a framework; how much of a game engine
you want is entirely dependent on how much of the engine you intend to use. You could,
in theory, develop your own engine libraries and with the provided platform abstractions to
tailor fit the engine that fits your needs.

Ninetails is also free to use with the MIT license. Although it isn't a requirement of the
license agreement, if you intend to use Ninetails in a commercial project, *please* credit
all major contributors in the credits of the game.

# Getting Started

This project is written using the C-programming language.

Developing a game with Ninetails is not like Unity, Godot, or Unreal. The game
is designed to be directly coupled with the engine. There is no "create new project" button. Simply
clone the project and begin programming.

In order to maintain modularity between Ninetails and your game, organize and modify
`src/engine/runtime.h` and `src/engine/runtime.c` such that it forwards off your
render and update functionality to a separate implementation file. I recommend creating
a folder like `src/game/` which contains all your game source files. This indirection
will help make it easier to get newer version of Ninetails without having to reintegrate
your game's source code.

Due to the nature of how Ninetails works, you will be tempted to modify core platform
APIs found in `src/platform/`. This is absolutely fine to do, but be warned that collecting
updates from the project repository will become difficult. You should expect API
breaking changes in the platform APIs pretty regularly.

# Building the Project

This project doesn't use a build utility such as CMake. This is primarily because directly
invoking the compiler is way faster than using some intermediate build tool. You should
be comfortable modifying the build scripts for each respective platform. Documentation
will be available to describe how to do go about this for each platform.

### Windows

In order to build Ninetails on Windows, you will need to install Visual Studio with Desktop
Development with C++ installed. Once you have it installed, you will need access to the `cl`
compiler by setting the VC runtime variables in your path. This can be accomplished in a
number of ways, but the easiest way is to run the Developer Command Prompt or by running
the `vcvars64.bat` file located in the `VC/Auxiliary/Build/` folder of your Visual Studio
install location in Command Prompt. You can also manually add `cl.exe` to your environment PATH.

Once you have access to the `cl.exe` compiler, ensure that it is the 64-bit
version of the compiler. If you do not, the project may fail to compile or fail to run correctly.

This project comes equipped with build scripts for you, `build.ps1` and `build.bat` for PowerShell
and Command Prompt respectively. These scripts merely contain the compiler command that builds the
project for you. By default, build output is placed in `bin`. If this folder doesn't exist when you
clone the project, then you may need to create this folder.

### Linux

Linux support is currently TBD.

### MacOSX

Mac support is currently TBD.

# Project Contributions

Contributions are welcome, please read below on how to handle each contribution.

### Contributing Features

Feature contributions require considerable review.

1.  Submit a pull request that outlines what your feature is and how it works. Your
    pull request should contain only one feature at a time. Engine features must be
    designed to be opt-in. Platform features should be well isolated from other
    platform API features.

2.  A review of the feature will be done to ensure that the above guidelines are followed.
    There are no guarantees that your feature will be added.

3.  If your feature is approved, it will be merged into the project.

### Contributing Bugfixes

If you spot a bug with the engine, please provide the bug fix.

1.  Submit a pull request that outlines the bug, what you did to fix it, and the steps
    to reproduce this bug. The more detailed you are, the easier it is to review.

2.  A review of the bugfix will be done. If the bug can not be reproduced by the maintainer,
    then a discussion will be opened up to find a way to reproduce it. If the bug can not
    be reproduced, the bug fix may not be approved.

3.  If your bugfix is approved, it will be merged into the project.

### Contributing Documentation

This is the easiest way to contribute to the project.

1.  Submit a pull request with your documentation.

2.  A review of the formatting and changes.

3.  If approved, it will be merged into the project.

# Documentation

Documenation is a work in progress and are tentative in the early development phase
of the project.

### **[Platform: Type System](./doc/TYPE_SYSTEM.md)**

A provided set of typedefs that are used heavily throughout the source code.

### **[Platform: Window API](./doc/WINDOW_API.md)**

This is the primary API for constructing a window and the associated utilities to manage it.
Currently, this API only allows for a single primary window to be shown.

### **[Platform: OpenGL API](./doc/OPENGL_API.md)**

The OpenGL API is the system hooks to establish a hardware rendering context and
related helpers to make working with OpenGL a little easier.

### **[Platform: Input API](./doc/INPUT_API.md)**

The input API provides the methods necessary to inspect the state of input, timing,
and analog access to various controller based functionalities.

# License

Copyright (c) 2024 Christopher DeJong / magictrick-dev on GitHub

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
