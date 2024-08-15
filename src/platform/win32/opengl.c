#include <platform/opengl.h>
#include <glad/glad_wgl.h>

typedef struct opengl_context
{
    HWND    window_handle;
    HDC     device_context;
    HGLRC   render_context;
} opengl_context;

static inline opengl_context* get_opengl_context() { static opengl_context ctx = {0}; return &ctx; }

// --- Platform OpenGL ---------------------------------------------------------
//
// Although OpenGL is platform agnostic, initializing it and setting its contexts
// is not, so the following front-end API provides this functionality.
//

b32 
create_opengl_render_context(vptr window_handle)
{

    NX_ASSERT(window_handle != NULL);
    opengl_context* ctx = get_opengl_context();

    if (ctx->render_context != NULL) return true; // Already established.

    // Get the device context from the window.
    HDC device_context = GetDC(window_handle);

    // Set the pixel format descriptor hoopla.
    PIXELFORMATDESCRIPTOR pfd   = {0};
    pfd.nSize                   = sizeof(pfd);
    pfd.nVersion                = 1;
    pfd.dwFlags                 = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType              = PFD_TYPE_RGBA;
    pfd.cColorBits              = 32;

    // This is the goofiest way to do this.
    int pixel_format = ChoosePixelFormat(device_context, &pfd);
    NX_ASSERT(pixel_format != 0);
    NX_ASSERT(SetPixelFormat(device_context, pixel_format, &pfd));

    // Intiialize OpenGL using GLAD.
    HGLRC opengl_render_context = wglCreateContext(device_context);
    wglMakeCurrent(device_context, opengl_render_context);
    gladLoadGL();
    gladLoadWGL(device_context);

    // Finally, set our contexts.
    ctx->window_handle = window_handle;
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
