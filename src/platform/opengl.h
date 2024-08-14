#ifndef SRC_PLATFORM_OPENGL_H
#define SRC_PLATFORM_OPENGL_H
#include <core/definitions.h>
#include <glad/glad.h>

b32 create_opengl_render_context(vptr window_handle);
b32 set_opengl_vertical_sync(i32 interval);

#endif
