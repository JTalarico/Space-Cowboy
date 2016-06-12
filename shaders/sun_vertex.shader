#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvs;

uniform mat4 MVP;
out vec2 outUV;

void main() {
	gl_Position = MVP * vec4(position, 1.0);
	outUV = uvs;
}
