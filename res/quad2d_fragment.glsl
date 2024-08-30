#version 430 core

uniform sampler2D u_texture_index;
in vec2 v_texture_uv;
in vec2 v_original_uv;
out vec4 fragment_color;

void main()
{

    //fragment_color = vec4(v_original_uv, 0, 1);
    fragment_color = texture(u_texture_index, v_texture_uv);

}
