#include "Hitbox.h"
#include "Entity.h"

Hitbox::Hitbox()
	:m_Pos(0.0f), m_Size(1.0f), m_Rotation(0.0f), m_Mass(1.0f), m_Velocity(0.0f)
{
	m_Renderer = new HitboxRenderer(ResourceManager::Get<Shader>("hboutline"));
	m_ShowHitbox = true;
	m_ActiveHitbox = true;
	m_Entity = nullptr;
}

Hitbox::Hitbox(glm::vec2 hitboxPos, glm::vec2 hitboxSize, glm::vec2 velocity, float rotation, float mass, Entity * parentEntity)
	:m_Pos(hitboxPos), m_Size(hitboxSize), m_Velocity(velocity), m_Rotation(rotation), m_Entity(parentEntity), m_Mass(mass)
{
	m_Renderer = new HitboxRenderer(ResourceManager::Get<Shader>("hboutline"));
	m_ShowHitbox = true;
	m_ActiveHitbox = true;
}

Hitbox::~Hitbox() {
	delete m_Renderer;
	m_Renderer = nullptr;

	m_Entity = nullptr; //Can't delete, will cause both classes trying to delete eachother
}

void Hitbox::Draw(glm::vec3 color, bool followCamera) {
	if (m_ShowHitbox) {
		m_Renderer->DrawOutline(m_Pos, m_Size, m_Rotation, followCamera, color);
	}
}

void Hitbox::Move(glm::vec2 newPos) {
	m_Pos = newPos;

	if (m_Entity) {
		m_Entity->MovePos();
	}
}

void Hitbox::Translate(float deltaTime) {
	m_Pos += m_Velocity * deltaTime;

	if (m_Entity) {
		m_Entity->MovePos();
	}
}

void Hitbox::GravityMovement(float gravity, float deltaTime) {
	m_Pos.y += gravity * deltaTime;

	//Would recommend only choosing to effect either the entity or its hitbox, not both at the same time.
	if (m_Entity) {
		m_Entity->MovePos();
	}
}

std::vector<glm::vec2> Hitbox::GetHitboxCorners() {
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