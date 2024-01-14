#version 450 core

layout (location = 0) in vec2 vTextureCoords;
layout (location = 1) in float fogVisibility;

layout (location = 0) out vec4 fragColor;

uniform sampler2D textureDiffuse1;
uniform float useTextureAtlas;
uniform float numRows;
uniform vec2 offset;
uniform vec3 fogColor;
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

void calculateFogVisibility() {
    fragColor = mix(vec4(fogColor, 1.f), fragColor, fogVisibility);
}

void main() {
    loadTexture();
    calculateTransparency();
    calculateFogVisibility();
}
