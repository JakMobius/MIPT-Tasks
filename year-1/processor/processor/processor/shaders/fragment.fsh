
#version 330 core

uniform sampler2D u_texture;
in vec2 vertex_position;
out vec4 color;

void main(){
    color = vec4(texture(u_texture, vertex_position).xyz, 1.0);
}
