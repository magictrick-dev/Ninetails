#ifndef SRC_PLATFORM_WINDOW_H
#define SRC_PLATFORM_WINDOW_H
#include <core/definitions.h>

b32     window_initialize(ccptr title, i32 width, i32 height, b32 show);
void    window_swap_buffers();
void    window_process_events();
void    window_close();
b32     window_should_close();

vptr    window_get_handle();
ccptr   window_get_title();
i32     window_get_width();
i32     window_get_height();
i32     window_get_x_position();
i32     window_get_y_position();
void    window_get_size(i32 *width, i32 *height);
void    window_get_position(i32 *x, i32 *y);

void    window_set_title(ccptr title);
void    window_set_size(i32 width, i32 height);
void    window_set_maximization(b32 enable);
void    window_set_resizable(b32 enable);
void    window_set_visibility(b32 enable);
void    window_set_borderless_fullscreen(b32 enable, i32 exit_width, i32 exit_height);

b32     window_is_open();
b32     window_is_borderless_fullscreen();
b32     window_is_maximized();
b32     window_is_minimized();
b32     window_is_focused();
b32     window_is_resizable();
b32     window_is_visible();

b32     window_did_size_change();
b32     window_did_focus_change();

/*
u32 window_get_pixel(i32 x, i32 y);
void window_set_pixel(i32 x, i32 y);
void window_set_bitmap(image bitmap, u32 offset_x, u32 offset_y);
void window_set_fill(u8 r, u8 g, u8 b);
*/

/*
void window_swap_buffers();
*/

#endif
