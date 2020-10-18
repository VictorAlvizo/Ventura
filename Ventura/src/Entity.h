#pragma once
#include "SpriteRenderer.h"

class Entity {
public:
	Entity();
	Entity(Texture& texture, glm::vec2 pos, glm::vec2 size, glm::vec2 hbSize = glm::vec2(0.0f), glm::vec2 velocity = glm::vec2(0.0f), 
		glm::vec3 color = glm::vec3(1.0f));

	virtual void Draw(SpriteRenderer& spriteRenderer);

protected:
	Texture m_Texture;

	glm::vec2 m_Pos, m_Size, m_Hitbox, m_Velocity;
	glm::vec3 m_Color;

	float m_Rotation;
	bool m_Destroyed;
};

