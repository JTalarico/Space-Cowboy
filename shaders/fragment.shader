#version 330 core

out vec4 colour;

uniform vec3 setColour;
uniform float setOpacity;

void main() {
    colour = vec4(setColour, setOpacity);
}
