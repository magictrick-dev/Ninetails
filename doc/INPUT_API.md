**[Documentation Home](../README.md)**

# Input API

The input API is designed to be easy to use. It uses an internal frame system that
keeps track of digital and analog input changes and uses it to provided a functional
front-end that is very easy to use. Input initialization happens for you during
window initialization. Due to the nature of how the Win32 API delivers keyboard
and mouse inputs to the user, there is really no way around this without
intrusively affecting user experience.

Key mappings are done using internal engine flags that map to the operating system
constants so that everything is unified on the front-end. You can find these maps
in `src/platform/input.h` as macro definitions which correspond to their literal keys.

The API for digital keys follows a simple paradigm of pressed, released, and down.
Pressed and released are single fire functions which are true only when a key is
considered pressed or released *after* the `window_process_events()` function is called
for that frame. The timing of button presses are based on when `window_process_events()`
occur, leaving a small gap of time between when the event was issued by the OS and when
the user finally calls `window_process_events()`. This time is generally neglible and the
latency is pretty decent for how trivial the setup works.

There is a slight problem with the way Windows handles key events. Since Ninetails relies
on the window procedure to capture input messages, events are not sent to the window if
it is not the currently focused window. This includes key and mouse events. The consequence
of this is a "sticky key" behavior when a key is pressed, the window leaves focus, the key
is released, and then the window regains focus. The release event is never caught and it is
as if the key is still treated as "down" by the engine. You will find this behavior in a lot
of games. There is a simple fix for this:

```C
while (game_is_running)
{

    // ...

    window_process_events();
    if (window_should_close()) break;

    // Detect focus change and if out of focus, release all downed keys.
    if (window_did_focus_change() && !window_is_focused())
    {
        input_release_all();
    }

    // ... 

}
```

## Documentation

Keys/buttons are considered digital inputs, which contain a `pressed`, `released`, and `down` state.
These states are what is returned to the user. The `pressed` and `released` states are frame-dependent
states which get reset and triggered *after* `window_process_events()` and are denoted as single-fire
events.

```C
void input_release_all();
```

-   Releases all inputs that currently down to up, also triggering `released` for that key.

```C
bool input_key_is_pressed(u32 key_code);
```

-   Returns true if the key went down on that frame.

```C
bool input_key_is_released(u32 key_code);
```

-   Returns true if the key went up on that frame.

```C
bool input_key_is_down(u32 key_code);
```

-   Returns true if the key is currently down. Inversely, can be used to check
    if the key is up.

```C
r64 input_key_time_down(u32 key_code);
```

-   Returns the duration, in seconds, that the key has been down.

```C
r64 input_key_time_up(u32 key_code);
```

-   Returns the duration, in seconds, that the key has been up.

```C
bool input_mouse_button_is_pressed(u32 mouse_code);
```

-   Returns true if the mouse button went down on that frame.

```C
bool input_mouse_button_is_released(u32 mouse_code);
```

-   Returns true if the mouse button went up on that frame.

```C
bool input_mouse_button_is_down(u32 mouse_code);
```
-   Returns true if the mouse button is currently down. Inversely, can be used to check
    if the mouse button is up.

```C
r64 input_mouse_button_time_down(u32 mouse_code);
```

-   Returns the duration, in seconds, that the mouse button has been down.

```C
r64 input_mouse_button_time_up(u32 mouse_code);
```

-   Returns the duration, in seconds, that the mouse button has been up.

```C
b32 input_mouse_position_moved();
```

-   Returns true if the mouse position has changed within the window. On Windows,
    this event does not trigger when the mouse moves outside the client window area.
    You will need to manually invoke `input_mouse_position_relative_unbounded()` and
    cache the results to determine if the most changed relative to the unbounded
    positions.

```C
void input_mouse_position_relative_unbounded(i32 *x, i32 *y);
```

-   Sets the x & y coordinates to the mouses relative unbounded coordinates. These
    positions correspond to the upper left hand corner of the client are of the
    window and may return results that are outside the area of client window.

```C
void input_mouse_position_relative_bounded(i32 *x, i32 *y);
```

-   Sets the x & y coordinates to the mouses relative bounded coordinates. This
    value always returns a value that is bounded to the client area of the window.

```C
void input_mouse_position_relative_delta(i32 *x, i32 *y);
```

-   Returns the the delta x & y coordinates of the mouse movement in respect to
    the last frame. These values are bounded to the client area of the window.

```C
void input_mouse_position_absolute(i32 *x, i32 *y);
```

-   Returns the absolute position of the mouse relative to screen space of the
    active monitor. This value may not be entirely consistent within multi-monitor
    setups and you should have relative coordinates for almost all cases. Use at your
    own discretion.

```C
r32 input_mouse_scroll_delta_y();
```

-   Returns the delta distance of the scroll wheel.

```C
b32 input_mouse_scrolled();
```

-   Returns true if the mouse wheel has been scrolled for the current frame.
