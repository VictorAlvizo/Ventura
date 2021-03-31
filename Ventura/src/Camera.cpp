#include "Camera.h"

Camera::Camera() 
	:m_Pos(0.0f)
{
	//holder
}

Camera::Camera(glm::vec2 pos, glm::vec2 windowSize, Entity * entity)
	:m_Pos(pos), m_WindowSize(windowSize), m_FollowEntity(entity)
{
	if (m_FollowEntity) { //Entity to follow there, attach itself to entity
		m_FollowEntity->AttachCamera(this);
	}
}

Camera::~Camera() {
	m_FollowEntity = nullptr;
}

void Camera::Move(glm::vec2 pos) {
	m_Pos = pos;
}

void Camera::Translate(glm::vec2 transPos, float deltaTime) {
	m_Pos += transPos * deltaTime;
}

void Camera::SetEntity(Entity * entity) {
	m_FollowEntity = entity;
	m_FollowEntity->AttachCamera(this);
}

void Camera::Disconnect() {
	if (m_FollowEntity) {
		m_FollowEntity->DetachCamera();
		m_FollowEntity = nullptr;
	}
}

glm::mat4 Camera::UpdateView() {
	return glm::lookAt(glm::vec3(m_Pos, 0.0f), glm::vec3(m_Pos, 0.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}