#include <core/definitions.h>
#include <core/arena.h>

static memory_arena primary_arena;
static b32 runtime_flag;

b32 
runtime_init(buffer heap)
{

    // Rather than dealing with the raw heap buffer, convert it to a memory arena.
    memory_arena_initialize(&primary_arena, heap.ptr, heap.size);

    // Create the window, automatically show it to the user after it is made.
    b32 window_created = window_initialize("Ninetails Game Engine", 1280, 720, false);
    if (window_created == false) return false;

    // Return true to indicate that init succeeded.
    return true;

}

b32 
runtime_main(buffer heap)
{

    // Show the window.
    window_set_visibility(true);

    // Standard runtime loop.
    i32 count = 0;
    runtime_flag = true;
    while (runtime_flag == true)
    {
        
        window_process_events();
        if (window_should_close()) break;

        if (window_did_size_change())
        {
            i32 width, height;
            window_get_size(&width, &height);
            printf("-- Window size changed to %i, %i.\n", width, height);
        }

        if (window_did_focus_change())
        {
            b32 is_in_focus = window_is_focused();
            printf("-- Window focus changed to %i.\n", is_in_focus);
        }

        if (window_did_position_change())
        {

            i32 x, y;
            window_get_position(&x, &y);
            printf("-- Window position changed to %i, %i.\n", x, y);

        }
        
    }

    window_close();

    return 0; // Return zero for success here.

}

