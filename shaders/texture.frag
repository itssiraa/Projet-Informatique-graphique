#version 330 core

in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D diffuse_map;

void main()
{
    fragColor = texture(diffuse_map, texCoords);
}
