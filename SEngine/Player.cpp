#include "Player.h"
#include <glm/glm.hpp>
#include "Physics.h"
#include "Raycast.h"
#include <iostream>

#define MOVE_SPEED 50
#define JUMP_FORCE 600
#define CAM_OFFSET glm::vec3(0,1,0)

Player::Player(glm::vec3 spawnPos, float fov, float aspectRatio, Physics physics)
{
	m_pos = spawnPos;
	m_vel = glm::vec3(0, 0, 0);
	m_cam = Camera(spawnPos, fov, aspectRatio, 0.1f, 2000.0f);

	//Physics
	m_worldID = physics.GetWorld();
	m_spaceID = physics.GetSpace();
	m_bodyID = dBodyCreate(m_worldID);
	m_geomID = dCreateCapsule(physics.GetSpace(), .5, 2);

	dGeomSetBody(m_geomID, m_bodyID);

	dMassSetCapsule(&m_mass, .5, 2, .5, 2);
	dBodySetMass(m_bodyID, &m_mass);

	dBodySetMaxAngularSpeed(m_bodyID, 0);
	//dBodySetLinearDamping(m_bodyID, .5f);
	//dBodySetLinearDampingThreshold(m_bodyID, 10);
	dBodySetPosition(m_bodyID, spawnPos.x, spawnPos.y, spawnPos.z);

	dMatrix3 rotOffsetMatrix;
	dRFromEulerAngles(rotOffsetMatrix, 90, 0, 0);
	dGeomSetOffsetWorldRotation(m_geomID, rotOffsetMatrix);

}

void Player::Move(glm::vec3 moveAmount)
{

	float xComp = (moveAmount.z * cosf(m_forward_angle) - moveAmount.x * sinf(m_forward_angle)) * MOVE_SPEED;
	float zComp = (moveAmount.z * sinf(m_forward_angle) + moveAmount.x * cosf(m_forward_angle)) * MOVE_SPEED;

	//dBodySetLinearVel(m_bodyID, xComp, dBodyGetLinearVel(m_bodyID)[1], zComp);
	dBodyAddForce(m_bodyID, xComp, 0, zComp);

	UpdatePos();
}

void Player::Update()
{
	UpdateRot();
	CheckGround();
}

void Player::UpdatePos()
{
	m_pos = glm::vec3(dBodyGetPosition(m_bodyID)[0], dBodyGetPosition(m_bodyID)[1], dBodyGetPosition(m_bodyID)[2]);
	m_cam.ChangePos(m_pos + CAM_OFFSET);
}

void Player::UpdateRot()
{
	m_cam.ChangeRot(glm::vec3(cos(m_forward_angle), m_up_angle, sin(m_forward_angle)));
}

void Player::CheckGround()
{
	dVector3 startPos = { dBodyGetPosition(m_bodyID)[0], dBodyGetPosition(m_bodyID)[1] - 1.2f,dBodyGetPosition(m_bodyID)[2] };
	dVector3 endPos = { dBodyGetPosition(m_bodyID)[0], dBodyGetPosition(m_bodyID)[1] - 1.5f,dBodyGetPosition(m_bodyID)[2] };

	bool oldGrounded = grounded;

	grounded = RaycastQuery(m_spaceID, startPos, endPos);

	if (!grounded)
		canJump = false;
	else if (!oldGrounded && grounded)
	{
		canJump = true;
	}

}

//Player actions
void Player::Jump()
{
	if (canJump)
	{
		std::cout << canJump << std::endl;
		dBodySetLinearVel(m_bodyID, dBodyGetLinearVel(m_bodyID)[0], 0, dBodyGetLinearVel(m_bodyID)[2]);
		dBodyAddForce(m_bodyID, 0, JUMP_FORCE, 0);
		canJump = false;
	}
}

