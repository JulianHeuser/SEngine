#pragma once
#include "Camera.h"
#include "Scene.h"
#include "Physics.h"
#include <ode/ode.h>
#include <glm/glm.hpp>

/*
The Player

The player has a position, angle, and a camera.
*/

class Player
{
public:
	Player(glm::vec3 spawnPos, float fov, float aspectRatio, Physics physics);

	void Move(glm::vec3 moveAmount);
	void Update();

	void Jump();
	void StoreVel(float amount);
	void ReleaseVel();

	float m_forward_angle = 0;
	float m_up_angle = 0;

	Camera m_cam;
private:
	void UpdateRot();
	void UpdatePos();
	void CheckGround();
	void AutoStep();

	//CommonRigidBodyBase* m_rigidBody;
	dWorldID m_worldID;
	dSpaceID m_spaceID;
	dBodyID m_bodyID;
	dGeomID m_geomID;
	dMass m_mass;

	glm::vec3 m_pos;


	bool grounded = false;
	bool canJump = false;

	float floorDist;

	//variables for mechanics
	float vel;
	float storedVel;
};

