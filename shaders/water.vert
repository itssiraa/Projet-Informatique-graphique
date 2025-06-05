#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 texCoords;
out vec3 fragPos;

uniform sampler2D wavesHeightMap;
uniform float time;
uniform float texScale; 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec2 uv = aTexCoord * texScale + vec2(time * 0.02, time * 0.005); // échelle + animation
    float height = texture(wavesHeightMap, uv).r;

    vec3 pos = vec3(aPos.x, height * 0.3, aPos.y);
    fragPos = vec3(model * vec4(pos, 1.0));
    texCoords = uv;

    gl_Position = projection * view * vec4(fragPos, 1.0);
}