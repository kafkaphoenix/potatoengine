#version 450 core

layout (location = 0) in vec2 vTextureCoords;

layout (location = 0) out vec4 fragColor;

uniform sampler2D textureDiffuse1;
uniform float useTextureAtlas;
uniform float numRows;
uniform vec2 offset;

void main()
{
    vec2 offsetTexture = vTextureCoords;
    if (int(useTextureAtlas) == 1) {
        offsetTexture = vTextureCoords / numRows + offset;
    }
    vec4 texture = texture(textureDiffuse1, offsetTexture);

    fragColor = texture;
    if (fragColor.a < 0.1) {
        discard;
    }
}
