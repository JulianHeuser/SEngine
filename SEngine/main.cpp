#include <iostream>
#include "display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include <GL/glew.h>

int main()
{
	Display display(800, 600, "Hello World");


	
	Vertex verticies[] = {
		Vertex(glm::vec3(-0.5, -0.5 ,0), glm::vec2(0,0)),
		Vertex(glm::vec3(0,0.5,0), glm::vec2(.5f,1.0f)),
		Vertex(glm::vec3(0.5,-0.5,0), glm::vec2(1.0f,0))
	};


	Vertex verticies2[] = {
		Vertex(glm::vec3(-.5, -.5 ,0), glm::vec2(0,0)),
		Vertex(glm::vec3(.5f,-.5f,0), glm::vec2(1.0f,0)),
		Vertex(glm::vec3(0,-1.0,0), glm::vec2(.5f,1.0f))
	};

	Mesh mesh(verticies, sizeof(verticies) / sizeof(verticies[0]));
	Mesh mesh2(verticies2, sizeof(verticies2) / sizeof(verticies2[0]));

	Shader shader("./res/basicShader");
	Texture texture("./res/bricks.jpg");
	Camera camera(glm::vec3(0, 0, -3), 70.0f, (float)display.GetWidth() / (float)display.GetHeight(), 0.01f, 1000.0f);
	Transform transform;

	float counter = 0.0f;

	while (!display.IsClosed())
	{
		display.Clear(0.0f, 0.5f, 0.5f, 1.0f);

		transform.GetRot().x = counter*2;
		transform.GetRot().z = counter;

		shader.Bind();
		texture.Bind(0);
		shader.Update(transform, camera);
		mesh.Draw();
		mesh2.Draw();


		display.Update();
		counter += 0.001f;
	}
	return 0;
}