#version 330 core

uniform vec3 u_Color;
out vec4 out_color;

void main() {
    out_color = vec4(u_Color, 1.0);
}
