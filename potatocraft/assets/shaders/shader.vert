#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;

layout (location = 0) out vec4 vColor;
layout (location = 1) out vec2 vTexCoord;

uniform mat4 viewProjection;
uniform mat4 transform;

void main()
{
    gl_Position = viewProjection * transform * vec4(position, 1.0);
    vColor = color;
    vTexCoord  = texCoord;
}