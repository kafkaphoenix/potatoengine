#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    mat4 trans = projection * view * model;
    texCoords = inTexCoords;
    gl_Position = trans * vec4(pos, 1.0);
}