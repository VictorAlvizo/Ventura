#include "Entity.h"

Entity::Entity()
	:m_Pos(0.0f), m_Size(1.0f), m_HitboxOffset(0.0f)
{
	m_SpriteSheet = nullptr;
	m_Rotation = 0.0f;
	m_Destroyed = false;
	m_Flipped = false;

	m_Hitbox = nullptr;
}

Entity::Entity(std::shared_ptr<Texture>& texture, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 hbPos, glm::vec2 hbSize, bool childClass)
	:m_Texture(texture), m_Pos(pos), m_Size(size), m_HitboxOffset(hbPos), m_Rotation(rotation)
{
	m_SpriteSheet = nullptr;
	m_Destroyed = false;
	m_Flipped = false;

	if (!childClass) {
		//Set hitbox varibles
		glm::vec2 hitboxSize = (hbSize != glm::vec2(0.0f)) ? hbSize : m_Size;
		m_Hitbox = new Hitbox(glm::vec2(hbPos + pos), hitboxSize, m_Rotation, this);
	}
}

Entity::Entity(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 hbPos, glm::vec2 hbSize, bool childClass)
	:m_Texture(texture), m_Pos(pos), m_Size(size), m_HitboxOffset(hbPos)
{
	//Not wanting a glm::vec2 for sprite size to avoid ambiguity between the overloaded functions
	m_SpriteSheet = new SpriteSheetReader(texture, glm::vec2(spriteX, spriteY));
	m_Rotation = 0.0f;
	m_Destroyed = false;
	m_Flipped = false;

	if (!childClass) {
		glm::vec2 hitboxSize = (hbSize != glm::vec2(0.0f)) ? hbSize : m_Size;
		m_Hitbox = new Hitbox(glm::vec2(hbPos + pos), hitboxSize, m_Rotation, this);
	}
}

Entity::~Entity() {
	delete m_SpriteSheet;
	m_SpriteSheet = nullptr;

	delete m_Hitbox;
	m_Hitbox = nullptr;
}

void Entity::Draw(SpriteRenderer& spriteRenderer, glm::vec3 color, glm::vec3 hbColor) {
	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, m_Rotation, glm::vec4(color, 1.0f));

	if (m_Hitbox->m_ShowHitbox) {
		m_Hitbox->Draw(hbColor);
	}
}

void Entity::Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec3 color, glm::vec3 hbColor) {
	if (!m_SpriteSheet) {
		std::cout << "Error: Entity does not have a dedicated sprite sheet" << std::endl;
	}
	else {
		spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, m_Rotation, glm::vec4(color, 1.0f), 
			m_SpriteSheet->getTexUV(spritePos.x, spritePos.y, m_Flipped));

		if (m_Hitbox->m_ShowHitbox) {
			m_Hitbox->Draw(hbColor);
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
	m_Hitbox->Move(m_Pos + m_HitboxOffset);
}

void Entity::Translate(glm::vec2 trans, float deltaTime) {
	m_Pos += trans * deltaTime;
	m_Hitbox->Move(m_Pos + m_HitboxOffset);
}

void Entity::Flip(bool flip) {
	m_Flipped = flip;
}

void Entity::SetRotation(float newRotation) {
	m_Rotation = newRotation;
	m_Hitbox->m_Rotation = newRotation;
}

void Entity::MovePos() {
	m_Pos = m_Hitbox->getPos() - m_HitboxOffset;
}
