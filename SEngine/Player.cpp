#include "Player.h"
#include <glm/glm.hpp>
#include "Physics.h"


Player::Player(glm::vec3 spawnPos, float fov, float aspectRatio, Physics physics)
{
	m_pos = spawnPos;
	m_vel = glm::vec3(0, 0, 0);
	m_cam = Camera(spawnPos, fov, aspectRatio, 0.1f, 2000.0f);

	//Physics
	m_worldID = physics.GetWorld();
	m_bodyID = dBodyCreate(m_worldID);
	m_geomID = dCreateBox(physics.GetSpace(), 1, 1, 1);

	dGeomSetBody(m_geomID, m_bodyID);

	dBodySetPosition(m_bodyID, spawnPos.x, spawnPos.y, spawnPos.z);
	//btBoxShape* groundShape = new btBoxShape(btVector3(1,1,1)); //m_rigidBody->createBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
}


void Player::Move(glm::vec3 moveAmount)
{

	float xComp = (moveAmount.z * cosf(m_forward_angle) - moveAmount.x * sinf(m_forward_angle)) * 10;
	float zComp = (moveAmount.z * sinf(m_forward_angle) + moveAmount.x * cosf(m_forward_angle)) * 10;

	dBodyAddForce(m_bodyID, xComp, 0, zComp);
	//m_pos.z += moveAmount.z * sinf(m_forward_angle) + moveAmount.x * cosf(m_forward_angle);
	//m_pos.x += moveAmount.z * cosf(m_forward_angle) - moveAmount.x * sinf(m_forward_angle);
	UpdatePos();
	m_cam.ChangePos(m_pos);
}

void Player::Update()
{
	UpdateRot();
}

void Player::UpdatePos()
{
	m_pos = glm::vec3(dBodyGetPosition(m_bodyID)[0], dBodyGetPosition(m_bodyID)[1], dBodyGetPosition(m_bodyID)[2]);
}

void Player::UpdateRot()
{
	m_cam.ChangeRot(glm::vec3(cos(m_forward_angle), m_up_angle, sin(m_forward_angle)));
}