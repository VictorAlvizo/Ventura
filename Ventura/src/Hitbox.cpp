#include "Hitbox.h"

Hitbox::Hitbox() 
	:m_HitboxSize(0.0f), m_HitboxOffset(0.0f)
{
	m_Renderer = new HitboxRenderer(ResourceManager::Get<Shader>("hboutline"));
	m_ShowHitbox = true;
}

Hitbox::Hitbox(glm::vec2 hitboxSize, glm::vec2 hitboxOffset)
	:m_HitboxSize(hitboxSize), m_HitboxOffset(hitboxOffset)
{
	m_Renderer = new HitboxRenderer(ResourceManager::Get<Shader>("hboutline"));
	m_ShowHitbox = true;
}

Hitbox::~Hitbox() {
	delete m_Renderer;
	m_Renderer = nullptr;
}

void Hitbox::Draw(glm::vec2 pos, float rotation, glm::vec3 color) {
	m_Renderer->DrawOutline(pos + m_HitboxOffset, m_HitboxSize, rotation, color);
}

std::vector<glm::vec2> Hitbox::GetHitboxCorners(glm::vec2 pos, float rotation) {
	std::vector<glm::vec2> corners;
	glm::vec2 center = glm::vec2(pos.x + (m_HitboxSize.x / 2.0f), pos.y + (m_HitboxSize.y / 2.0f));
	glm::vec2 hbPos = pos + m_HitboxOffset;

	corners.push_back({ hbPos.x, hbPos.y }); //Top left
	corners.push_back({ hbPos.x + m_HitboxSize.x, hbPos.y }); //Top right
	corners.push_back({ hbPos.x, hbPos.y + m_HitboxSize.y }); //Bottom left
	corners.push_back({ hbPos.x + m_HitboxSize.x, hbPos.y + m_HitboxSize.y }); //Bottom right

	for (glm::vec2& corner : corners) {
		corner -= center;

		corner = glm::vec2(corner.x * glm::cos(glm::radians(rotation)) - corner.y * glm::sin(glm::radians(rotation)),
			corner.x * glm::sin(glm::radians(rotation)) + corner.y * glm::cos(glm::radians(rotation)));

		corner += center;
	}

	return corners;
}
