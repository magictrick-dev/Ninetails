#include <platform/win32/inputhandler.h>
#include <platform/window.h>
#include <platform/input.h>

void 
input_release_all()
{
    input_state *current_frame = get_current_input_state();
    for (size_t idx = 0; idx < 256; ++idx)
    {
        if (current_frame->keyboard[idx].is_down = true)
        {
            current_frame->keyboard[idx].is_down = false;
            current_frame->keyboard[idx].is_released = true;
        }
    }
}

bool 
input_key_is_pressed(u32 key_code)
{
    input_state *current_frame = get_current_input_state();
    return current_frame->keyboard[key_code].is_pressed;
}

bool 
input_key_is_released(u32 key_code)
{

    input_state *current_frame = get_current_input_state();
    return current_frame->keyboard[key_code].is_released;

}

bool 
input_key_is_down(u32 key_code)
{

    input_state *current_frame = get_current_input_state();
    return current_frame->keyboard[key_code].is_down;

}

r64 
input_key_time_down(u32 key_code)
{

    input_state *current_frame = get_current_input_state();
    r64 time_elapsed = system_timestamp_difference_ss(
        current_frame->keyboard[key_code].time,
        system_timestamp());
    return time_elapsed;

}

r64 
input_key_time_up(u32 key_code)
{

    input_state *current_frame = get_current_input_state();
    r64 time_elapsed = system_timestamp_difference_ss(
        current_frame->keyboard[key_code].time,
        system_timestamp());
    return time_elapsed;

}

bool 
input_mouse_button_is_pressed(u32 mouse_code)
{

    input_state *current_frame = get_current_input_state();
    return current_frame->mouse[mouse_code].is_pressed;


}

bool 
input_mouse_button_is_released(u32 mouse_code)
{

    input_state *current_frame = get_current_input_state();
    return current_frame->mouse[mouse_code].is_released;

}

bool 
input_mouse_button_is_down(u32 mouse_code)
{

    input_state *current_frame = get_current_input_state();
    return current_frame->mouse[mouse_code].is_down;

}

r64 
input_mouse_button_time_down(u32 mouse_code)
{

    input_state *current_frame = get_current_input_state();
    r64 time_elapsed = system_timestamp_difference_ss(
        current_frame->mouse[mouse_code].time,
        system_timestamp());
    return time_elapsed;

}

r64 
input_mouse_button_time_up(u32 mouse_code)
{

    input_state *current_frame = get_current_input_state();
    r64 time_elapsed = system_timestamp_difference_ss(
        current_frame->mouse[mouse_code].time,
        system_timestamp());
    return time_elapsed;

}

void 
input_mouse_position_relative_unbounded(i32 *x, i32 *y)
{

    NX_ENSURE_POINTER(x);
    NX_ENSURE_POINTER(y);

    POINT position = {};
    GetCursorPos(&position);
    ScreenToClient((HWND)window_get_handle(), &position);
    *x = position.x;
    *y = position.y;

}

void 
input_mouse_position_relative_bounded(i32 *x, i32 *y)
{

    NX_ENSURE_POINTER(x);
    NX_ENSURE_POINTER(y);
    input_state *current_frame = get_current_input_state();
    *x = current_frame->mouse_position.mouse_x;
    *y = current_frame->mouse_position.mouse_y;

}

void 
input_mouse_position_absolute(i32 *x, i32 *y)
{

    NX_ENSURE_POINTER(x);
    NX_ENSURE_POINTER(y);

    POINT position = {};
    GetCursorPos(&position);
    *x = position.x;
    *y = position.y;

}

b32
input_mouse_position_moved()
{
    input_state *current_frame = get_current_input_state();
    b32 result = current_frame->mouse_position.moved;
    return result;
}

void 
input_mouse_position_relative_delta(i32 *x, i32 *y)
{
    NX_ENSURE_POINTER(x);
    NX_ENSURE_POINTER(y);
    input_state *current_frame = get_current_input_state();
    *x = current_frame->mouse_position.delta_x;
    *y = current_frame->mouse_position.delta_y;

}

r32 
input_mouse_scroll_delta_y()
{
    input_state *current_frame = get_current_input_state();
    r32 result = current_frame->mouse_wheel.delta_y;
    return result;
}

b32 
input_mouse_scrolled()
{

    input_state *current_frame = get_current_input_state();
    b32 result = current_frame->mouse_wheelie;
    return result;

}

