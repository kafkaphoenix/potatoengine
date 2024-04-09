#version 450 core
layout (location = 0) in vec3 position;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    vec4 worldPosition = model * vec4(position.x, position.y, position.z, 1.f);
    vec4 viewPosition = view * worldPosition;
    vec4 clipPosition = projection * viewPosition;
    gl_Position = clipPosition;
}