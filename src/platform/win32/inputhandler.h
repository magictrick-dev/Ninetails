#ifndef SRC_PLATFORM_WIN32_INPUTHANDLER_H
#define SRC_PLATFORM_WIN32_INPUTHANDLER_H
#include <core/definitions.h>
#include <platform/input.h>

typedef struct digital_input
{
    b32 is_down;
    b32 is_pressed;
    b32 is_released;
    u64 time;
} digital_input;

typedef struct analog_input
{
    r32 delta_x;
    r32 delta_y;
} analog_input;

typedef struct input_state
{
    digital_input keyboard[256];
    digital_input mouse[8];
} input_state;

input_state*    get_current_input_state();
input_state*    get_previous_input_state();
u32             convert_keycode(u32 code);
void            initialize_input_states();
void            swap_input_states();

#endif
