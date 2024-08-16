#include <platform/input.h>
#include <platform/system.h>
#include <platform/filesystem.h>
#include <platform/opengl.h>
#include <platform/window.h>
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
    b32 window_created = window_initialize("Ninetails Game Engine", 640, 480, false);
    if (window_created == false) return false;

    // When the window is created, we can now initialize a render context.
    if (!create_opengl_render_context(window_get_handle())) return false;
    if (!set_opengl_vertical_sync(1))
    {
        printf("-- Unable to set swap interval to desired value.\n");
    }

    // Set some OpenGL context stuff.
    glEnable(GL_DEPTH_TEST);

    // Preset values, swap frame afterwards to show it.
    glViewport(0, 0, window_get_width(), window_get_height());
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    window_swap_buffers();

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

        // Pre-loop stuff.
        window_process_events();
        if (window_should_close()) break;

        // Prevents keys sticking when window focus changes.
        if (window_did_focus_change() && !window_is_focused())
        {
            input_release_all();
        }

        // Test keyboard functionality.
        if (input_key_is_pressed(NxKeyA))
        {
            r64 time_released = input_key_time_up(NxKeyA);
            printf("-- The A key was free for %fs.\n", time_released);
            printf("-- The A key was pressed.\n");
        }

        else if (input_key_is_released(NxKeyA))
        {
            r64 time_down = input_key_time_down(NxKeyA);
            printf("-- The A key was free for %fs.\n", time_down);
            printf("-- The A key was released.\n");
        }

        // Test mouse functionality.
        if (input_mouse_button_is_pressed(NxMouseLeft))
        {
            printf("-- The left mouse button was pressed.\n");
        }

        else if (input_mouse_button_is_released(NxMouseLeft))
        {
            printf("-- The left mouse button was released.\n");
        }

        if (input_mouse_position_moved())
        {
            i32 x, y, dx, dy;
            input_mouse_position_relative(&x, &y);
            input_mouse_position_relative_delta(&dx, &dy);
            printf("-- Mouse position moved %i %i, delta %i %i\n", x, y, dx, dy);
        }

        // Rendering pre-frame stuff.
        glViewport(0, 0, window_get_width(), window_get_height());
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Swap the buffers at the end.
        window_swap_buffers();
        
        
    }

    window_close();

    return 0; // Return zero for success here.

}

