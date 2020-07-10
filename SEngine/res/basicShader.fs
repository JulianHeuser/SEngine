#version 150

varying vec2 texCoord0;
varying vec3 normal0;

uniform sampler2D diffuse;

out vec4 diffuseColor;


void main()
{
	diffuseColor = texture2D(diffuse, texCoord0) * clamp(dot(-vec3(0,0,1), normal0), 0.0, 1.0);  //vec4(1.0, 0.0, 0.0, 1.0);
	
}