#version 460 core

layout (location = 0) in vec3 vTextureCoords;

layout (location = 0) out vec4 fragColor;

uniform samplerCube textureDiffuse1;
uniform samplerCube textureDiffuse2;
uniform float useBlending;
uniform float blendFactor;
uniform float useFog;
uniform vec3 fogColor;

const float LOWER_LIMIT = 0.f; // center of the screen
const float UPPER_LIMIT = 30.f; // slightly above the horizon

void main()
{
	vec4 texture1 = texture(textureDiffuse1, vTextureCoords);
	vec4 texture2 = texture(textureDiffuse2, vTextureCoords);
	vec4 finalColor = texture2;

	if (int(useBlending) == 1) {
  		finalColor = mix(texture1, texture2, blendFactor);
	} 
	
	if (int(useFog) == 1) {
		float fog_factor = clamp((vTextureCoords.y - LOWER_LIMIT) / (UPPER_LIMIT - LOWER_LIMIT), 0.f, 1.f);
		finalColor = mix(vec4(fogColor, 1.f), finalColor, fog_factor);
	}

	fragColor = finalColor;
}