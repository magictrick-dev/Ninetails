#include <platform/opengl.h>
#include <glad/glad_wgl.h>

typedef struct opengl_context
{
    HWND    window_handle;
    HDC     device_context;
    HGLRC   render_context;
} opengl_context;

static inline opengl_context* get_opengl_context() { static opengl_context ctx = {0}; return &ctx; }

// --- OpenGL Helpers ----------------------------------------------------------
//
// Non-platform specific OpenGL helper functions.
//

GLuint  
opengl_shader_create(GLuint type)
{

    return glCreateShader(type);

}

b32     
opengl_shader_compile(GLuint id, ccptr source)
{

    GLuint shader_identifier = id;

    glShaderSource(shader_identifier, 1, &source, NULL);
    glCompileShader(shader_identifier);

    // Check compilation status.
    GLint compile_status = GL_FALSE;
    glGetShaderiv(shader_identifier, GL_COMPILE_STATUS, &compile_status);

    // Determine compilation status.
    if (compile_status == GL_FALSE)
    {

        // If the compilation failed, report why to the user through debug output.
        int info_log_length = 0;
        glGetShaderiv(shader_identifier, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0)
        {
            char *message_buffer = (char*)malloc(info_log_length);
            glGetShaderInfoLog(shader_identifier, info_log_length, NULL, message_buffer);
            printf("-- OpenGL shader compilation failed:\n%s\n", message_buffer);
            free(message_buffer);
        }

        // Exit.
        return false;

    }

    return true;

}

void    
opengl_shader_release(GLuint id)
{

    glDeleteShader(id);

}

GLuint  
opengl_program_create()
{

    return glCreateProgram();

}

void    
opengl_program_attach(GLuint program, GLuint shader)
{

    glAttachShader(program, shader);

}

b32     
opengl_program_link(GLuint program)
{

    GLuint program_identifier = program;

    glLinkProgram(program_identifier);
 
    GLint program_link_status = GL_FALSE;
    glGetProgramiv(program_identifier, GL_LINK_STATUS, &program_link_status);
    if (program_link_status == GL_FALSE)
    {
        
        int info_log_length = 0;
        glGetProgramiv(program_identifier, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0)
        {
            char *message_buffer = (char*)malloc(info_log_length + 1);
            glGetProgramInfoLog(program_identifier, info_log_length, NULL, message_buffer);
            printf("-- OpenGL shader link failed:\n%s\n", message_buffer);
            free(message_buffer);
        }

        return false;

    }

    return true;


}

void    
opengl_program_release(GLuint program)
{

    glDeleteProgram(program);

}

GLuint 
opengl_texture_create(image *img)
{

    GLuint texture_identifier = NULL;
    glGenTextures(1, &texture_identifier);
    glBindTexture(GL_TEXTURE_2D, texture_identifier);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,  img->width, img->height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, img->buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, NULL);
    return texture_identifier;

}

void 
opengl_texture_delete(GLuint texture_identifier)
{

    glDeleteTextures(1, &texture_identifier);
    return;

}

b32     
opengl_shader_set_uniform_mat4(GLuint program, ccptr loc, mat4 *source, u64 count)
{
    GLint uniform_location = glGetUniformLocation(program, loc);
    if (uniform_location == GL_INVALID_VALUE || uniform_location == -1)
        return false;
    glUniformMatrix4fv(uniform_location, count, GL_FALSE, &(*source)[0][0]);
    return true;
}

b32     
opengl_shader_set_uniform_vec2(GLuint program, ccptr loc, vec2 *source, u64 count)
{
    GLint uniform_location = glGetUniformLocation(program, loc);
    if (uniform_location == GL_INVALID_VALUE || uniform_location == -1)
        return false;
    glUniform2fv(uniform_location, count, &(*source)[0]);
    return true;
}

b32     
opengl_shader_set_uniform_vec3(GLuint program, ccptr loc, vec3 *source, u64 count)
{
    GLuint uniform_location = glGetUniformLocation(program, loc);
    if (uniform_location == GL_INVALID_VALUE || uniform_location == -1)
        return false;
    glUniform3fv(uniform_location, count, &(*source)[0]);
    return true;
}

b32     
opengl_shader_set_uniform_vec4(GLuint program, ccptr loc, vec4 *source, u64 count)
{
    GLint uniform_location = glGetUniformLocation(program, loc);
    if (uniform_location == GL_INVALID_VALUE || uniform_location == -1)
        return false;
    glUniform4fv(uniform_location, count, &(*source)[0]);
    return true;
}

b32     
opengl_shader_set_uniform_r32(GLuint program, ccptr loc, r32 *source, u64 count)
{
    GLint uniform_location = glGetUniformLocation(program, loc);
    if (uniform_location == GL_INVALID_VALUE || uniform_location == -1)
        return false;
    glUniform1fv(uniform_location, count, (source));
    return true;
}

b32     
opengl_shader_set_uniform_i32(GLuint program, ccptr loc, i32 *source, u64 count)
{
    GLint uniform_location = glGetUniformLocation(program, loc);
    if (uniform_location == GL_INVALID_VALUE || uniform_location == -1)
        return false;
    glUniform1iv(uniform_location, count, (source));
    return true;
}

b32     
opengl_shader_set_uniform_u32(GLuint program, ccptr loc, u32 *source, u64 count)
{
    GLint uniform_location = glGetUniformLocation(program, loc);
    if (uniform_location == GL_INVALID_VALUE || uniform_location == -1)
        return false;
    glUniform1uiv(uniform_location, count, (source));
    return true;
}


// --- Platform OpenGL ---------------------------------------------------------
//
// Although OpenGL is platform agnostic, initializing it and setting its contexts
// is not, so the following front-end API provides this functionality.
//

static inline b32 
render_context_check_last_error()
{

    DWORD errorMessageID = GetLastError();
    if (errorMessageID != 0) 
    {
    
        LPSTR messageBuffer = NULL;

        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR)&messageBuffer, 0, NULL);
        
        printf("-- OpenGL Context Creation Error: %s\n", messageBuffer);
        LocalFree(messageBuffer);

        return true;

    }

    return false;

}

b32 
create_opengl_render_context(vptr window_handle)
{

    NX_ASSERT(window_handle != NULL);
    opengl_context* ctx = get_opengl_context();

    if (ctx->render_context != NULL) return true; // Already established.

    // Get the device context from the window.
    HDC device_context = GetDC((HWND)window_handle);

    // Set the pixel format descriptor hoopla.
    PIXELFORMATDESCRIPTOR pfd = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };


    // This is the goofiest way to do this.
    int pixel_format = ChoosePixelFormat(device_context, &pfd);
    SetPixelFormat(device_context, pixel_format, &pfd);
    if (render_context_check_last_error()) return false;

    // Intiialize OpenGL using GLAD.
    HGLRC opengl_render_context = wglCreateContext(device_context);
    if (render_context_check_last_error()) return false;

    wglMakeCurrent(device_context, opengl_render_context);
    if (render_context_check_last_error()) return false;

    gladLoadGL();
    gladLoadWGL(device_context);

    // Finally, set our contexts.
    ctx->window_handle = (HWND)window_handle;
    ctx->device_context = device_context;
    ctx->render_context = opengl_render_context;

    return true;

}

b32
set_opengl_vertical_sync(i32 interval)
{

    b32 result = wglSwapIntervalEXT(interval);
    return result;


}
