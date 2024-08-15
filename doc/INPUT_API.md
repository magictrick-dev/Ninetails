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
for that frame. The timing of button presses are difficult to manage without near
kernel level access, so the best timing that Ninetails offers is in-frame timing.
This means there is a minor gap of time between when the event is registered via the
kernel and when the messages are finally processed by `window_process_events()`. Typically,
this isn't very long, but it is still frame-bound.

