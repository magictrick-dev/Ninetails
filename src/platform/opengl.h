#ifndef SRC_PLATFORM_OPENGL_H
#define SRC_PLATFORM_OPENGL_H
#include <core/definitions.h>
#include <glad/glad.h>

// --- Context Helpers ---------------------------------------------------------

b32 create_opengl_render_context(vptr window_handle);
b32 set_opengl_vertical_sync(i32 interval);

// --- Shader Helpers ----------------------------------------------------------

GLuint  opengl_shader_create(GLuint type);
b32     opengl_shader_compile(GLuint id, ccptr source);
void    opengl_shader_release(GLuint id);
GLuint  opengl_program_create();
void    opengl_program_attach(GLuint program, GLuint shader);
b32     opengl_program_link(GLuint program);
void    opengl_program_release(GLuint program);


#endif
