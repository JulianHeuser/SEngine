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
	vec2 WIN_SCALE = vec2(1280,720);
	
	// taken from 'Single-Pass Wireframe Rendering'
	//vec2 p0 = .5 * WIN_SCALE * gl_PositionIn[0].xy/gl_PositionIn[0].w;
	//vec2 p1 = .5 * WIN_SCALE * gl_PositionIn[1].xy/gl_PositionIn[1].w;
	//vec2 p2 = .5 * WIN_SCALE * gl_PositionIn[2].xy/gl_PositionIn[2].w;
	//vec2 v0 = p2-p1;
	//vec2 v1 = p2-p0;
	//vec2 v2 = p1-p0;
	//float area = abs(v1.x*v2.y - v1.y * v2.x);
	
	float a = length(gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz);
    float b = length(gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);
    float c = length(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz);

    float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
    float beta = acos( (a*a + c*c - b*b) / (2.0*a*c) );
    float ha = abs( c * sin( beta ) );
    float hb = abs( c * sin( alpha ) );
    float hc = abs( b * sin( alpha ) );
	
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