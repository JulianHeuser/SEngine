#include "Player.h"
#include <iostream>

Player::Player(glm::vec3 spawnPos, float fov, float aspectRatio)
{
	m_pos = spawnPos;
	m_vel = glm::vec3(0, 0, 0);
	m_cam = Camera(spawnPos, fov, aspectRatio, 0.01f, 10000.0f);
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

void Player::UpdateRot()
{

	std::cout << m_forward_angle << std::endl;
	m_cam.ChangeRot(glm::vec3(cos(m_forward_angle), 0, sin(m_forward_angle)));
}