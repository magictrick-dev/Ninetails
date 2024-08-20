#ifndef SRC_ENGINE_PRIMITIVES_H
#define SRC_ENGINE_PRIMITIVES_H
#include <core/definitions.h>
#include <platform/opengl.h>
#include <platform/filesystem.h>
#include <engine/runtime.h>

r32 cube_vertices[] = {
    -1.0f,  1.0f,  0.0f,     0.0f,  0.0f,  1.0f,
    -1.0f, -1.0f,  0.0f,     0.0f,  0.0f,  1.0f,
     1.0f, -1.0f,  0.0f,     0.0f,  0.0f,  1.0f,
     1.0f,  1.0f,  0.0f,     0.0f,  0.0f,  1.0f
};

u32 cube_indices[] = {
    0, 1, 2, 0, 2, 3
};

void cube()
{

    static b32 initialized = false;
    static GLuint cube_vertex_buffer;
    static GLuint cube_program;

    if (initialized == false)
    {

        // The buffer.
        glGenBuffers(1, &cube_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(r32) * 9, cube_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Pull the sources into memory.
        ccptr cube_vertex_shader = NULL;
        ccptr cube_fragment_shader = NULL;

        memory_arena *primary_arena = runtime_get_primary_arena();
        u64 arena_save_state = memory_arena_save(primary_arena);

        ccptr vertex_shader_path = "res/primitive_cube_vtx.glsl";
        ccptr fragment_shader_path = "res/primitive_cube_frg.glsl";

        if (file_exists(vertex_shader_path))
        {
            u64 size = file_size(vertex_shader_path);
            cptr file_buffer = (cptr)memory_arena_push(primary_arena, size + 1);
            file_read_all(vertex_shader_path, file_buffer, size + 1);
            file_buffer[size] = '\0';
            cube_vertex_shader = (ccptr)file_buffer;
        }

        if (file_exists(fragment_shader_path))
        {
            u64 size = file_size(fragment_shader_path);
            cptr file_buffer = (cptr)memory_arena_push(primary_arena, size + 1);
            file_read_all(fragment_shader_path, file_buffer, size + 1);
            file_buffer[size] = '\0';
            cube_fragment_shader = (ccptr)file_buffer;
        }

        NX_ASSERT(cube_vertex_shader != NULL);
        NX_ASSERT(cube_fragment_shader != NULL);

        // Compiling and linking the shader program.
        GLuint vertex_shader = opengl_shader_create(GL_VERTEX_SHADER);
        GLuint fragment_shader = opengl_shader_create(GL_FRAGMENT_SHADER);
        opengl_shader_compile(vertex_shader, cube_vertex_shader);
        opengl_shader_compile(fragment_shader, cube_fragment_shader);
        cube_program = opengl_program_create();
        opengl_program_attach(cube_program, vertex_shader);
        opengl_program_attach(cube_program, fragment_shader);
        opengl_program_link(cube_program);
        opengl_shader_release(vertex_shader);
        opengl_shader_release(fragment_shader);

        // Restore the arena state.
        memory_arena_restore(primary_arena, arena_save_state);

        initialized = true;

    }

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_buffer);
    glUseProgram(cube_program);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (vptr)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glUseProgram(0);

}

#endif
