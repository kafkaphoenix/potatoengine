#version 460 core

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

uniform mat4 viewProjection;
uniform mat4 transform;

void main()
{
    gl_Position = viewProjection * transform * vec4(position, 1.0);
    surfaceNormal = (transform * vec4(normal, 0.0)).xyz;
    vTextureCoords = textureCoords;
    vColor = color;
}