#version 450 core

layout (location = 0) in vec3 surfaceNormal;
layout (location = 1) in vec2 vTextureCoords;
layout (location = 2) in vec3 vColor;
layout (location = 3) in vec3 directionToLight;
layout (location = 4) in vec3 directionToCamera;
layout (location = 5) in float fogVisibility;
layout (location = 6) in vec4 worldPosition;
layout (location = 7) in vec3 worldNormal;

layout (location = 0) out vec4 fragColor;

uniform sampler2D textureDiffuse1;
uniform float useColor;
uniform float useLighting;
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

void main()
{

    vec4 base = vec4(0.f);
    if (int(useColor) == 1) {
        base = vec4(vColor, 1.f);
    } else {
        base = texture(textureDiffuse1, vTextureCoords);
    }

    if (int(useReflection) == 1) {
        vec3 I = normalize(worldPosition.rgb - cameraPosition);
        vec3 R = reflect(I, normalize(worldNormal));
        vec4 skyTexture1 = texture(textureDiffuseSky11, R);
        vec4 reflectedSkyColor = skyTexture1;
        if (int(useSkyBlending) == 1) {
            vec4 skyTexture2 = texture(textureDiffuseSky10, R);
            reflectedSkyColor = mix(skyTexture2, skyTexture1, skyBlendFactor);
        }
        base = mix(base, reflectedSkyColor, reflectivity);
    }

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
        base = mix(base, refractedSkyColor, 0.5);
    }

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
        base = vec4(base.rgb * result, base.a);
    }

    fragColor = mix(vec4(fogColor, 1.f), base, fogVisibility);
    if (fragColor.a < 0.1) {
        discard;
    }
}
