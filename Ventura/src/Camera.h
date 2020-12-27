#pragma once
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Entity.h"

class Camera {
public:
	Camera();
	Camera(glm::vec2 pos, glm::vec2 windowSize,Entity * entity = nullptr);
	~Camera();

	void Move(glm::vec2 pos);
	void Translate(glm::vec2 transPos, float deltaTime);
	void SetEntity(Entity * entity);
	//If there is an entity attached to this camera, stop following it
	void Disconnect();

	glm::mat4 UpdateView();
	
	inline glm::vec2 getPos() const { return m_Pos; }
	inline glm::vec2 getWindowSize() const { return m_WindowSize; }

private:
	glm::vec2 m_Pos, m_WindowSize;

	Entity * m_FollowEntity;
};

