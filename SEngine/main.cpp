#include <iostream>
#include "display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Player.h"
#include <GL/glew.h>
#include <chrono>
#include <random>


enum class GameState
{
	START,
	PLAYING,
	DEAD
};

#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 600


int main()
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Hello World");

	Shader shader("./res/basicShader");
	Texture texture("./res/bricks.jpg");
	Transform transform;

	float frameTime = 0;

	Mesh mesh("./res/building.obj", glm::vec3(0,0,200));

	Player player(glm::vec3(0, 0, 0), 70.0f, (float)display.GetWidth() / (float)display.GetHeight());
	glm::vec3 dir = glm::vec3(0, 0, 0);

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


				case SDLK_RIGHT:
					player.m_forward_angle += frameTime * 50;
					break;
				case SDLK_LEFT:
					player.m_forward_angle -= frameTime * 50;
					break;
				}
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
		player.UpdateRot();


		//Rendering
		display.Clear(0.0f, 0.5f, 0.5f, 1.0f);

		shader.Bind();
		texture.Bind(0);
		shader.Update(transform, player.m_cam);
		mesh.Draw();

		display.Update();
		auto frameTimeEnd = std::chrono::high_resolution_clock::now();
		frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(frameTimeEnd - frameTimeStart).count() / 1000000000.0;
	}
	return 0;
}