#include "Entity.h"

Entity::Entity()
	:m_Pos(0.0f), m_Size(1.0f), m_HitboxPos(0.0f), m_Hitbox(0.0f), m_HitboxOffset(0.0f)
{
	m_SpriteSheet = nullptr;
	m_Rotation = 0.0f;
	m_Destroyed = false;
	m_Flipped = false;
	m_ShowHitbox = false;

	m_HBRenderer = new HitboxRenderer(ResourceManager::Get<Shader>("hboutline"));
}

Entity::Entity(std::shared_ptr<Texture>& texture, glm::vec2 pos, glm::vec2 size, glm::vec2 hbPos, glm::vec2 hbSize)
	:m_Texture(texture), m_Pos(pos), m_Size(size)
{
	m_SpriteSheet = nullptr;
	m_Rotation = 0.0f;
	m_Destroyed = false;
	m_Flipped = false;
	m_ShowHitbox = false;

	//Set hitbox varibles
	m_Hitbox = (hbSize != glm::vec2(0.0f)) ? m_Hitbox = hbSize : m_Hitbox = m_Size;
	m_HitboxOffset = hbPos;
	m_HitboxPos = m_HitboxOffset + pos;

	m_HBRenderer = new HitboxRenderer(ResourceManager::Get<Shader>("hboutline"));
}

Entity::Entity(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, glm::vec2 size, glm::vec2 hbPos, glm::vec2 hbSize)
	:m_Texture(texture), m_Pos(pos), m_Size(size)
{
	//Not wanting a glm::vec2 for sprite size to avoid ambiguity between the overloaded functions
	m_SpriteSheet = new SpriteSheetReader(texture, glm::vec2(spriteX, spriteY));
	m_Rotation = 0.0f;
	m_Destroyed = false;
	m_Flipped = false;
	m_ShowHitbox = false;

	m_Hitbox = (hbSize != glm::vec2(0.0f)) ? m_Hitbox = hbSize : m_Hitbox = m_Size;
	m_HitboxOffset = hbPos;
	m_HitboxPos = m_HitboxOffset + pos;

	m_HBRenderer = new HitboxRenderer(ResourceManager::Get<Shader>("hboutline"));
}

Entity::~Entity() {
	delete m_SpriteSheet;
	m_SpriteSheet = nullptr;

	delete m_HBRenderer;
	m_HBRenderer = nullptr;
}

void Entity::Draw(SpriteRenderer& spriteRenderer, glm::vec3 color, glm::vec3 hbColor) {
	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, m_Rotation, color);

	if (m_ShowHitbox) {
		m_HBRenderer->DrawOutline(m_HitboxPos, m_Hitbox, m_Rotation, hbColor);
	}
}

void Entity::Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec3 color, glm::vec3 hbColor) {
	if (!m_SpriteSheet) {
		std::cout << "Error: Entity does not have a dedicated sprite sheet" << std::endl;
	}
	else {
		spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, m_Rotation, color, 
			m_SpriteSheet->getTexUV(spritePos.x, spritePos.y, m_Flipped));

		if (m_ShowHitbox) {
			m_HBRenderer->DrawOutline(m_HitboxPos, m_Hitbox, m_Rotation, hbColor);
		}
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
	m_HitboxPos = m_HitboxOffset + m_Pos;
}

void Entity::Translate(glm::vec2 trans, float deltaTime) {
	m_Pos += trans * deltaTime;
	m_HitboxPos = m_HitboxOffset + m_Pos;
}

void Entity::Flip(bool flip) {
	m_Flipped = flip;
}