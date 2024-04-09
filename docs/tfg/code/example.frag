#version 450 core
layout (location = 0) out vec4 fragColor;

void calculateTransparency() {
    if (fragColor.a < 0.1) {
        discard;
    }
}

void main() {
    fragColor = color;
    calculateTransparency();
}