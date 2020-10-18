#include "Entity.h"

Entity::Entity()
	:m_Pos(0.0f), m_Size(1.0f), m_Hitbox(0.0f), m_Color(1.0f), m_Velocity(0.0f)
{
	m_Rotation = 0.0f;
	m_Destroyed = false;
}

Entity::Entity(Texture& texture, glm::vec2 pos, glm::vec2 size, glm::vec2 hbSize, glm::vec2 velocity, glm::vec3 color)
	:m_Texture(texture), m_Pos(pos), m_Size(size), m_Hitbox(hbSize), m_Velocity(velocity), m_Color(color)
{
	m_Rotation = 0.0f;
	m_Destroyed = false;
}

void Entity::Draw(SpriteRenderer& spriteRenderer) {
	spriteRenderer.DrawSprite(m_Texture, m_Pos, m_Size, m_Rotation, m_Color);
}
