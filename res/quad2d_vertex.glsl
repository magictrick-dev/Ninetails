#version 430 core
layout (location = 0) in vec2 in_position;
layout (location = 1) in vec2 in_texture_coordinates;
layout (location = 2) in vec2 v_position;
layout (location = 3) in vec2 v_scale;
layout (location = 4) in vec2 v_texture_offset;
layout (location = 5) in vec2 v_texture_dimensions;

uniform mat4 projection;
uniform mat4 camera;

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

}


