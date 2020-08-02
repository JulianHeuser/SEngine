#version 150

uniform sampler2D screenTexture;

out vec4 diffuseColor;

uniform vec3 color1;
uniform vec3 color2;


in VertexData {
	vec3 position;
    vec3 normal;
	vec2 texCoord;
	vec3 lightDir;
} gs_in;



void main()
{
	diffuseColor = vec4((dot(vec3(.5,.5,0), gs_in.normal) + 1)/2,0,0,1);//vec4((gs_in.normal.x+1)/2,(gs_in.normal.y+1),(gs_in.normal.z+1)/2,1);
}