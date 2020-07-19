#include <iostream>
#include "display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Player.h"
#include "Scene.h"
#include <GL/glew.h>
#include <chrono>
#include <random>
#include <memory>

enum class GameState
{
	START,
	PLAYING,
	DEAD
};

#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720

int main()
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Hello World");

	Shader shader("./res/basicShader");
	Transform transform;

	float frameTime = 0;

	Mesh meshList[] = { Mesh("./res/models/building.obj", glm::vec3(-500,0,0)), Mesh("./res/models/flat_floor.obj", glm::vec3(0,-20,0)),Mesh("./res/models/flat_floor.obj", glm::vec3(50,-40,0)) };
	Scene testScene(meshList, 3);

	Player player(glm::vec3(0, 0, -50), 70.0f, (float)display.GetWidth() / (float)display.GetHeight());
	glm::vec3 dir = glm::vec3(0, 0, 0);

	const float lookSensitivity = 5.0f;

	while (!display.IsClosed())
	{
		//Start timing the frame
		auto frameTimeStart = std::chrono::high_resolution_clock::now();

		//Input
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{

			switch (e.type)
			{
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_w:
					dir.z = 1;
					break;
				case SDLK_s:
					dir.z = -1;
					break;
				case SDLK_a:
					dir.x = -1;
					break;
				case SDLK_d:
					dir.x = 1;
					break;

				case SDLK_ESCAPE:
					display.IsClosed() = true;
					break;
				}
				break;

			case SDL_MOUSEMOTION:
				player.m_forward_angle += e.motion.xrel * lookSensitivity * frameTime;
				player.m_up_angle -= e.motion.yrel * lookSensitivity * frameTime;
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
				case SDLK_w:
				case SDLK_s:
					dir.z = 0;
					break;
				case SDLK_a:
				case SDLK_d:
					dir.x = 0;
					break;
				}
				break;

			case SDL_QUIT:
				display.IsClosed() = true;
				break;
			default:
				break;

			}
		}
		
		//Player code
		player.Move(dir * (frameTime * 50));
		player.Update();
		player.DetectCollision(testScene);


		//Rendering
		display.Clear(0.0f, 0.5f, 0.5f, 1.0f);

		shader.Bind();
		shader.Update(transform, player.m_cam);
		//mesh.Draw();

		//Render scene
		testScene.Draw();



		display.Update();
		auto frameTimeEnd = std::chrono::high_resolution_clock::now();
		frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(frameTimeEnd - frameTimeStart).count() / 1000000000.0;
	}
	return 0;
}