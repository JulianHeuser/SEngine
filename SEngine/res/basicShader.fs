#version 150

varying vec3 normal0;
varying vec3 position0;
varying vec2 texCoord0;
varying vec3 lightDir0;

uniform sampler2D diffuse;

out vec4 diffuseColor;


void main()
{
	vec3 color1 = vec3(0.9,0.8,0);
	vec3 color2 = vec3(0.9,0.6,0);

	diffuseColor = vec4(color1 * texCoord0.x + color2 * texCoord0.y,1) * clamp(dot(lightDir0, normal0), .3, 1.0); //vec4(.25,.25,.25,1) + vec4(1,0,0,1)
	
}