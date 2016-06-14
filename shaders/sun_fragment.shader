#version 330 core

out vec4 colour;

in vec2 outUV;

uniform sampler2D sunTexture;

void main() {
	colour = texture(sunTexture, outUV);
}
