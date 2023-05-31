#version 460 core

layout (location = 0) in vec3 surfaceNormal;
layout (location = 1) in vec2 vTexCoords;

layout (location = 0) out vec4 fragColor;

uniform vec4 colorFactor;
uniform sampler2D mtexture;
uniform vec3 lightDir;

void main()
{
    vec4 base = colorFactor * texture(mtexture, vTexCoords);
    float nDotL = dot(normalize(surfaceNormal), normalize(lightDir));
    float brightness = max(nDotL, 0.0);
    fragColor = vec4(base.rgb * brightness, base.a);
}