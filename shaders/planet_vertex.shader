#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragmentPosition;
out vec3 fragmentNormal;

uniform mat4 model;
uniform mat4 MVP;
uniform mat4 normalMatrix;

void main() {
    fragmentPosition = vec3(model * vec4(position, 1.0f));
    fragmentNormal   = vec3(normalMatrix * vec4(normal, 0));
	gl_Position      = MVP * vec4(position, 1.0f);
}
