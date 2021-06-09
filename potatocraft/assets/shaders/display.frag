#version 460
out vec4 color;

in  vec2 vTexCoords;

uniform sampler2D texSampler;

void main()
{
    color = texture(texSampler, vTexCoords);
}
