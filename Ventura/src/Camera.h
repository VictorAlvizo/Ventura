#pragma once
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Entity.h"

class Camera {
public:
	//TODO: Add a follow entity mode similar to the hitbox
	Camera();
	Camera(glm::vec2 pos, Entity * entity = nullptr);
	~Camera();

	void Move(glm::vec2 pos);
	void Translate(glm::vec2 transPos, float deltaTime);
	void SetEntity(Entity * entity);

	glm::mat4 UpdateView();
	
	inline glm::vec2 getPos() const { return m_Pos; }

private:
	glm::vec2 m_Pos;

	Entity * m_FollowEntity;
};

