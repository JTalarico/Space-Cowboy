#version 330 core

out vec4 colour;

in vec2 outUV;

uniform vec3  objectColour;
uniform float objectOpacity;

uniform sampler2D sunTexture;

void main() {

	vec4 useless = vec4(objectColour, objectOpacity);
    
	colour = texture(sunTexture, outUV);
}
