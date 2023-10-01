#version 450 core

layout (location = 0) in vec2 vTextureCoords;
layout (location = 1) in float fogVisibility;

layout (location = 0) out vec4 fragColor;

uniform sampler2D textureDiffuse1;
uniform float useAtlas;
uniform float numRows;
uniform vec2 offset;
uniform vec3 fogColor;

void main()
{
    vec2 offsetTexture = vTextureCoords;
    if (int(useAtlas) == 1) {
        offsetTexture = vTextureCoords / numRows + offset;
    }
    vec4 texture1 = texture(textureDiffuse1, offsetTexture);

    fragColor = mix(vec4(fogColor, 1.f), texture1, fogVisibility);
}
