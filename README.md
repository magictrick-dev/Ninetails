# Ninetails Game Engine

Ninetails is a 2D game engine optimized for performance and customizability. This
project is the barebones necessary to develop indie game projects and does not come
with your typical game engine utilities. Instead, it provides all the low-level APIs
to create such utilities so that they can be tailor fitted towards any given project.

The goal of this project is to create a base project that indie game developers can
use to shortcut the early development process by providing a set of performant subsystems
that can be extended. Games and software have lost their quality and it is my mission to
show that doesn't have to be the case.

- **Window Subsystem**

    The window system in Ninetails is designed to be highly responsive. Even modern
    PC games struggle with this, so the Ninetails game engine remedies this by providing
    a non-blocking window system. This comes with some minor concessions about how input
    is fed into the engine.

- **Rendering Subsystem**

    The rendering subsystem is hardware accelerated by default. The rendering API is
    directly exposed to the user should they wish to access it. Currently, Ninetails
    uses OpenGL as the API of choice for this task.

- **Audio Subsystem**

    The audio subsystem, unlike the rendering subsystem, is not exposed by default.
    Instead, it utilizes the lowest latency OS API and provides a rolling buffer
    back to the user to mix audio into. This means that audio synchronization and
    such are up to the user to implement.

- **Jobbing Subsystem**

    For asynchronous multithreading tasks, a primitive jobbing system is provided
    to make this easier. This doesn't supplement specialize parallelization tasks.
    For example, the jobbing system can help distribute entity update workloads,
    file resource fetching, and cleanup routines. Jobs are not differentiable,
    only that the tasklist can be joined until the queue is complete. The main
    thread can participate as it waits. For the most part, the jobbing system is
    designed for updating large amounts of independent data.

- **Memory Allocators**

    Ninetails ships with a handful of custom memory allocators. The most common
    allocator you will see and use in this project is the `memory_arena` allocator.
    This allocator is by far the most robust and performant allocator you can use
    and works perfectly for game development. In addition to this, there are additional
    allocators that can be used depending on the shape of the data being used in
    conjuction of the arena allocator.

- **Limited Library Dependencies**

    In order to keep things barebones, Ninetails only uses a select few libraries.
    These libraries are vetted by the community and widely regarded for their quality
    and ease-of-use. These libraries are available for use and are not required to
    develop with Ninetails (though you probably will since they're just that good).
    Instead, Ninetails utilizes OS-provided libraries to do the heavy lifting. This
    low-dependency requirement of Ninetails ensures that there is no mystery of what
    is happening your code.

    **Dear ImGUI:** For creating a GUI systems and debugging. Requires some amount
    of experience and comfort reading code for documentation, but is super easy to
    get the hang of once you do.

    **GLAD:** No one wants to write function pointers for OpenGL, so GLAD is a must
    for using OpenGL. This is a near unavoidable library to use, but since it provides
    function pointer definitions and typenames, GLAD itself is extremely lightweight.

    **STB Image:** Parsing `.png` files is the devil's work and no one wants to do
    this by hand. Fortunately, this library does all this for you so you don't have to.
    It requires one or two functions to work and overriding the memory allocation
    routines are trivial to do.

## Building the Engine from Source

In order to build Ninetails, you will need to have Visual Studio installed with
Desktop Development with C++ installed. You need to have access to the `cl.exe`
compiler by setting VCVars or launching the developer console (prior option preferred).
Run the build script, which is automatically configured with all the options necessary
to compile the project at the root directory.

Output files are stored in `bin/*`, including the executable.

No other dependencies required.

## Contributing to the Engine

If you would like to contribute to the project, submit a pull request detailing
the changes made to the engine. Please keep pull requests single-featured. While
it isn't required, it would be helpful to match the coding style of the project.
If you're unsure, browse `src` header and source files to get a basic idea.

For bug fixes, please make sure adequately describe the bug that is being fixed, along
with appropriate description of how it occurs and the steps reproduce. If I am not
able to reproduce the bug on my system, then a screenshot/video of the bug would
suffice as an example.

*All contributions to the game engine must not use the standard library outside
of what is already included. This includes* `malloc()/free()`. Ninetails uses
a custom allocator and provides a number of utilities to make memory management
easy and intuitive.

## Documentation

TBD. Documenation will be provided once all core libraries are created. Some documentation
is provided, but there is no guarantee that they're complete. Feel free to browse.

- **[Ninetails Build System](./doc/BUILD_SYSTEM.md)**

    Learn about the project layout, how source files are included, and platform-specific APIs.

- **[Ninetails Type System](./doc/TYPE_SYSTEM.md)**

    There are a number of typedefs in Ninetails and they are an absolute must to use.

## License

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