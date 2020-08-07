#version 150
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D edgeTexture;

mat3 sx = mat3( 
    1.0, 2.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -2.0, -1.0 
);
mat3 sy = mat3( 
    1.0, 0.0, -1.0, 
    2.0, 0.0, -2.0, 
    1.0, 0.0, -1.0 
);

void main()
{ 

	vec3 diffuse = texture(screenTexture, TexCoords.st).rgb;
    mat3 I;
    mat3 I2;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            //vec3 sample  = texelFetch(edgeTexture, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb; //length(sample); 
            I[i][j] = texelFetch(edgeTexture, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).r;
            I2[i][j] = texelFetch(edgeTexture, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).g;
		}
	}
	
	float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]); 
	float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);
	
	float gx2 = dot(sx[0], I2[0]) + dot(sx[1], I2[1]) + dot(sx[2], I2[2]); 
	float gy2 = dot(sy[0], I2[0]) + dot(sy[1], I2[1]) + dot(sy[2], I2[2]);
	
	float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));
	float g2 = sqrt(pow(gx2, 2.0)+pow(gy2, 2.0));
	g += g2;
	vec4 color = vec4(diffuse - vec3(g), 1.0); 

	//FragColor = vec4(TexCoords,0,1);
    FragColor = color; //texture(screenTexture, TexCoords);
}