**[Documentation Home](../README.md)**

# OpenGL API

The OpenGL API provides the functionality necessary to establish an OpenGL rendering
context onto a window. It also comes with some additional utilities to make working
with OpenGL a little bit easier, such as shader compilation and texture loading.

Establishing an OpenGL render context is pretty simple. Assuming you have a valid
window created, then all you need to do is establish the context and you are now
free to invoke GL commands.

```C
// Create the context.
create_opengl_render_context(window_get_handle());

while (game_is_running)
{

    window_process_events();
    if (window_should_close()) break;

    // Perform your rendering commands, such as adjusting the viewport,
    // clearing and setting the fill colors, etc.
    glViewport(0, 0, window_get_width(), window_get_height());
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Do at the end of your rendering routine to display results.
    window_swap_buffers();
}

window_close();
```


## Documentation

```C
b32 create_opengl_render_context(vptr window_handle);
```

-   Sets up an OpenGL rendering context to the current window handle. This only
    needs to happen once. Any successive calls to this with an active context will
    simply return true. If the context creation fails for whatever reason, this function
    will return false. Generally speaking, this is pretty much guaranteed to work since
    we are asking for a pretty basic context with no special features.

-   As the Ninetails engine progresses, the context creation setup may change to
    accomodate more advanced features. This is mostly due to the limitation of the
    way Windows handles OpenGL.

```C
b32 set_opengl_vertical_sync(i32 interval);
```

-   Vertical syncronization can be enabled with this function. It is not guaranteed
    that this function will work. The return value of this function directly corresponds
    to the return value of the kernel call to enable this on the GPU side.


