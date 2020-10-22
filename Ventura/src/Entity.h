#pragma once
#include "SpriteRenderer.h"

class Entity {
public:
	Entity();
	Entity(Texture& texture, glm::vec2 pos, glm::vec2 size, glm::vec2 hbSize = glm::vec2(0.0f), glm::vec2 velocity = glm::vec2(0.0f), 
		glm::vec3 color = glm::vec3(1.0f));

	virtual void Draw(SpriteRenderer& spriteRenderer);
	void Move(glm::vec2 newPos);

	inline glm::vec2 getPos() const { return m_Pos; }
	inline glm::vec2 getSize() const { return m_Size; }
	inline glm::vec2 getHitbox() const { return m_Hitbox; }
	inline glm::vec2 getVelocity() const { return m_Velocity; }
	inline glm::vec3 getColor() const { return m_Color; }

protected:
	Texture m_Texture;

	glm::vec2 m_Pos, m_Size, m_Hitbox, m_Velocity;
	glm::vec3 m_Color;

	float m_Rotation;
	bool m_Destroyed;
};

