#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;

out vec4 colour;

uniform vec3  objectColour;
uniform float objectOpacity;

vec3 lightColour   = vec3(1.0f);
vec3 lightPosition = vec3(0);

float ambientStrength = 0.1f;

void main() {
    // Ambient lighting.
    vec3 ambientLight = ambientStrength * lightColour;

    // Diffuse lighting.
    vec3 normal     = normalize(fragmentNormal);
    vec3 lightDir   = normalize(lightPosition - fragmentPosition);

    float diffStrength = max(dot(normal, lightDir), 0.0f);
    vec3 diffuseLight  = diffStrength * lightColour;

    colour = vec4((ambientLight + diffuseLight) * objectColour, objectOpacity);
}
