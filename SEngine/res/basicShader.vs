#version 150

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec3 position0;
varying vec3 normal0;
varying vec2 texCoord0;
varying vec3 lightDir0;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position, 1.0);
	lightDir0 = vec3(.25, 0, .5);//(transform * vec4(0, 0, 1, 0)).xyz;
	
	normal0 = normal;//(transform * vec4(normal, 0)).xyz;
	position0 = position;
	texCoord0 = texCoord;
	
	//color01 = color1;
	//color02 = color2;
}