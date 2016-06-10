#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;

in vec2 outUV;

out vec4 color;

uniform vec3  objectColour;
uniform float objectOpacity;

vec3 lightColour   = vec3(1.0f);
vec3 lightPosition = vec3(0);
float K_c = 0.0f;
float K_l = 0.0f;
float K_q = 1.0f;

float ambientStrength = 0.1f;

uniform sampler2D planetTexture;

void main() {
    // Ambient lighting.
    vec3 ambientLight = ambientStrength * lightColour;

    // Diffuse lighting.
    vec3 normal     = normalize(fragmentNormal);
    vec3 lightDir   = normalize(lightPosition - fragmentPosition);

    float diffStrength = max(dot(normal, lightDir), 0.0f);
    vec3 diffuseLight  = diffStrength * lightColour;

    // Attenuation
    float d = length(fragmentPosition - lightPosition);
    float attenuation = 1.0f / (K_c + K_l * d + K_q * d * d);
	
	vec4 objectTexture = texture(planetTexture, outUV);

    vec4 final_color = vec4((ambientLight + attenuation * diffuseLight) * objectColour, objectOpacity);
	
	color = final_color + objectTexture;
}
