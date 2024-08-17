#include <platform/input.h>
#include <platform/system.h>
#include <platform/filesystem.h>
#include <platform/opengl.h>
#include <platform/window.h>
#include <core/definitions.h>
#include <core/linear.h>
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

    // Test the math.
    printf("Identity Matrix\n");
    m4 identity = matrix4_identity();
    matrix4_print(identity);

    printf("Translation Matrix\n");
    m4 translation = matrix4_translate((v3){ 1.0f, 2.0f, 3.0f });
    matrix4_print(translation);
    
    printf("Scale Matrix\n");
    m4 scale = matrix4_scale((v3){2.0f, 2.0f, 1.0f});
    matrix4_print(scale);

    printf("Test Point\n");
    v4 point_a = { 10.0f, 15.0f, 0.0f, 1.0f };
    vector4_print(point_a);

    printf("After applying transforms\n");
    v4 result = matrix4_mulv4(matrix4_mulm4(translation, scale), point_a);
    vector4_print(result);

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

