#ifndef SRC_PLATFORM_OPENGL_H
#define SRC_PLATFORM_OPENGL_H
#include <core/definitions.h>
#include <core/linear.h>
#include <glad/glad.h>

// --- Context Helpers ---------------------------------------------------------

b32 create_opengl_render_context(vptr window_handle);
b32 set_opengl_vertical_sync(i32 interval);

// --- Shader Helpers ----------------------------------------------------------

GLuint  opengl_shader_create(GLuint type);
b32     opengl_shader_compile(GLuint id, ccptr source);
void    opengl_shader_release(GLuint id);

GLuint  opengl_program_create();
b32     opengl_program_link(GLuint program);
void    opengl_program_attach(GLuint program, GLuint shader);
void    opengl_program_release(GLuint program);

b32     opengl_shader_set_uniform_mat4(GLuint program, ccptr loc, mat4 *source, u64 count);
b32     opengl_shader_set_uniform_vec2(GLuint program, ccptr loc, vec2 *source, u64 count);
b32     opengl_shader_set_uniform_vec3(GLuint program, ccptr loc, vec3 *source, u64 count);
b32     opengl_shader_set_uniform_vec4(GLuint program, ccptr loc, vec4 *source, u64 count);
b32     opengl_shader_set_uniform_r32(GLuint program, ccptr loc, r32 *source, u64 count);
b32     opengl_shader_set_uniform_i32(GLuint program, ccptr loc, i32 *source, u64 count);
b32     opengl_shader_set_uniform_u32(GLuint program, ccptr loc, u32 *source, u64 count);


// --- Texture Helpers ---------------------------------------------------------

GLuint opengl_texture_create(image *img);
void opengl_texture_delete(GLuint texture_identifier);

#endif
