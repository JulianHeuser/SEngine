#include "Player.h"
#include <glm/glm.hpp>
#include "Physics.h"
#include "Raycast.h"
#include <iostream>

#define CAM_OFFSET glm::vec3(0,1,0)

#define MAX_SPEED 100//75
#define MOVE_SPEED 50
#define JUMP_FORCE 5000

#define MAX_STORED_VEL 5
#define STORED_VEL_MULTIPLIER .95 //Storing all velocity can lead to gaining infinite speed

Player::Player(glm::vec3 spawnPos, float fov, float aspectRatio, Physics physics)
{
	storedVel = 0;

	m_pos = spawnPos;
	vel = 0;
	m_cam = Camera(spawnPos, fov, aspectRatio, 0.1f, 2000.0f);

	//Physics
	m_worldID = physics.GetWorld();
	m_spaceID = physics.GetSpace();
	m_bodyID = dBodyCreate(m_worldID);
	m_geomID = dCreateCapsule(physics.GetSpace(), .5, 2);

	dGeomSetBody(m_geomID, m_bodyID);

	dMassSetCapsule(&m_mass, 1, 2, .5, 2);
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


	float xComp = (moveAmount.z * cosf(m_forward_angle) - moveAmount.x * sinf(m_forward_angle));// * MOVE_SPEED;
	float zComp = (moveAmount.z * sinf(m_forward_angle) + moveAmount.x * cosf(m_forward_angle));// * MOVE_SPEED;


	float totalVel = fabs(dBodyGetLinearVel(m_bodyID)[0]) + fabs(dBodyGetLinearVel(m_bodyID)[2]);
	if (totalVel == 0)
		totalVel = 1;
	dVector3 normalizedVel = { dBodyGetLinearVel(m_bodyID)[0] / totalVel, 0, dBodyGetLinearVel(m_bodyID)[2] / totalVel }; //Acts as the forward vector of velocity
	dVector3 moveVector = { xComp, 0 , zComp };
	dReal dot = dCalcVectorDot3(normalizedVel, moveVector);
	dReal inverseDot = ((dot < 0 ? 1 : 1 - dot) * 5 + 1 );

	//Subtract a force in the direction of our current velocity
	//dBodyAddForce(m_bodyID, -normalizedVel[0] * inverseDot * (totalVel/2), 0, -normalizedVel[2] * inverseDot * (totalVel/2));

	//Add a force in the direction we're looking
	dBodyAddForce(m_bodyID, xComp * MOVE_SPEED * (inverseDot + 1), 0, zComp * MOVE_SPEED * (inverseDot + 1));

	if (totalVel > MAX_SPEED)
	{
		dVector3 maxVel = { normalizedVel[0] * float(MAX_SPEED), 0 , normalizedVel[2] * float(MAX_SPEED) };
		dBodySetLinearVel(m_bodyID, maxVel[0], dBodyGetLinearVel(m_bodyID)[1], maxVel[2]);
	}

	UpdatePos();
}

void Player::Update()
{
	UpdateRot();
	dGeomDisable(m_geomID);
	CheckGround();
	AutoStep();
	dGeomEnable(m_geomID);
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
	dVector3 boxPos = { dBodyGetPosition(m_bodyID)[0], dBodyGetPosition(m_bodyID)[1] - 1.2f,dBodyGetPosition(m_bodyID)[2] };

	bool oldGrounded = grounded;
	grounded = (BoxCheck(m_spaceID, boxPos, .8f));

	if (!grounded)
	{
		canJump = false;
	}
	else if (!oldGrounded && grounded)
	{
		canJump = true;
	}


	if (grounded)
	{
		dBodyAddForce(m_bodyID, 0, -25, 0);
	}
}

#define SNAP_THRESHOLD .5f
#define SNAP_THRESHOLD_2 3.0f
//Automatically "snap" the player to the ground if there's a small change in elevation
void Player::AutoStep()
{
	dVector3 startPos = { dBodyGetPosition(m_bodyID)[0], dBodyGetPosition(m_bodyID)[1],dBodyGetPosition(m_bodyID)[2] };
	dVector3 endPos = { dBodyGetPosition(m_bodyID)[0], dBodyGetPosition(m_bodyID)[1] - 50.0f,dBodyGetPosition(m_bodyID)[2] };
	dVector3 dif;

	float lastFloorDist = floorDist;
	RaycastQuery(m_spaceID, startPos, endPos);
	floorDist = endPos[1];

	float floorDistDiff = lastFloorDist - floorDist;

	if (floorDistDiff < SNAP_THRESHOLD && floorDistDiff > .05f && fabs(startPos[1] - endPos[1]) < SNAP_THRESHOLD_2)
	{
		//dBodySetPosition(m_bodyID, dBodyGetPosition(m_bodyID)[0], dBodyGetPosition(m_bodyID)[1] + floorDistDiff, dBodyGetPosition(m_bodyID)[2]);

		dBodyAddForce(m_bodyID, 0, -50, 0);

		std::cout << floorDistDiff << std::endl;
	}

}

//Player actions
void Player::Jump()
{
	if (canJump)
	{
		dBodySetLinearVel(m_bodyID, dBodyGetLinearVel(m_bodyID)[0], 0, dBodyGetLinearVel(m_bodyID)[2]);
		dBodyAddForce(m_bodyID, 0, JUMP_FORCE, 0);
		canJump = false;
	}
}

void Player::StoreVel(float amount)
{
	if (storedVel == 0)
	{
		storedVel = (fabs(dBodyGetLinearVel(m_bodyID)[0]) + fabs(dBodyGetLinearVel(m_bodyID)[2]));
	}
	dBodyAddForce(m_bodyID, dBodyGetLinearVel(m_bodyID)[0] * amount * -2000, dBodyGetLinearVel(m_bodyID)[1] * amount * -2000, dBodyGetLinearVel(m_bodyID)[2] * amount * -2000);
}

void Player::ReleaseVel()
{
	//if (storedVel > MAX_STORED_VEL)
	//	storedVel = MAX_STORED_VEL;
	storedVel *= STORED_VEL_MULTIPLIER;

	float xComp = storedVel * (cosf(m_forward_angle));
	float yComp = storedVel * (sinf(m_up_angle));
	float zComp = storedVel * (sinf(m_forward_angle));

	dBodySetLinearVel(m_bodyID, xComp, yComp, zComp);
	//dBodyAddForce(m_bodyID, xComp, yComp, zComp);

	//Move(force * storedVel);

	storedVel = 0;
}