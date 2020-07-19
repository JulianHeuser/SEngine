#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include "Scene.h"

#include <ode/ode.h>


/*
The Player

The player has a position, angle, and a camera.
*/

class Player
{
public:
	Player(glm::vec3 spawnPos, float fov, float aspectRatio);

	void Move();
	void Move(glm::vec3 moveAmount);
	void Update();
	void UpdateRot();
	void DetectCollision(Scene &scene);

	float m_forward_angle = 0;
	float m_up_angle = 0;

	Camera m_cam;
private:

	//CommonRigidBodyBase* m_rigidBody;

	glm::vec3 m_pos;
	glm::vec3 m_vel;

};

