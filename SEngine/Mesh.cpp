#include "Mesh.h"
#include <vector>

#include <iostream>

Mesh::Mesh(const std::string fileName, Physics physics, glm::vec3 pos)
{
	IndexedModel model = OBJModel(fileName).ToIndexedModel();

	m_pos = pos;

	for (unsigned int i = 0; i < model.positions.size(); i++)
	{
		model.positions[i].x += pos.x;
		model.positions[i].y += pos.y;
		model.positions[i].z += pos.z;
	}

	InitMesh(model);

	//Physics
	m_worldID = physics.GetWorld();

	dTriMeshDataID triMeshID = dGeomTriMeshDataCreate();

	triVert = new dVector3[model.positions.size()];
	indexes = new dTriIndex[model.indices.size()];

	for (unsigned int i = 0; i < model.positions.size(); i++)
	{
		triVert[i][0] = model.positions[i].x;
		triVert[i][1] = model.positions[i].y;
		triVert[i][2] = model.positions[i].z;
	}
	for (unsigned int i = 0; i < model.indices.size(); i++)
	{
		indexes[i] = model.indices[i];
	}

	dGeomTriMeshDataBuildSimple(triMeshID, *triVert, model.positions.size(), indexes, model.indices.size());
	m_geomID = dCreateTriMesh(physics.GetSpace(), triMeshID, NULL, NULL, NULL);

}


Mesh::Mesh(const std::string fileName, glm::vec3 pos)
{
	IndexedModel model = OBJModel(fileName).ToIndexedModel();

	m_pos = pos;

	for (unsigned int i = 0; i < model.positions.size(); i++)
	{
		model.positions[i].x += pos.x;
		model.positions[i].y += pos.y;
		model.positions[i].z += pos.z;
	}

	InitMesh(model);

}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::InitMesh(const IndexedModel& model)
{
	m_drawCount = model.indices.size();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT,0);

	glBindVertexArray(0);
}
