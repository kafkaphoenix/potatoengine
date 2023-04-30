#version 460 core

layout (location = 0) in vec4 vColor;
layout (location = 1) in vec2 vTexCoord;

layout (location = 0) out vec4 fragColor;

uniform sampler2D texSampler;

void main()
{
    fragColor = vColor * texture(texSampler, vTexCoord);
}