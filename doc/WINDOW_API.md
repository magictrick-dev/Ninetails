**[Documentation Home](../README.md)**

# Window API

The window API provides the necessary functionality to create the primary window.
Ninetails only allows for one window at a time in order to prevent event contention
on single-process, multi-window setups. Most games rarely need more than one window anyway.

First, you need to initialize the window. You supply the name, size, and determine
if you want to immediately want to show the window or not. This will automatically
handle all the necessary OS-grunt work required to get a window ready for use.

```C
b32 window_was_created = window_initialize("My Window", 1280, 720, true)
```

Once a window is created, you should handle all incoming messages in the main update
loop for your game. Mishandling this process can cause all sorts of unexpected behavior
that will affect nearly every aspect of the engine, not to mention a horrible user
experience. Fortunately, it's pretty easy to do.

```C
while (game_is_running)
{

    window_process_events();
    if (window_should_close()) break;

}

window_close();
```

There isn't much to it. Remember to process your window events *before*
checking if the window should close. This ensures that things occur on the same frame
rather than a frame late. This is especially important for input, since Windows sends
input through the window message loop. Additional flags are also set in this function
that determine if single-shot events should occur. More on that later.

## Documentation

```C
b32 window_initialize(ccptr title, i32 width, i32 height, b32 show);
```
-   Initializes a window and potentially displays it. If `show` is set to false,
    you must manually set its visibility later with `window_set_visibility(true)`.
    Once a window is initialized, you must process all events at least once per
    update loop. This behavior is described at the top of this documentation page.
    On Windows, creating a window is convoluted process and requires a separate
    thread to handle event processing. If you intend to extend window functionality,
    take your time to go over the source code of this function to trace this process.

```C
void window_process_events();
```

-   This function must be called once per update loop to ensure that window related
    events are processed correctly. Make sure that this is called *before* you
    call `window_should_close()` so that you have the collection of current events
    for the current frame.


```C
void window_swap_buffers();
```

-   Responsible for swapping the back buffer with the front buffer. This is required
    for hardware accelerated rendering and should be invoked at the end of the render
    routine to display the results of what was just rendered.

```C
void window_close();
```

-   Destroys the window immediately. Recreating a window after destroying the window
    is somewhat undefined behavior; the expectation is that you immediately close
    the program after you call this.

```C
b32 window_should_close();
```

-   Returns a non-zero value if the window should close. You aren't required to
    immediately close the window should this return true; you may cleanup, save
    data, or prompt the user if they're sure. In pedantic assertion mode, the
    engine will fire an assertion if this is ignored too many times.

```C
vptr window_get_handle();
```

-   Returns the handle that corresponds to the platform window. This handle may
    be required for vendor libraries or other platform APIs that require it.

```C
ccptr window_get_title();
```

-   Returns the string that last set the title of the window.

```C
i32 window_get_width();
```

-   Returns the window's client-area width.

```C
i32 window_get_height();
```

-   Returns the window's client-area height.

```C
void window_get_size(i32 *width, i32 *height);
```

-   Sets the provided pointers to the window's client-area dimensions.

```C
i32 window_get_x_position();
```

-   Returns the window's relative x-position in the monitor viewport.

```C
i32 window_get_y_position();
```

-   Returns the window's relative y-position in the monitor viewport.

```C
void window_get_position(i32 *x, i32 *y);
```

-   Sets the provided pointers to the window's relative position in the monitor viewport.

```C
void window_set_title(ccptr title);
```

-   Sets the window's title.

```C
void window_set_size(i32 width, i32 height);
```

-   Sets the window's size.

```C
void window_set_maximization(b32 enable);
```

-   Sets the window's maximization status. This function has a side-effect of
    showing the window if it isn't shown.

```C
void window_set_resizable(b32 enable);
```

-   Enables or disables the windows ability to be resized by the user.

```C
void window_set_visibility(b32 enable);
```

-   Shows or hides the window from the user.

```C
void window_set_borderless_fullscreen(b32 enable, i32 exit_width, i32 exit_height);
```

-   Sets borderless fullscreen status. Enabling borderless fullscreen ignores the
    exit sizes as it will resize to the monitor's dimensions. When disabling,
    the exit dimensions determine what size the window will return to. This function
    *does not* remember its previous size, so you will need cache this or provide
    a mechanism to handle this procedure.

```C
b32 window_is_open();
```

-   Determines if a window is open. A window is considered open if the window
    handle is valid and present, not if a window is shown. The window handle
    becomes valid after `window_initialize()`.

```C
b32 window_is_borderless_fullscreen();
```

-   Returns the status of borderless fullscreen.

```C
b32 window_is_maximized();
```

-   Returns the status of maximization.

```C
b32 window_is_minimized();
```

-   Returns the status of minimization.

```C
b32 window_is_focused();
```

-   Returns the status of focus.

```C
b32 window_is_resizable();
```

-   Returns the status of resizable.

```C
b32 window_is_visible();
```

-   Returns the status of visibility. A window is considered visible if
    it is shown, this property could be independent of minimization depending
    on platform. You may want to test for minimization as well.

```C
b32 window_did_size_change();
```

-   After `window_process_events()`, if the window was resized *that* frame,
    this function returns true.

```C
b32 window_did_focus_change();
```

-   After `window_process_events()`, if the window focus changed *that* frame,
    this function returns true.

```C
u32 window_get_pixel(i32 x, i32 y);
```

-   Returns the value of the pixel at the provided x and y coordinates. This value
    corresponds to the system's pixel format and may require masking to get the
    color components.

```C
void window_set_pixel(i32 x, i32 y);
```

-   Sets the value of the pixel at the provided x and y coordinates. Much like the
    get counterpart, the pixel format needs to be set in the right order to ensure
    that the correct color is set. The use of this function is terribly inefficient
    outside of trivial contexts and only exists for software rendering.

```C
void window_set_bitmap(image bitmap, u32 offset_x, u32 offset_y);
```

-   Renders a bitmap to the window. The use of this function is preferred over
    `window_set_pixel()` because it allows the user to provide a fully rendered
    bitmap rather than invoking system calls for each pixel. Optimizations can be
    made on user-constructed bitmaps that otherwise would not be possible with the
    set pixel methods. 

```C
void window_set_fill(u8 r, u8 g, u8 b);
```

-   Fills the current window with the provided 8-bit color values using the platform's
    software renderer. This is *not* the preferred method of clearing the screen.
    Please use the hardware accelerators clear functions if you intend to use
    hardware acceleration instead. Otherwise, if you're writting a software renderer,
    this is the fastest method to fill/clear the screen.




