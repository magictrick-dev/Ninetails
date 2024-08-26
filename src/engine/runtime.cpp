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
static GLuint quad_program;
static GLuint base_texture;

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
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Preset values, swap frame afterwards to show it.
    glViewport(0, 0, window_get_width(), window_get_height());
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    window_swap_buffers();

    // Generate our quad shaders.
    GLuint q2d_vertex_shader = opengl_shader_create(GL_VERTEX_SHADER);
    GLuint q2d_fragment_shader = opengl_shader_create(GL_FRAGMENT_SHADER);
    ccptr quad_vertex_shader_path = "./res/quad2d_vertex.glsl";
    ccptr quad_fragment_shader_path = "./res/quad2d_fragment.glsl";

    if (!file_exists(quad_vertex_shader_path))
    {
        printf("-- Critical shader missing, quad2d_vertex.glsl\n");
        return false;
    }

    if (!file_exists(quad_fragment_shader_path))
    {
        printf("-- Critical shader missing, quad2d_fragment.glsl\n");
        return false;
    }

    u64 vertex_shader_size = file_size(quad_vertex_shader_path);
    u64 fragment_shader_size = file_size(quad_fragment_shader_path);
    if (vertex_shader_size == 0 || fragment_shader_size == 0)
    {
        printf("-- Critical shader error, size for either fragment or vertex shader is zero.\n");
        return false;
    }

    u64 shader_save_point = memory_arena_save(&primary_arena);
    cptr vertex_shader = (cptr)memory_arena_push(&primary_arena, vertex_shader_size + 1);
    cptr fragment_shader = (cptr)memory_arena_push(&primary_arena, fragment_shader_size + 1);
    u64 vertex_read_size = file_read_all(quad_vertex_shader_path, vertex_shader, vertex_shader_size);
    u64 fragment_read_size = file_read_all(quad_fragment_shader_path, fragment_shader, fragment_shader_size);
    if (vertex_read_size != vertex_shader_size)
    {
        printf("-- Critical shader error, read size mismatch for vertex shader.\n");
        return false;
    }

    if (fragment_read_size != fragment_shader_size)
    {
        printf("-- Critical shader error, read size mismatch for fragment shader.\n");
        return false;
    }

    vertex_shader[vertex_shader_size] = '\0';
    fragment_shader[fragment_shader_size] = '\0';

    if (!opengl_shader_compile(q2d_vertex_shader, vertex_shader))
    {
        printf("-- Critical shader error, unable to compile vertex shader.\n");
        return false;
    }

    if (!opengl_shader_compile(q2d_fragment_shader, fragment_shader))
    {
        printf("-- Critical shader error, unable to compile fragment shader.\n");
        return false;
    }

    quad_program = opengl_program_create();
    opengl_program_attach(quad_program, q2d_vertex_shader);
    opengl_program_attach(quad_program, q2d_fragment_shader);
    if (!opengl_program_link(quad_program))
    {
        printf("-- Critical shader error, unable to link program.\n");
        return false;
    }

    opengl_shader_release(q2d_vertex_shader);
    opengl_shader_release(q2d_fragment_shader);


    // Load the texture.
    ccptr test_image_path = "./res/testtex1024x1024.png";
    if (!file_exists(test_image_path))
    {
        printf("-- Critical texture error, image does not exists.\n");
        return false;
    }

    image test_image = {0};
    u64 test_image_size = file_image_size(test_image_path);
    vptr image_buffer = memory_arena_push(&primary_arena, test_image_size);
    if (!file_image_load(test_image_path, &test_image, image_buffer, test_image_size))
    {
        printf("-- Critical texture error, image couldn't be loaded.\n");
        return false;
    }

    base_texture = opengl_texture_create(&test_image);

    memory_arena_restore(&primary_arena, shader_save_point);

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

        //cube();



        // Swap the buffers at the end.
        window_swap_buffers();
        
        
    }

    window_close();

    return 0; // Return zero for success here.

}

