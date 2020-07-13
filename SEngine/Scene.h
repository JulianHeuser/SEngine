#pragma once
#include "Mesh.h"
#include <memory>

/*
Scenes

We store meshes in m_meshes and when Draw is called, we draw each mesh to the scene.
This makes it easy to combine meshes into a level and draw it all at once. And it allows
for flexibility and functionality such as reading scenes from a file.
*/

class Scene
{
public:
	Scene(Mesh sceneMeshes[], unsigned int size);
	void Draw();

private:
	std::unique_ptr<Mesh[]> m_meshes;
	unsigned int m_meshNum;
};
