#ifndef SRC_ENGINE_RENDERERS_QUAD2D_H
#define SRC_ENGINE_RENDERERS_QUAD2D_H
#include <core/definitions.h>
#include <core/linear.h>
#include <core/arena.h>
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
    quad_mesh *mesh;                // Instance mesh.
    quad_layout *vertex_buffer;     // The buffer of quads.
    GLuint vao;                     // Vertex array object.
    GLuint vbo;                     // Vertex buffer object.
    GLuint instance_vbo;            // Per-instance vbo.
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
// Shader Program Reference:
//      layout (location = 0) in vec2 in_position;
//      layout (location = 1) in vec2 in_texture_coordinates;
//      layout (location = 2) in vec2 v_position;
//      layout (location = 3) in vec2 v_scale;
//      layout (location = 4) in vec2 v_texture_offset;
//      layout (location = 5) in vec2 v_texture_dimensions;
//      gl_VertexID will provide which of the four mesh coordinates you are on.
//

void renderer2d_create_quad_render_context(quad_render_buffer *buffer, memory_arena *arena, u64 count);
void renderer2d_delete_quad_render_context(quad_render_buffer *buffer);
void renderer2d_render_quad_render_context(quad_render_buffer *buffer, u64 count);

#endif
