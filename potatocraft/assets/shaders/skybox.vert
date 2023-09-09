#version 460 core

layout (location = 0) in vec3 position;

layout (location = 0) out vec3 vTextureCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vTextureCoords = position * 500.f; // Scale up the texture coords to make the fog works
    mat4 modifiedView = view;
    modifiedView[3] = vec4(0.f, 0.f, 0.f, 1.f); // Remove translation
    vec4 worldPosition = model * vec4(position, 1.f);
    vec4 viewPosition = modifiedView * worldPosition;
    vec4 clipPosition = projection * viewPosition;
    gl_Position = clipPosition.xyww;
}