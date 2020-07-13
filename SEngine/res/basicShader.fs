#version 150

//varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
varying vec3 lightDir0;

uniform sampler2D diffuse;

out vec4 diffuseColor;


void main()
{
	diffuseColor = vec4((position0.x + position0.y)/50, position0.y/100, 0, 1) * clamp(dot(lightDir0, normal0), .3, 1.0); //vec4(.25,.25,.25,1) + vec4(1,0,0,1)
	
}