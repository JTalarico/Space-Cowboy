#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec3 vColor;

out vec4 color;

vec3 lightColour   = vec3(1.0f);
vec3 lightPosition = vec3(0);
float K_c = 0.4f;
float K_l = 0.0001f;
float K_q = 0.0f;

float ambientStrength = 0.1f;

void main() {
    // Ambient lighting.
    vec3 ambientLight = ambientStrength * lightColour;

    // Attenuation.
    float d = length(fragmentPosition - lightPosition);
    float attenuation = 1.0f / (K_c + K_l * d + K_q * d * d);

    // Diffuse lighting.
    vec3 normal     = normalize(fragmentNormal);
    vec3 lightDir   = normalize(lightPosition - fragmentPosition);

    float diffStrength = max(dot(normal, lightDir), 0.0f);
    vec3 diffuseLight  = attenuation * diffStrength * lightColour;

    color = vec4((ambientLight + diffuseLight) * vColor, 1.0f);

}
