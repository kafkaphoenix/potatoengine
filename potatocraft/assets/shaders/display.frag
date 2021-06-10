#version 460

out vec4 color;
in vec2 texCoords;

uniform sampler2D texSampler;

void main()
{
    color = texture(texSampler, texCoords);
}