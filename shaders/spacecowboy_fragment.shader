#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;

//in vec2 outUV;

out vec4 color;

uniform vec3  objectColour;
uniform float objectOpacity;

vec3 lightColour   = vec3(1.0f);
vec3 lightPosition = vec3(0);

float ambientStrength = 0.1f;

//uniform sampler2D spacecowboyTexture;

void main() {
    // Ambient lighting.
    vec3 ambientLight = ambientStrength * lightColour;

    // Diffuse lighting.
    vec3 normal     = normalize(fragmentNormal);
    vec3 lightDir   = normalize(lightPosition - fragmentPosition);

    float diffStrength = max(dot(normal, lightDir), 0.0f);
    vec3 diffuseLight  = diffStrength * lightColour;
	
	//vec4 objectTexture = texture(spacecowboyTexture, outUV);

    vec4 final_color = vec4((ambientLight + diffuseLight) * objectColour, objectOpacity); 
	
	color = final_color; //+ objectTexture
}
