#ifndef SRC_PLATFORM_WINDOW_H
#define SRC_PLATFORM_WINDOW_H
#include <core/definitions.h>

// --- Window API --------------------------------------------------------------
//
// The window API is by far the most important API to get correct, both in implementation
// and in use. Please read carefully.
//
// Ninetails enforces one window instance, meaning that you can only create one
// window at a time. This is apparent by the fact that the API doesn't return any
// handles back to the user.
//
// You first initialize the window with window_initialize(). This will create a
// window for you. The show parameter dictates whether or not the window will be
// automatically visible on init. Set to false if you intend to modify the window
// after initialization and manually set visibility later.
//
// Once per frame, inspect window_should_close(). If this value returns true, you
// should close the window gracefully. Typically, you will want to break from your
// main loop and *then* use window_close() so that you don't accidentally invoke
// window behaviors on an otherwise invalid window handle.
//
// Do *not* cache anything beyond a single frame. The reason is fairly obvious,
// but if it isn't, the state of the window can be freely modified by the user
// at any time. This means you can't rely on things like size to stay the same
// for very long. Even when you disable resizing, its good practice to query every
// frame to be sure you have actionable values.
//
// Finally, be nice to the user. If the user hits the x-button and window_should_close
// returns true, please close the window ASAP. You can freely ignore this for as long
// as you want, but no one *wants* the window to ignore you (or even worse, close very
// slowly). The engine is designed so that you should be able to close almost immediately.
//

void window_initialize(ccptr title, i32 width, i32 height, b32 show);
void window_set_title(ccptr title);
void window_set_size(i32 width, i32 height);
void window_set_visibility(b32 hide);
void window_set_maximization(b32 enable);
void window_set_borderless_fullscreen(b32 enable);
void window_set_resizable(b32 enable);

vptr window_get_handle();
i32 window_get_width();
i32 window_get_height();
ccptr window_get_title();
void window_get_size(i32 *width, i32 *height);
void window_get_position(i32 *x, i32 *y);

b32 window_should_close();
void window_close();

b32 window_is_open();
b32 window_is_borderless_fullscreen();
b32 window_is_maximized();
b32 window_is_minimized();
b32 window_is_focused();
b32 window_is_resizable();
b32 window_is_visible();

void window_swap_buffers();

#endif
