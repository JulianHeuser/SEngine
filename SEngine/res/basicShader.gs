#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VertexData {
	vec3 position;
    vec3 normal;
	vec2 texCoord;
	vec3 lightDir;
} gs_in[3];

out VertexData {
	vec3 position;
    vec3 normal;
	vec2 texCoord;
	vec3 lightDir;
} gs_out;



void main()
{
	gl_Position = gl_in[0].gl_Position;
	gs_out.position = gs_in[0].position;
	gs_out.normal = gs_in[0].normal;
	gs_out.texCoord = gs_in[0].texCoord;
	gs_out.lightDir = gs_in[0].lightDir;
	EmitVertex();
	
	gl_Position = gl_in[1].gl_Position;
	gs_out.position = gs_in[1].position;
	gs_out.normal = gs_in[1].normal;
	gs_out.texCoord = gs_in[1].texCoord;
	gs_out.lightDir = gs_in[1].lightDir;
	EmitVertex();
	
	gl_Position = gl_in[2].gl_Position;
	gs_out.position = gs_in[2].position;
	gs_out.normal = gs_in[2].normal;
	gs_out.texCoord = gs_in[2].texCoord;
	gs_out.lightDir = gs_in[2].lightDir;
	EmitVertex();
	
	EndPrimitive();
	
}