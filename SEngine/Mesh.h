#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include "obj_loader.h"
#include <ode/ode.h>
#include "Physics.h"

class Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal = glm::vec3(0,0,0))
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	inline glm::vec3* GetPos() { return &pos; };
	inline glm::vec2* GetTexCoord() { return &texCoord; };
	inline glm::vec3* GetNormal() { return &normal; };

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

#define MAX_COL_VERICIES 100
#define MAX_COL_INDICES 200

class Mesh
{
public:
	Mesh() {};
	Mesh(Vertex* verticies, unsigned int numVerticies, unsigned int* indicies, unsigned int numIndicies, Physics physics);
	Mesh(const std::string fileName, Physics physics, glm::vec3 pos = glm::vec3(0, 0, 0));
	virtual ~Mesh();

	void Draw();
	inline glm::vec3 GetPos() { return m_pos; };

private:

	void InitMesh(const IndexedModel& model);

	enum
	{
		POSITION_VB,
		NORMAL_VB,
		TEXCOORD_VB,
		INDEX_VB,

		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;

	glm::vec3 m_pos;


	dWorldID m_worldID;
	dBodyID m_bodyID;
	dGeomID m_geomID;



	dTriIndex indexes[MAX_COL_INDICES];
	dVector3 triVert[MAX_COL_VERICIES];
};

