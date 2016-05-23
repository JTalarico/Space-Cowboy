#version 330 core

out vec4 colour;

uniform vec3  objectColour;
uniform float objectOpacity;

void main() {
    colour = vec4(objectColour, objectOpacity);
}
