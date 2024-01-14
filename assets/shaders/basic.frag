#version 450 core

layout (location = 0) in vec3 surfaceNormal;
layout (location = 1) in vec2 vTextureCoords;
layout (location = 2) in vec4 vColor;
layout (location = 3) in vec3 directionToLight;
layout (location = 4) in vec3 directionToCamera;
layout (location = 5) in float fogVisibility;
layout (location = 6) in vec4 worldPosition;
layout (location = 7) in vec3 worldNormal;

layout (location = 0) out vec4 fragColor;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureDiffuse2;
uniform sampler2D textureDiffuse3;
uniform sampler2D textureSpecular1;
uniform sampler2D textureNormal1;
uniform float useColor;
uniform vec4 color;
uniform float useBlending;
uniform float blendFactor;
uniform float useLighting;
uniform float useTextureAtlas;
uniform float numRows;
uniform vec2 offset;
uniform vec3 fogColor;
uniform float useNormal;
uniform float useReflection;
uniform float useSkyBlending;
uniform float skyBlendFactor;
uniform samplerCube textureDiffuseSky10;
uniform samplerCube textureDiffuseSky11;
uniform float reflectivity;
uniform vec3 cameraPosition;
uniform float useRefraction;
uniform float refractiveIndex;
uniform vec3 lightColor;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform float noTexture;
uniform vec3 materialColor;

void loadTexture() {
    if (int(noTexture) == 1) {
        fragColor = vec4(materialColor, 1.f);
    } else {
        vec2 offsetTexture = vTextureCoords;
        if (int(useTextureAtlas) == 1) {
            offsetTexture = vTextureCoords / numRows + offset;
        }
        vec4 texture1 = texture(textureDiffuse1, offsetTexture);
        if (int(useNormal) == 1) {
            texture1 = texture(textureNormal1, offsetTexture);
        }
        vec4 texture2 = texture(textureDiffuse2, offsetTexture);

        if (int(useColor) == 1) {
            if (int(useBlending) == 1) {
                fragColor = mix(texture1, texture2, blendFactor);
                fragColor = vec4(fragColor.rgb * color.rgb, color.a);
            } else {
                fragColor = color;
            }
        } else {
            if (int(useBlending) == 1) {
                fragColor = mix(texture1, texture2, blendFactor);
            } else {
                fragColor = texture1;
            }
        }
    }
}

void calculateTransparency() {
    if (fragColor.a < 0.1) {
        discard;
    }
}

void calculateReflection() {
    if (int(useReflection) == 1) {
        vec3 I = normalize(worldPosition.rgb - cameraPosition);
        vec3 R = reflect(I, normalize(worldNormal));
        vec4 skyTexture1 = texture(textureDiffuseSky11, R);
        vec4 reflectedSkyColor = skyTexture1;
        if (int(useSkyBlending) == 1) {
            vec4 skyTexture2 = texture(textureDiffuseSky10, R);
            reflectedSkyColor = mix(skyTexture2, skyTexture1, skyBlendFactor);
        }
        fragColor = mix(fragColor, reflectedSkyColor, reflectivity);
    }
}

void calculateRefraction() {
    if (int(useRefraction) == 1) {
        float ratio = 1.f / refractiveIndex;
        vec3 I = normalize(worldPosition.rgb - cameraPosition);
        vec3 R = refract(I, normalize(worldNormal), ratio);
        vec4 skyTexture1 = texture(textureDiffuseSky11, R);
        vec4 refractedSkyColor = skyTexture1;
        if (int(useSkyBlending) == 1) {
            vec4 skyTexture2 = texture(textureDiffuseSky10, R);
            refractedSkyColor = mix(skyTexture2, skyTexture1, skyBlendFactor);
        }
        fragColor = mix(fragColor, refractedSkyColor, 0.5);
    }
}

void calculateLighting() {
    if (int(useLighting) == 1) {
        // ambient
        vec3 ambient_ = lightColor * ambient;

        // diffuse
        float diff = max(dot(normalize(surfaceNormal), normalize(directionToLight)), 0.f);
        vec3 diffuse_ = lightColor * (diff * diffuse);

        // specular
        vec3 viewDirection = normalize(cameraPosition - worldPosition.rgb);
        vec3 reflectDirection = reflect(-directionToLight, worldNormal);
        float spec = pow(max(dot(viewDirection, reflectDirection), 0.f), shininess);
        vec3 specular_ = lightColor * (spec * specular);

        vec3 result = ambient_ + diffuse_; //+ specular;
        fragColor = vec4(fragColor.rgb * result, fragColor.a);
    }
}

void calculateFogVisibility() {
    fragColor = mix(vec4(fogColor, 1.f), fragColor, fogVisibility);
}

void main() {
    loadTexture();
    calculateTransparency();
    calculateReflection();
    calculateRefraction();
    calculateLighting();
    calculateFogVisibility();
}
