#include <engine/renderers/quad2d.h>

void 
renderer2d_create_quad_render_context(quad_render_buffer *buffer, memory_arena *arena, u64 count)
{

    NX_ENSURE_POINTER(buffer);
    NX_ENSURE_POINTER(arena);

    quad_layout *quad_buffer = memory_arena_push_array(arena, quad_layout, count);
    buffer->vertex_buffer_size          = sizeof(quad_layout) * count;
    buffer->vertex_buffer_count         = count;
    buffer->vertex_buffer               = quad_buffer;
    buffer->index_buffer[0] = 0;
    buffer->index_buffer[1] = 1;
    buffer->index_buffer[2] = 2;
    buffer->index_buffer[3] = 0;
    buffer->index_buffer[4] = 2;
    buffer->index_buffer[5] = 3;
    
    quad_mesh *mesh = memory_arena_push_type(arena, quad_mesh);
    mesh->bottom_left.position     = { -0.5f, -0.5f };
    mesh->top_left.position        = { -0.5f,  0.5f };
    mesh->top_right.position       = {  0.5f,  0.5f };
    mesh->bottom_right.position    = {  0.5f, -0.5f };
    mesh->bottom_left.texture      = {  0.0f,  0.0f };
    mesh->top_left.texture         = {  0.0f,  1.0f };
    mesh->top_right.texture        = {  1.0f,  1.0f };
    mesh->bottom_right.texture     = {  1.0f,  0.0f };
    buffer->mesh = mesh;

    glGenVertexArrays(1, &buffer->vao);
    glGenBuffers(1, &buffer->instance_vbo);
    glGenBuffers(1, &buffer->vbo);
    glGenBuffers(1, &buffer->ibo);

    glBindVertexArray(buffer->vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_mesh), mesh, GL_STATIC_DRAW);   
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, buffer->vertex_buffer_size, quad_buffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * 6, buffer->index_buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float)*2));

    glBindBuffer(GL_ARRAY_BUFFER, buffer->instance_vbo);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*2));
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*4));
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*6));
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    glBindVertexArray(NULL);

}

void 
renderer2d_delete_quad_render_context(quad_render_buffer *buffer)
{

    assert(buffer != NULL);
    glDeleteBuffers(1, &buffer->vbo);
    glDeleteBuffers(1, &buffer->instance_vbo);
    glDeleteBuffers(1, &buffer->ibo);
    glDeleteVertexArrays(1, &buffer->vao);

    buffer->vbo             = NULL;
    buffer->instance_vbo    = NULL;
    buffer->ibo             = NULL;
    buffer->vao             = NULL;

}

void 
renderer2d_render_quad_render_context(quad_render_buffer *buffer, u64 count)
{

    glBindVertexArray(buffer->vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->instance_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad_layout) * count, buffer->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ibo);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, count);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ibo);
    glBindVertexArray(buffer->vao);

}


