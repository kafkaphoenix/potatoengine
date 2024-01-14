#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

layout (location = 0) out vec2 vTextureCoords;
layout (location = 1) out float fogVisibility;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float useFog;
uniform float fogDensity;
uniform float fogGradient;

void calculateFogVisibility(vec4 viewPosition) {
    if (int(useFog) == 0) {
        fogVisibility = 1.f;
    } else {
        float distanceRelativeToCamera = length(viewPosition.xyz);
        fogVisibility = exp(-pow((distanceRelativeToCamera * fogDensity), fogGradient));
        fogVisibility = clamp(fogVisibility, 0.f, 1.f);
    }
}

void main() {
    vec4 worldPosition = model * vec4(position.x, position.y, position.z, 1.f);
    vec4 viewPosition = view * worldPosition;
    vec4 clipPosition = projection * viewPosition;
    gl_Position = clipPosition;
    vTextureCoords = textureCoords;

    calculateFogVisibility(viewPosition);
}