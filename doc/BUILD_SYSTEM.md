**[Documentation Home](../README.md)**

# Build System in Ninetails (Windows)

The build system in Ninetails is extremely primitive. For Windows, you will need
to have access to `cl.exe`, which is available through Visual Studio. The "Desktop
Development with C++" package needs to be installed in order to get the C/C++ compiler.
From there, you will need to launch `vcvars64.bat` in order to get access to the 64-bit
compiler and set it to your path. The process to do this is somewhat tedious if you use
PowerShell, though if you use Command Prompt, it's as simple as executing the bat file.
You will find this build script in a path similar to this:

`C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat`

You can also use `Developer Command Prompt for Visual Studio 2022`.

However you manage to do this, test to make sure you have access
to `cl.exe` *and (this is very important) that it is the 64-bit compiler*. If you
do not have the 64-bit version, the engine may not compile or run correctly.

# Build Structure

The `build.ps1` file contains the compilation command. If you chose to use Command
Prompt instead of PowerShell, then make a batch script with exact same command.

In this file, it is a standard list of files to compile, flags to adjust the compiler,
and linker flags. You will also see some definitions which you can freely adjust or add to.

Most source files are listed here *except* for platform definition files. You will
find these monolithically included in `src/main.c` in their respective platform definition
locations. This is done to appease the build script writer (me). Compilation is faster,
there is no switch logic required, and keeps things isolated on their platform.

# Project Layout

You will find all project source code in `src/`.

For common utilities which any API may need, it is located in `src/core/`. Each utility
in core should be near independent of each other and must *not* rely on anything from
parent directories.

For anything engine-specific, it is located in `src/engine/`. Any APIs in this folder
are optional. These libraries that assist with developing with Ninetails such as
OpenGL renderer abstractions, game development utilities, entity management systems, or UI snap-ins.

For platform specific functionality, `src/platform/[file].h` describes the front-end
header files, and `src/platform/[platform_name]/[file].c` contains the implementation.
The idea here is that the front-end has no concept of how the backend provides platform
functionality. Any time you need to write an API that interacts with OS, this is the
preferred way of doing it.

