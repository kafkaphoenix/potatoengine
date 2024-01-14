#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in int boneIDs;
layout (location = 6) in float boneWeights;
layout (location = 7) in vec4 color;

layout (location = 0) out vec3 surfaceNormal;
layout (location = 1) out vec2 vTextureCoords;
layout (location = 2) out vec4 vColor;
layout (location = 3) out vec3 directionToLight;
layout (location = 4) out vec3 directionToCamera;
layout (location = 5) out float fogVisibility;
layout (location = 6) out vec4 worldPosition;
layout (location = 7) out vec3 worldNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPosition;
uniform float useFog;
uniform float fogDensity;
uniform float fogGradient;

void calculateReflection() {
    surfaceNormal = (model * vec4(normal, 0.f)).xyz;
    directionToLight = lightPosition - worldPosition.xyz;
    directionToCamera = (inverse(view) * vec4(0.f, 0.f, 0.f, 1.f)).xyz - worldPosition.xyz;
}

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
    worldPosition = model * vec4(position, 1.f);
    worldNormal = mat3(transpose(inverse(model))) * normal;
    vec4 viewPosition = view * worldPosition;
    vec4 clipPosition = projection * viewPosition;
    gl_Position = clipPosition;
    vTextureCoords = textureCoords;
    vColor = color;

    calculateReflection();
    calculateFogVisibility(viewPosition);
}