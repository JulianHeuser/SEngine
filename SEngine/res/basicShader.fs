#version 150

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
	//Gradient texturing
	vec4 gradientTex = vec4(gs_in.texCoord.x * color1 + (1 - gs_in.texCoord.x) * color2,1);
	
	//Lighting
	float lighting = (dot(gs_in.lightDir, gs_in.normal)/4 + .75f);
	
	
	//outlines
	//vec2((gs_in.texCoord.x-.5)*(gs_in.texCoord.x-.5)*-4+1, (gs_in.texCoord.y-.5)*(gs_in.texCoord=.y-.5)*-4+1);
	//vec4 outline1 = (abs(gs_in.texCoord.x - .5) <= (gs_in.lengths.x/gs_in.lengths.y)) ? vec4(1,1,1,1) : vec4(0,0,0,1);
	//vec4 outline2 = (abs(gs_in.texCoord.y - .5) <= (gs_in.lengths.x/gs_in.lengths.y)) ? vec4(1,1,1,1) : vec4(0,0,0,1);
	//outline1 = outline1 * outline2;
	
	//float nearD = min(min(gs_in.dist[0],gs_in.dist[1]),gs_in.dist[2]);
	//float edgeIntensity = exp2(-1.0*nearD*nearD);
	
	
	vec4 finalColor = gradientTex * lighting;;
	
	diffuseColor = finalColor;
}