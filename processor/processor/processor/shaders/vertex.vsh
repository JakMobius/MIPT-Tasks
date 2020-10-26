
#version 330 core

uniform sampler2D u_texture;
layout(location = 0) in vec2 position;
out vec2 vertex_position;

void main()
{
    vertex_position = position * 0.5 + vec2(0.5, 0.5);
    gl_Position = vec4(position, 0.0, 1.0);
}
