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
#include "Physics.h"

#define MAX_CONTACTS 50

Physics physics;


static void nearCallback(void* data, dGeomID o1, dGeomID o2)

{

	// Temporary index for each contact

	int i;



	// Get the dynamics body for each geom

	dBodyID b1 = dGeomGetBody(o1);

	dBodyID b2 = dGeomGetBody(o2);



	// Create an array of dContact objects to hold the contact joints

	dContact contact[MAX_CONTACTS];



	// Now we set the joint properties of each contact. Going into the full details here would require a tutorial of its

	   // own. I'll just say that the members of the dContact structure control the joint behaviour, such as friction,

	   // velocity and bounciness. See section 7.3.7 of the ODE manual and have fun experimenting to learn more.  

	for (i = 0; i < MAX_CONTACTS; i++)

	{

		//contact[i].surface.mode = dContactBounce | dContactSlip1;

		contact[i].surface.mode = dContactMu2;

		contact[i].surface.mu = dInfinity;

		contact[i].surface.mu2 = 0;

		//contact[i].surface.bounce = 0.01;

		//contact[i].surface.bounce_vel = 0.1;

		//contact[i].surface.soft_cfm = 0.01;

	}



	// Here we do the actual collision test by calling dCollide. It returns the number of actual contact points or zero

	   // if there were none. As well as the geom IDs, max number of contacts we also pass the address of a dContactGeom

	   // as the fourth parameter. dContactGeom is a substructure of a dContact object so we simply pass the address of

	   // the first dContactGeom from our array of dContact objects and then pass the offset to the next dContactGeom

	   // as the fifth paramater, which is the size of a dContact structure. That made sense didn't it?  

	if (int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)))

	{

		// To add each contact point found to our joint group we call dJointCreateContact which is just one of the many

		   // different joint types available.  

		for (i = 0; i < numc; i++)

		{

			// dJointCreateContact needs to know which world and joint group to work with as well as the dContact

			   // object itself. It returns a new dJointID which we then use with dJointAttach to finally create the

			   // temporary contact joint between the two geom bodies.

			dJointID c = dJointCreateContact(physics.GetWorld(), physics.GetContacts(), contact + i);

			dJointAttach(c, b1, b2);

		}

	}

}

#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720

enum class GameState
{
	START,
	PLAYING,
	DEAD
};


int main()
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Hello World");

	Shader shader("./res/basicShader");
	Transform transform;



	double frameTime = 0;
	double elapsedTime = 0;

	//Initialize physics
	dWorldSetGravity(physics.GetWorld(), 0, -3, 0);

	Mesh meshList[] = {Mesh("./res/models/plane.obj", physics, glm::vec3(0,0,0))}; //Mesh("./res/models/flat_floor.obj", physics, glm::vec3(50,1,0)) 
	Scene testScene(meshList, 1);

	Player player(glm::vec3(0, 50, 0), 70.0f, (float)display.GetWidth() / (float)display.GetHeight(), physics);
	glm::vec3 dir = glm::vec3(0, 0, 0);

	const float lookSensitivity = 5.0f;


	while (!display.IsClosed())
	{
		//Start timing the frame
		auto frameTimeStart = std::chrono::high_resolution_clock::now();

		//Physics step
		elapsedTime += frameTime;
		if (elapsedTime > .01)
		{
			dSpaceCollide(physics.GetSpace(), 0, &nearCallback);
			dWorldStep(physics.GetWorld(), .01);
			dJointGroupEmpty(physics.GetContacts());
			elapsedTime = 0;
		}


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
				player.m_forward_angle += e.motion.xrel * lookSensitivity * float(frameTime);
				player.m_up_angle -= e.motion.yrel * lookSensitivity * float(frameTime);
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
		player.Move(dir * (float(frameTime) * 50.0f));
		player.Update();


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

	//Shutdown ODE
	dCleanupODEAllDataForThread();
	dCloseODE();

	return 0;
}
