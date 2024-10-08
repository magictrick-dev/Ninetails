#version 430 core
layout (location = 0) in vec2 in_position;
layout (location = 1) in vec2 in_texture_coordinates;
layout (location = 2) in vec2 v_position;
layout (location = 3) in vec2 v_scale;
layout (location = 4) in vec2 v_texture_offset;
layout (location = 5) in vec2 v_texture_dimensions;

uniform mat4 u_projection;
uniform mat4 u_camera;

out vec2 v_texture_uv;
out vec2 v_original_uv;

mat4 mat4_from_position(vec2 position)
{

    mat4 position_matrix;
    position_matrix[0] = vec4( 1, 0, 0, 0 );
    position_matrix[1] = vec4( 0, 1, 0, 0 );
    position_matrix[2] = vec4( 0, 0, 1, 0 );
    position_matrix[3] = vec4(position, 0, 1.0f);

    return position_matrix;

}

mat4 mat4_from_scale(vec2 scale)
{

    mat4 scale_matrix;
    scale_matrix[0] = vec4( scale.x, 0, 0, 0 );
    scale_matrix[1] = vec4( 0, scale.y, 0, 0 );
    scale_matrix[2] = vec4( 0, 0, 1, 0 );
    scale_matrix[3] = vec4( 0, 0, 0, 1 );

    return scale_matrix;

}

void main()
{

    // Some trickery to generate the proper UVs depending on which of the four
    // vertices we're located at in the instance we're rendering.
    vec2 instance_uvs[4];
    instance_uvs[0] = v_texture_offset; 

    instance_uvs[1] = v_texture_offset; 
    instance_uvs[1].y += v_texture_dimensions.y;

    instance_uvs[2] = v_texture_offset;
    instance_uvs[2] += v_texture_dimensions;

    instance_uvs[3] = v_texture_offset; 
    instance_uvs[3].x += v_texture_dimensions.x;

    int index = gl_VertexID % 4;
    v_texture_uv = instance_uvs[index];
    v_original_uv = in_texture_coordinates;

    mat4 translate_matrix = mat4_from_position(v_position);
    mat4 scale_matrix = mat4_from_scale(v_scale);
    gl_Position = u_projection * u_camera * translate_matrix * scale_matrix * vec4(in_position, 0.0f, 1.0f);

}


