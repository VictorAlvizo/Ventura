#include "Entity.h"

Entity::Entity()
	:m_Pos(0.0f), m_Size(1.0f), m_Hitbox(0.0f), m_Color(1.0f)
{
	m_Rotation = 0.0f;
	m_Destroyed = false;
	m_Flipped = false;
}

Entity::Entity(std::shared_ptr<Texture>& texture, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 hbSize)
	:m_Texture(texture), m_Pos(pos), m_Size(size), m_Hitbox(hbSize),  m_Color(color)
{
	m_SpriteSheet = nullptr;
	m_Rotation = 0.0f;
	m_Destroyed = false;
	m_Flipped = false;
}

Entity::Entity(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 hbSize)
	:m_Texture(texture), m_Pos(pos), m_Size(size), m_Hitbox(hbSize), m_Color(color)
{
	//Not wanting a glm::vec2 for sprite size to avoid ambiguity between the overloaded functions
	m_SpriteSheet = new SpriteSheetReader(texture, glm::vec2(spriteX, spriteY));
	m_Rotation = 0.0f;
	m_Destroyed = false;
	m_Flipped = false;
}

Entity::~Entity() {
	delete m_SpriteSheet;
}

void Entity::Draw(SpriteRenderer& spriteRenderer, glm::vec3 color) {
	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, m_Rotation, m_Color = (color == glm::vec3(1.0f)) ? m_Color : color);
}

void Entity::Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec3 color) {
	if (!m_SpriteSheet) {
		std::cout << "Error: Entity does not have a dedicated sprite sheet" << std::endl;
	}
	else {
		spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, m_Rotation, m_Color = (color == glm::vec3(1.0f)) ? m_Color : color, 
			m_SpriteSheet->getTexUV(spritePos.x, spritePos.y, m_Flipped));
	}
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

void Entity::Translate(glm::vec2 trans, float deltaTime) {
	m_Pos += trans * deltaTime;
}

void Entity::Flip(bool flip) {
	m_Flipped = flip;
}
