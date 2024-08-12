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

- `b32 window_initialize(ccptr title, i32 width, i32 height, b32 show);`

    Initializes a window and potentially displays it. If `show` is set to false,
    you must manually set its visibility later with `window_set_visibility(true)`.
    Once a window is initialized, you must process all events at least once per
    update loop. This behavior is described at the top of this documentation page.
    On Windows, creating a window is convoluted process and requires a separate
    thread to handle event processing. If you intend to extend window functionality,
    take your time to go over the source code of this function to trace this process.

- `void window_process_events();`

    This function must be called once per update loop to ensure that window related
    events are processed correctly.

- `void window_close();`

    Documentation here.

- `b32 window_should_close();`

    Documentation here.


