#version 330 core

in vec2 outUV;
out vec4 color;

//uniform sampler2D hubTexture;

void main() {
	vec4 redColor = vec4(1.0f, 0.0f, 0.0f,0.0f);
	//color = texture(hubTexture, outUV) + redColor;
	color = redColor;

}