#include <platform/win32/inputhandler.h>
#include <platform/input.h>

void 
input_release_all()
{
    input_state *current_frame = get_current_input_state();
    for (size_t idx = 0; idx < 256; ++idx)
        current_frame->keyboard[idx].is_down = false;
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


