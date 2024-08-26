#version 430 core

uniform uint v_texture_index;
in vec2 v_uv;
out vec4 fragment_color;

void main()
{
    fragment_color = vec4(v_uv, 0, 1);
}
