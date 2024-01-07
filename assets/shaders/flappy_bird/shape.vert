#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

layout (location = 0) out vec2 vTextureCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 worldPosition = model * vec4(position.x, position.y, position.z, 1.f);
    vec4 viewPosition = view * worldPosition;
    vec4 clipPosition = projection * viewPosition;
    gl_Position = clipPosition;
    vTextureCoords = textureCoords;
}