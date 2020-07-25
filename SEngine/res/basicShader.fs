#version 150

varying vec3 normal0;
varying vec3 position0;
varying vec2 texCoord0;
varying vec3 lightDir0;

uniform sampler2D diffuse;

out vec4 diffuseColor;

uniform vec3 color1;
uniform vec3 color2;

void main()
{
	//vec3 color1 = vec3(1,0,0);
	//vec3 color2 = vec3(0,0,1);

	diffuseColor = vec4(texCoord0.x * color1 + (1 - texCoord0.x) * color2,1) * (dot(lightDir0, normal0)/4 + .75f); //vec4(color1 * texCoord0.x + color2 * texCoord0.y,1)
	
}