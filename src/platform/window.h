#ifndef SRC_PLATFORM_WINDOW_H
#define SRC_PLATFORM_WINDOW_H
#include <core/definitions.h>

// --- Window API --- Read Me --------------------------------------------------
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
//
//
// --- Software Rendering to the Window ----------------------------------------
//
// Window drawing functions are entirely software based and should be used if you're
// intending to write a software renderer. These functions expose the direct system
// calls to blitting to a window.
//
// If you want to perform hardware-based rendering, using the approprate hardware
// renderer calls instead. These are defined in <engine/renderer.h>.
// 
// There are some caveats to software rendering that aren't entirely apparent. For
// example, Win32 pixel colors are in in BGR format, not your typical RGB format.
// The bitmask must be set in the image to determine which format the *bitmap* is
// using to ensure that the copy routine in the implementation file knows where to
// place pixels.
//
// A lot of work is put into making these software rendering routines as performant 
// as possible in the implementation, however you shouldn't directly render to the
// window. Instead, create a "backbuffer" of sorts that you render to and then
// render that bitmap to the window. It's easier to vectorize your software renderer
// routines when you have direct control of the pixel blitting. Much like hardware
// rendering, draw calls are expensive and you should make as few as possible to
// ensure that everything is running as fast as possible. If you do this, you will
// be able to write full-fledged 2D games doing this. (Although you will find that
// using OpenGL as intermediatte rendering layer for software rendering a bit more
// desirable. Use your software-rendered bitmaps as textures and render a single
// quad to the screen. This will give you the full benefits of vertical syncing
// and faster blitting.)
//

void window_initialize(ccptr title, i32 width, i32 height, b32 show);
void window_set_title(ccptr title);
void window_set_size(i32 width, i32 height);
void window_set_visibility(b32 hide);
void window_set_maximization(b32 enable);
void window_set_borderless_fullscreen(b32 enable);
void window_set_resizable(b32 enable);

// TODO(Chris): We will revisit this once we have everything else working as intended.
//              Software rendering will be explored once a 2D hardware renderer is
//              built and ready.

u32 window_get_pixel(i32 x, i32 y);
void window_set_pixel(i32 x, i32 y);
void window_set_bitmap(image bitmap, u32 offset_x, u32 offset_y);
void window_set_fill(u8 r, u8 g, u8 b);

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
