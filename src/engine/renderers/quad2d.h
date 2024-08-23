#ifndef SRC_ENGINE_RENDERERS_QUAD2D_H
#define SRC_ENGINE_RENDERERS_QUAD2D_H
#include <core/definitions.h>
#include <platform/opengl.h>

typedef struct quad_mesh_vertex
{
    vec2 position;
    vec2 texture;
} quad_mesh_vertex;

typedef union quad_mesh
{

    quad_mesh_vertex vertices[4];

    struct
    {
        quad_mesh_vertex bottom_left;
        quad_mesh_vertex top_left;
        quad_mesh_vertex top_right;
        quad_mesh_vertex bottom_right;
    };

} quad_mesh;

typedef struct quad_layout
{

    struct
    {
        vec2 position;
        vec2 scale;
    } transform;

    struct
    {
        vec2 offset;
        vec2 dimension;
    } texture;

} quad_layout;

typedef struct quad_render_buffer
{
    u32 index_buffer[6];            // Indexing order of vertices.
    u32 vertex_buffer_size;         // How many slots are available.
    u32 vertex_buffer_count;        // How many are actually used.
    quad_layout *vertex_buffer;     // The buffer of quads.
    GLuint vao;                     // Vertex array object.
    GLuint vbo;                     // Vertex buffer object.
    GLuint ibo;                     // Index buffer object.
} quad_render_buffer;

// --- Renderer2D Quad Renderer ------------------------------------------------
//
// The quad renderer is an instanced renderer which takes regular 2D quads and
// quickly renders them to the screen. It's one of the fastest ways to deliver
// quads to the screen since it reduces the per-vertex footprint down to its
// core transformational components.
//
// You can use this to easily render off a single sprite sheet very quickly.
// This setup has been tested to handle over one million quads (2 million triangles)
// over 60FPS. This is pretty typical for modern GPUs.
//
// Here are some numbers for you:
//
//      Assuming we have a 512x512 sprite sheet with 32x32 tiles, you have 256
//      tiles to work with. Fullscreen 1080p displays can fit roughly ~2025 tiles
//      at a 1-to-1 scale.
//
//      You can effectively render ~493 tilemap layers before hitting the rendering
//      limits of a reference GTX 1080. That's a lot of layers.
//
//      This isn't even the most optimized way to do this. If you make assumptions
//      about tile positions, you rework the quad layout to use 16-bit signed integers,
//      essentially allowing you to fit the entire transformation packed in 8 bytes.
//      That is half the amount of information of the origin float-based quad mesh.
//
//      Wait, there's more!
//
//      Lets assume that you're using 16-bit signed integer for transformations.
//      Now, what if we wanted more than sprite sheet? Well, OpenGL gives us 16
//      slots guaranteed, lets reserve 1 for default, giving us 15 texture slots
//      to play with. Add a single u8 to the quad layout, setting up for 9 bytes
//      of information. We can do better. A full 1080 display width-wise fits 60
//      tiles. A single 8-bit byte fits that. Height as well. Scale is pixel based
//      and tiles are less than 256, so we can fit all transformation data in a
//      byte of data each. That 4 bytes + 1 byte for texture. That's 5 bytes.
//
//      This means a fully optimized sprite-based 32x32 pixel tilemap renderer
//      can render 15 sprite sheets + 1 fall-back NULL texture, with 5-bytes for
//      each instance rendered. That is more than a 66% reduction from the original
//      float-based layout PLUS it renders more sheets than original. The amount
//      of sprites that can be rendered at the point is so much that you could
//      dynamically render the whole scene every frame without stitching the
//      tilemaps to save on performance.
//
//      Why would you do this?
//
//      Well, you probably *wouldn't* given that stitching tilemaps into textures
//      is actually the most sane way to go about this, but it goes to show that
//      what you see here is actually the appetizer for things you can do.
//

void renderer2d_create_quad_render_context(quad_render_buffer *buffer, memory_arena *arena, u64 count);
void renderer2d_delete_quad_render_context(quad_render_buffer *buffer);
void renderer2d_render_quad_render_context(quad_render_buffer *buffer);

#endif
