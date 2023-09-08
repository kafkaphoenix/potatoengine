#version 460 core

layout (location = 0) in vec3 surfaceNormal;
layout (location = 1) in vec2 vTextureCoords;
layout (location = 2) in vec4 vColor;

layout (location = 0) out vec4 fragColor;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular0;
uniform float use_color;
uniform float use_texture_mix;
uniform float texture_mix_factor;
uniform vec3 light_dir;
uniform float use_atlas;
uniform float num_rows;
uniform vec2 offset;

void main()
{
    vec2 offsetTexture = vTextureCoords;
    if (int(use_atlas) == 1) {
        offsetTexture = vTextureCoords / num_rows + offset;
    }

    vec4 base = vec4(0.0);
    if (int(use_color) == 1) {
        if (int(use_texture_mix) == 1) {
            base = vColor * mix(vec4(1.0), texture(texture_diffuse0, offsetTexture), texture_mix_factor);
        } else {
            base = vColor;
        }
    } else {
        base = texture(texture_diffuse0, offsetTexture);
    }
    float nDotL = dot(normalize(surfaceNormal), normalize(light_dir));
    float brightness = max(nDotL, 0.0);
    fragColor = vec4(base.rgb * brightness, base.a);
}