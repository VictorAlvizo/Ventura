#include "Entity.h"
#include "Camera.h"

Entity::Entity()
	:m_Pos(0.0f), m_Size(1.0f), m_HitboxOffset(0.0f), m_Mass(1.0f), m_Velocity(0.0f), m_Rotation(0.0f)
{
	m_Destroyed = false;
	m_Flipped = false;
	m_Copied = false;
	m_FromFile = false;
	m_Tag = "Entity";

	m_SpriteSheet = nullptr;
	m_Hitbox = nullptr;
	m_Camera = nullptr;
}

Entity::Entity(std::shared_ptr<Texture>& texture, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 hbPos, glm::vec2 hbSize, float mass, bool childClass)
	:m_Texture(texture), m_Pos(pos), m_Size(size), m_Rotation(rotation), m_HitboxOffset(hbPos), m_Mass(mass), m_Velocity(0.0f)
{
	m_SpriteSheet = nullptr;
	m_Camera = nullptr;

	m_Destroyed = false;
	m_Flipped = false;
	m_Copied = false;
	m_FromFile = false;
	m_Tag = "Entity";

	if (!childClass) {
		//Set hitbox varibles
		glm::vec2 hitboxSize = (hbSize != glm::vec2(0.0f)) ? hbSize : m_Size;
		m_Hitbox = new Hitbox(glm::vec2(hbPos + pos), hitboxSize, m_Velocity, m_Rotation, m_Mass, this);
	}
}

Entity::Entity(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 hbPos, glm::vec2 hbSize, float mass, bool childClass)
	:m_Texture(texture), m_Pos(pos), m_Size(size), m_Rotation(rotation), m_HitboxOffset(hbPos), m_Mass(mass), m_Velocity(0.0f)
{
	//Not wanting a glm::vec2 for sprite size to avoid ambiguity between the overloaded functions
	m_SpriteSheet = new SpriteSheetReader(texture, glm::vec2(spriteX, spriteY));
	m_Camera = nullptr;

	m_Destroyed = false;
	m_Flipped = false;
	m_Copied = false;
	m_FromFile = false;
	m_Tag = "Entity";

	if (!childClass) {
		glm::vec2 hitboxSize = (hbSize != glm::vec2(0.0f)) ? hbSize : m_Size;
		m_Hitbox = new Hitbox(glm::vec2(hbPos + pos), hitboxSize, m_Velocity, m_Rotation, m_Mass, this);
	}
}

Entity::Entity(const Entity& copy) {
	//Component does not get copied, you have add them again. Copying the components donsen't work well
	//as they're pointers. So they would both have the same values no matter what
	//Camera not copied as well, also hitbox settings revert as if brand new cause well... it is
	m_Copied = (m_FromFile) ? false : true;
	m_Destroyed = copy.m_Destroyed;
	m_Flipped = copy.m_Flipped;
	m_Tag = copy.m_Tag;
	m_Rotation = copy.m_Rotation;
	m_Mass = copy.m_Mass;
	m_Pos = copy.m_Pos;
	m_Velocity = copy.m_Velocity;
	m_Size = copy.m_Size;
	m_SpriteSheet = (copy.m_SpriteSheet) ? new SpriteSheetReader(copy.m_Texture, copy.m_SpriteSheet->getSpriteSize()) : nullptr;
	m_Camera = nullptr;
	m_Hitbox = new Hitbox(copy.getHitbox()->getPos(), copy.getHitbox()->getSize(), copy.getHitbox()->m_Velocity, copy.getHitbox()->m_Rotation, copy.getHitbox()->getMass(), this);
	m_HitboxOffset = copy.m_HitboxOffset;
	m_Texture = copy.m_Texture;
}

void Entity::operator=(const Entity& copy) {
	m_Copied = true;
	m_Destroyed = copy.m_Destroyed;
	m_Flipped = copy.m_Flipped;
	m_Tag = copy.m_Tag;
	m_Rotation = copy.m_Rotation;
	m_Mass = copy.m_Mass;
	m_Pos = copy.m_Pos;
	m_Velocity = copy.m_Velocity;
	m_Size = copy.m_Size;
	m_SpriteSheet = (copy.m_SpriteSheet) ? new SpriteSheetReader(copy.m_Texture, copy.m_SpriteSheet->getSpriteSize()) : nullptr;
	m_Camera = nullptr;
	m_Hitbox = new Hitbox(copy.getHitbox()->getPos(), copy.getHitbox()->getSize(), copy.getHitbox()->m_Velocity, copy.getHitbox()->m_Rotation, copy.getHitbox()->getMass(), this);
	m_HitboxOffset = copy.m_HitboxOffset;
	m_Texture = copy.m_Texture;
}

Entity::~Entity() {
	if (!m_Copied) {
		delete m_SpriteSheet;
		m_SpriteSheet = nullptr;

		delete m_Hitbox;
		m_Hitbox = nullptr;

		m_Camera = nullptr;
	}
}

void Entity::Draw(SpriteRenderer& spriteRenderer, glm::vec4 color, glm::vec3 hbColor) {
	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, true, m_Rotation, color);

	if (m_Hitbox->m_ShowHitbox) {
		m_Hitbox->Draw(hbColor);
	}
}

void Entity::Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec4 color, glm::vec3 hbColor) {
	if (!m_SpriteSheet) {
		std::cout << "Error: Entity does not have a dedicated sprite sheet" << std::endl;
	}
	else {
		spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, true, m_Rotation, color, 
			m_SpriteSheet->getTexUV(spritePos, m_Flipped));

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

	if (m_Camera) {
		m_Camera->Move(m_Pos - (m_Camera->getWindowSize() / 2.0f - m_Size / 2.0f));
	}
}

void Entity::Translate(float deltaTime) {
	m_Pos += m_Velocity * deltaTime;
	m_Hitbox->Move(m_Pos + m_HitboxOffset);

	if (m_Camera) {
		m_Camera->Move(m_Pos - (m_Camera->getWindowSize() / 2.0f - m_Size / 2.0f));
	}
}

void Entity::GravityMovement(float gravity, float deltaTime) {
	m_Pos.y += gravity * deltaTime; //TODO: Physics is frame-rate dependent, want to change that
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

	if (m_Camera) {
		m_Camera->Move(glm::vec2(m_Pos.x - (m_Size.x), m_Pos.y - (m_Size.y / 2.0f)));
	}
}

void Entity::AttachCamera(Camera * camera) {
	m_Camera = camera;
	m_Camera->Move(glm::vec2(m_Pos.x - (m_Size.x), m_Pos.y - (m_Size.y / 2.0f)));
}

void Entity::DetachCamera() {
	m_Camera = nullptr;
}