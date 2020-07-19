#include "Player.h"
#include <glm/glm.hpp>



Player::Player(glm::vec3 spawnPos, float fov, float aspectRatio)
{
	m_pos = spawnPos;
	m_vel = glm::vec3(0, 0, 0);
	m_cam = Camera(spawnPos, fov, aspectRatio, 0.1f, 2000.0f);

	//btBoxShape* groundShape = new btBoxShape(btVector3(1,1,1)); //m_rigidBody->createBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
}

void Player::Move()
{
	m_pos += m_vel;
	m_cam.ChangePos(m_pos);
}

void Player::Move(glm::vec3 moveAmount)
{
	m_pos.z += moveAmount.z * sinf(m_forward_angle) + moveAmount.x * cosf(m_forward_angle);
	m_pos.x += moveAmount.z * cosf(m_forward_angle) - moveAmount.x * sinf(m_forward_angle);
	m_cam.ChangePos(m_pos);
}

void Player::Update()
{
	UpdateRot();
}

void Player::DetectCollision(Scene& scene)
{
	const glm::vec3 sphereOrigin = m_pos;
	const float sphereRadius = 3.0f;
	const float sphereRadiusSquared = sphereRadius*sphereRadius;

	std::unique_ptr<Mesh[]>& meshes = scene.GetMeshes();

	for (unsigned int i = 0; i < scene.GetMeshNum(); i++)
	{
		//meshes[i].model.
	}
}

void Player::UpdateRot()
{
	m_cam.ChangeRot(glm::vec3(cos(m_forward_angle), m_up_angle, sin(m_forward_angle)));
}