#include <platform/input.h>
#include <platform/system.h>
#include <platform/filesystem.h>
#include <platform/opengl.h>
#include <platform/window.h>
#include <core/definitions.h>
#include <core/linear.h>
#include <core/arena.h>
#include <engine/primitives.h>

static memory_arena primary_arena;
static b32 runtime_flag;

memory_arena *
runtime_get_primary_arena()
{
    return &primary_arena;
}

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

    // OpenGL setup.
    GLuint vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    // Standard runtime loop.
    i32 count = 0;
    runtime_flag = true;
    while (runtime_flag)
    {

        // Pre-loop stuff.
        window_process_events();
        if (window_should_close()) break;

        // Prevents keys sticking when window focus changes.
        if (window_did_focus_change() && !window_is_focused())
        {
            input_release_all();
        }

        // Rendering pre-frame stuff.
        glViewport(0, 0, window_get_width(), window_get_height());
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        cube();

        // Swap the buffers at the end.
        window_swap_buffers();
        
        
    }

    window_close();

    return 0; // Return zero for success here.

}

