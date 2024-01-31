#version 450 core

layout (location = 0) in vec2 vTextureCoords;

layout (location = 0) out vec4 fragColor;

uniform sampler2D textureDiffuse1;
uniform float useTextureAtlas;
uniform float numRows;
uniform vec2 offset;
uniform float useColor;
uniform vec4 color;

void loadTexture() {
    if (int(useColor) == 0) {
        vec2 offsetTexture = vTextureCoords;
        if (int(useTextureAtlas) == 1) {
            offsetTexture = vTextureCoords / numRows + offset;
        }
        fragColor = texture(textureDiffuse1, offsetTexture);
    } else {
        fragColor = color;
    }
}

void calculateTransparency() {
    if (fragColor.a < 0.1) {
        discard;
    }
}

void main()
{
    loadTexture();
    calculateTransparency();
}
