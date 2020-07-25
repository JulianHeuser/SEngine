#pragma once

#include "Transform.h"
#include "Camera.h"
#include <string>
#include <GL/glew.h>
class Shader
{
public:
	Shader(const std::string& fileName);
	virtual ~Shader();

	void Bind();
	void Update(const Transform& transform, const Camera& camera);

private:
	static const unsigned int NUM_SHADERS = 2;

	enum
	{
		TRANSFORM_U,
		COLOR1_U,
		COLOR2_U,

		NUM_UNIFORMS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];


	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);
};

