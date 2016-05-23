#version 330 core

out vec4 colour;

uniform vec3  objectColour;
uniform float objectOpacity;

vec3 lightColour   = vec3(1.0f);
vec3 lightPosition = vec3(0);

float ambientStrength = 0.1f;

void main() {
    vec3 ambientLight = ambientStrength * lightColour;
    colour = vec4(ambientLight * objectColour, objectOpacity);
}
