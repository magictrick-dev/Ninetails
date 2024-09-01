#include <platform/input.h>
#include <platform/system.h>
#include <platform/filesystem.h>
#include <platform/opengl.h>
#include <platform/window.h>

#include <core/definitions.h>
#include <core/linear.h>
#include <core/arena.h>

#include <engine/primitives.h>
#include <engine/renderers/quad2d.h>

#include <math.h>
#include <time.h>
#include <cstdlib>
#include <thread>
#include <random>

inline r32
generate_r32_in_range(i32 low, i32 high)
{

    static thread_local std::mt19937 float_generator(std::random_device{}());
    std::uniform_real_distribution<float> distribution((float)low, (float)high);
    return distribution(float_generator);

}

inline u32
generate_u32_in_range(u32 low, u32 high)
{

    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<unsigned int> distribution(low,high);
    return distribution(generator);

}

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
    b32 window_created = window_initialize("Ninetails Game Engine", 1280, 720, false);
    if (window_created == false) return false;

    // When the window is created, we can now initialize a render context.
    if (!create_opengl_render_context(window_get_handle())) return false;

    if (!set_opengl_vertical_sync(0))
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

// Determine our sub-texture dimensions to index each "sprite" from the sheet.
static r32 subtexture_scale_x      = 1024.0f / 8.0f;
static r32 subtexture_scale_y      = 1024.0f / 8.0f;
static r32 subtexture_width        = subtexture_scale_x / 1024.0f;
static r32 subtexture_height       = subtexture_scale_y / 1024.0f;

inline void
regenerate_quad(quad_layout *layout)
{

    r32 scale       = generate_r32_in_range(8, 32);
    r32 position_x  = generate_r32_in_range(-100, window_get_width() + 100);
    r32 position_y  = generate_r32_in_range(-100, window_get_height() + 100);
    u32 index_x     = generate_u32_in_range(0, 7);
    u32 index_y     = generate_u32_in_range(0, 7);

    layout->transform.position   = { position_x, position_y };
    layout->transform.scale      = { scale, scale};
    layout->texture.offset       = { subtexture_width * index_x, subtexture_height * index_y };
    layout->texture.dimension    = { subtexture_width, subtexture_height };

}

#define SCALE_REDUCTION 24.0f
#define FALL_REDUCTION 128.0f

void
update_quads_within_range(r32 delta_time, u64 start, u64 total, quad_layout *array)
{

    u64 remainder = (total - start) % 8;
    u64 end = (total - start) - remainder;

    // Loop unrolling to increase cache-level performance.
    u64 i = start;
    for (; i < start+end; i+=8)
    {

        quad_layout* a = array + i + 0;
        quad_layout* b = array + i + 1;
        quad_layout* c = array + i + 2;
        quad_layout* d = array + i + 3;
        quad_layout* e = array + i + 4;
        quad_layout* f = array + i + 5;
        quad_layout* g = array + i + 6;
        quad_layout* h = array + i + 7;


        r32 scaling = SCALE_REDUCTION * delta_time;
        r32 falling = FALL_REDUCTION * delta_time;

        a->transform.scale.X -= scaling;
        b->transform.scale.X -= scaling;
        c->transform.scale.X -= scaling;
        d->transform.scale.X -= scaling;
        a->transform.scale.Y = a->transform.scale.X;
        b->transform.scale.Y = b->transform.scale.X;
        c->transform.scale.Y = c->transform.scale.X;
        d->transform.scale.Y = d->transform.scale.X;

        e->transform.scale.X -= scaling;
        f->transform.scale.X -= scaling;
        g->transform.scale.X -= scaling;
        h->transform.scale.X -= scaling;
        e->transform.scale.Y = e->transform.scale.X;
        f->transform.scale.Y = f->transform.scale.X;
        g->transform.scale.Y = g->transform.scale.X;
        h->transform.scale.Y = h->transform.scale.X;

        a->transform.position.Y -= falling;
        b->transform.position.Y -= falling;
        c->transform.position.Y -= falling;
        d->transform.position.Y -= falling;

        e->transform.position.Y -= falling;
        f->transform.position.Y -= falling;
        g->transform.position.Y -= falling;
        h->transform.position.Y -= falling;

        if (a->transform.scale.X <= 0.0f) regenerate_quad(a);
        if (b->transform.scale.X <= 0.0f) regenerate_quad(b);
        if (c->transform.scale.X <= 0.0f) regenerate_quad(c);
        if (d->transform.scale.X <= 0.0f) regenerate_quad(d);
        if (e->transform.scale.X <= 0.0f) regenerate_quad(e);
        if (f->transform.scale.X <= 0.0f) regenerate_quad(f);
        if (g->transform.scale.X <= 0.0f) regenerate_quad(g);
        if (h->transform.scale.X <= 0.0f) regenerate_quad(h);

    }

    for (; i < start + end + remainder; i++)
    {

        quad_layout* a = array + i + 0;

        r32 scaling = SCALE_REDUCTION * delta_time;
        r32 falling = FALL_REDUCTION * delta_time;

        a->transform.scale.X -= scaling;
        a->transform.scale.Y = a->transform.scale.X;
        a->transform.position.Y -= falling;

        if (a->transform.scale.X <= 0.0f) regenerate_quad(a);

    }
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

    // Set up frame averaging.
    u32 frame_index = 0;
    r32 frame_times[128];
    for (u32 i = 0; i < 128; ++i) frame_times[i] = 1.0f/60.0f;

    // Initialize the quad renderer.
    i64 quads_rendered = 1;
    i64 quads_limit = 4000000;
    i64 quads_fast_increment = 10000;
    quad_render_buffer test_quad_renderer = {0};
    renderer2d_create_quad_render_context(&test_quad_renderer, &primary_arena, quads_limit);

    quad_layout* first = test_quad_renderer.vertex_buffer + 0;
    first->transform.position   = { 100.0f, 100.0f };
    first->transform.scale      = { 32.0f, 32.0f };
    first->texture.offset       = { subtexture_width * 0, subtexture_height * 3 };
    first->texture.dimension    = { subtexture_width, subtexture_height };

    // Initialize all the quads.
    for (u64 i = 0; i < quads_limit; ++i)
    {

        quad_layout* current = test_quad_renderer.vertex_buffer + i;
        regenerate_quad(current);

    }

    // Pre-activate the program and texture binding.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, base_texture); 
    glUseProgram(quad_program);

    // Runtime loop delta time.
    u64 frequency = system_timestamp_frequency();
    u64 frame_begin_time = system_timestamp();
    r32 frame_average = 1.0f / 60.0f;
    r32 frame_interval = 0.0f;
    r32 delta_time = 1.0f / 60.0f; // Default, for first frame.
    
    char window_title_buffer[100];

    // Standard runtime loop.
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

        // --- Game Logic ------------------------------------------------------
        //
        // Generally want this to occur before the render logic.
        //

        // Update the frame time.
        if (frame_index >= 128)
        {
            frame_index = 0;
        }

        frame_times[frame_index++] = delta_time;

        frame_interval += delta_time;
        if (frame_interval >= 0.33f)
        {
            for (u32 i = 0; i < 128; ++i) frame_average += frame_times[i];
            frame_average /= 128;
            frame_interval = 0.0f;
        }

        sprintf_s(window_title_buffer, 100, "Ninetails Game Engine - %.2f FPS - %llu",
                1.0f / frame_average, quads_rendered);
        window_set_title(window_title_buffer);

        if (input_key_is_pressed(NxKeyF))
        {
            
            printf("Deltatime is: %.8f or %.2f frames/second.\n", delta_time, 1.0f / delta_time);

        }

        if (input_key_is_pressed(NxKeyU))
        {

            for (u64 i = 0; i < quads_rendered; ++i)
            {

                quad_layout* current = test_quad_renderer.vertex_buffer + i;
                regenerate_quad(current);

            }

        }

        if (input_key_is_down(NxKeyControl) && input_key_is_pressed(NxKeyEnter))
        {

            b32 fullscreen = window_is_borderless_fullscreen();
            window_set_borderless_fullscreen(!fullscreen, 1280, 720);

        }

        if (input_key_is_pressed(NxKeyM))
        {
            
            if (quads_rendered == 0)
                quads_rendered = 1;
            else if (quads_rendered < 10)
            {
                i64 previous = quads_rendered;
                quads_rendered += 1;
                for (i64 idx = previous - 1; idx < quads_rendered; ++idx)
                    regenerate_quad(test_quad_renderer.vertex_buffer + idx);

            }
            else if (quads_rendered < 100)
            {
                i64 previous = quads_rendered;
                quads_rendered += 10;
                for (i64 idx = previous - 1; idx < quads_rendered; ++idx)
                    regenerate_quad(test_quad_renderer.vertex_buffer + idx);

            }
            else if (quads_rendered < 1000)
            {
                i64 previous = quads_rendered;
                quads_rendered += 100;
                for (i64 idx = previous - 1; idx < quads_rendered; ++idx)
                    regenerate_quad(test_quad_renderer.vertex_buffer + idx);

            }
            else if (quads_rendered < 10000)
            {
                i64 previous = quads_rendered;
                quads_rendered += 1000;
                for (i64 idx = previous - 1; idx < quads_rendered; ++idx)
                    regenerate_quad(test_quad_renderer.vertex_buffer + idx);

            }
            else if (quads_rendered < 100000)
            {
                i64 previous = quads_rendered;
                quads_rendered += 10000;
                for (i64 idx = previous - 1; idx < quads_rendered; ++idx)
                    regenerate_quad(test_quad_renderer.vertex_buffer + idx);

            }
            else
            {
                i64 previous = quads_rendered;
                quads_rendered += 100000;
                for (i64 idx = previous - 1; idx < quads_rendered; ++idx)
                    regenerate_quad(test_quad_renderer.vertex_buffer + idx);

            }

        }

        if (input_key_is_pressed(NxKeyN))
        {
            quads_rendered /= 10;
            if (quads_rendered <= 0) quads_rendered = 0;
        }

        update_quads_within_range(delta_time, 0, quads_rendered, test_quad_renderer.vertex_buffer);

        // --- Rendering -------------------------------------------------------
        //
        // Here is some rendering logic. Essentially, we update the OpenGL state
        // and fire off the quad renderer routine and clear the state.
        //

        glViewport(0, 0, window_get_width(), window_get_height());
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        u32 texture_slot = 0;
        opengl_shader_set_uniform_u32(quad_program, "u_texture_index", &texture_slot, 1);

        mat4 projection = orthographic_rh_no(0.0f, window_get_width(),
                0.0f, window_get_height(), 
                -10.0f, 10.0f);

        mat4 camera = translate({ 0.0f, 0.0f, 0.0f });

        opengl_shader_set_uniform_mat4(quad_program, "u_projection", &projection, 1);
        opengl_shader_set_uniform_mat4(quad_program, "u_camera", &camera, 1);

        test_quad_renderer.vertex_buffer_count = 1;
        renderer2d_render_quad_render_context(&test_quad_renderer, quads_rendered);

        // Swap the buffers at the end.
        window_swap_buffers();

        // Calculate the next frames delta time.
        u64 frame_end_time = system_timestamp();
        delta_time = system_timestamp_difference_ss(frame_begin_time, frame_end_time);

        // Prime the frame timer.
        frame_begin_time = system_timestamp();
        
    }

    window_close();

    return 0; // Return zero for success here.

}

