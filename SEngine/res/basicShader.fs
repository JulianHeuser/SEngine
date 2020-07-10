#version 150

varying vec2 texCoord0;

uniform sampler2D diffuse;

out vec4 diffuseColor;
	
void main()
{
	diffuseColor = texture2D(diffuse, texCoord0);  //vec4(1.0, 0.0, 0.0, 1.0);
	
}