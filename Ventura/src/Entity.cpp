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

std::vector<glm::vec2> Entity::GetCorners() { //Default is a rectangle / square
	std::vector<glm::vec2> corners;
	glm::vec2 center = glm::vec2(m_Pos.x + (m_Size.x / 2.0f), m_Pos.y + (m_Size.y / 2.0f));

	corners.push_back({ m_Pos.x, m_Pos.y }); //Top left
	corners.push_back({ m_Pos.x + m_Size.x, m_Pos.y }); //Top right
	corners.push_back({ m_Pos.x, m_Pos.y + m_Size.y }); //Bottom left
	corners.push_back({ m_Pos.x + m_Size.x, m_Pos.y + m_Size.y }); //Bottom right

	for (glm::vec2& corner : corners) {
		corner -= center;

		corner = glm::vec2(corner.x * glm::cos(glm::radians(m_Rotation)) - corner.y * glm::sin(glm::radians(m_Rotation)),
			corner.x * glm::sin(glm::radians(m_Rotation)) + corner.y * glm::cos(glm::radians(m_Rotation)));

		corner += center;
	}

	return corners;
}

void Entity::Move(glm::vec2 newPos) {
	m_Pos = newPos;
}
