#include "Scene.h"

#include <iostream>
Scene::Scene(Mesh sceneMeshes[], unsigned int size)
{
	m_meshNum = size;

	std::cout << m_meshNum << std::endl;

	m_meshes = std::make_unique<Mesh[]>(m_meshNum);

	for (unsigned int i = 0; i< m_meshNum; i++)
	{
		m_meshes[i] = sceneMeshes[i];
	}
}


void Scene::Draw()
{
	for (unsigned int i = 0; i < m_meshNum; i++)
	{
		m_meshes[i].Draw();
	}
}