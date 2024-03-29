#include <iostream>
#include "display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Player.h"
#include "Scene.h"
#include "Physics.h"
#include <GL/glew.h>
#include <chrono>
#include <memory>

#define MAX_CONTACTS 50
#define PHYSICS_TIMESTEP .016f

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

		contact[i].surface.mode = dContactSlip1;
		
		contact[i].surface.mu = 5;


		contact[i].surface.slip1 = .1;

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
	Shader quadShader("./res/renderQuad");
	Shader edgeShader("./res/edgeBase");
	Transform transform;

	
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	//Main render texture
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int mainTexture;
	glGenTextures(1, &mainTexture);
	glBindTexture(GL_TEXTURE_2D, mainTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mainTexture, 0); //Attach yourTexture so drawing goes into it

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo); //Create a frame buffer
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, DISPLAY_WIDTH, DISPLAY_HEIGHT); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Edge render texture
	unsigned int framebuffer2;
	glGenFramebuffers(1, &framebuffer2);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);

	unsigned int edgeTexture;
	glGenTextures(1, &edgeTexture);
	glBindTexture(GL_TEXTURE_2D, edgeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, edgeTexture, 0); //Attach yourTexture so drawing goes into it

	unsigned int rbo2;
	glGenRenderbuffers(1, &rbo2); //Create a frame buffer
	glBindRenderbuffer(GL_RENDERBUFFER, rbo2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, DISPLAY_WIDTH, DISPLAY_HEIGHT); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo2); // now actually attach it


	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	double frameTime = 0;
	double elapsedTime = 0;

	Mesh meshList[] = {Mesh("./res/models/level1.obj", physics, glm::vec3(0,0,0)), Mesh("./res/models/building.obj", glm::vec3(-400,0,0)) }; //Mesh("./res/models/flat_floor.obj", physics, glm::vec3(50,1,0)) 
	Scene testScene(meshList, 2);

	Player player(glm::vec3(0, 4, 0), 70.0f, (float)display.GetWidth() / (float)display.GetHeight(), physics);
	glm::vec3 dir = glm::vec3(0, 0, 0);
	bool holdingJump = false;
	bool holdingMouse1 = false;

	const float lookSensitivity = .005f;


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
				case SDLK_SPACE:
					holdingJump = true;
					break;
				case SDLK_ESCAPE:
					display.IsClosed() = true;
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					holdingMouse1 = true;
					break;
				case SDL_BUTTON_RIGHT:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					player.ReleaseVel();
					holdingMouse1 = false;
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				player.m_forward_angle += e.motion.xrel * lookSensitivity;
				player.m_up_angle -= e.motion.yrel * lookSensitivity;
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
				case SDLK_SPACE:
					holdingJump = false;
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

		//Fixed timestep (For physics and stuff)
		elapsedTime += frameTime;
		if (elapsedTime > PHYSICS_TIMESTEP)
		{
			elapsedTime = 0;

			//Physics Step
			dSpaceCollide(physics.GetSpace(), 0, &nearCallback);
			dWorldQuickStep(physics.GetWorld(), PHYSICS_TIMESTEP);
			dJointGroupEmpty(physics.GetContacts());

			//Player movement
			player.Move(dir * (.5f));
			player.Update();
		}
		
		if (holdingJump)
			player.Jump();
		if (holdingMouse1)
			player.StoreVel(frameTime);

		//Rendering
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); //Bind it so we draw to it
		glEnable(GL_DEPTH_TEST);
		display.Clear(0.333f, 0.98823f, 0.95686f, 1.0f);
		
		shader.Bind();
		shader.Update(transform, player.m_cam);

		//Render scene to main texture
		testScene.Draw();


		//Render scene to edge texture
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
		display.Clear(0.333f, 0.98823f, 0.95686f, 1.0f);
		edgeShader.Bind();
		edgeShader.Update(transform, player.m_cam);
		testScene.Draw();
		

		//Clear stuff + disable depth test temporarily
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		display.Clear(0.333f, 0.98823f, 0.95686f, 1.0f);

		//Render quad with the texture to apply post-processing
		quadShader.Bind();
		glBindVertexArray(quadVAO);

		GLuint t1Location = glGetUniformLocation(quadShader.GetProgram(), "screenTexture");
		GLuint t2Location = glGetUniformLocation(quadShader.GetProgram(), "edgeTexture");
		glUniform1i(t1Location, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mainTexture);
		glUniform1i(t2Location, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, edgeTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		display.Update();
		auto frameTimeEnd = std::chrono::high_resolution_clock::now();
		frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(frameTimeEnd - frameTimeStart).count() / 1000000000.0;
	}

	//Shutdown ODE
	dCleanupODEAllDataForThread();
	dCloseODE();

	return 0;
}
