#include "HitCircle.h"

HitCircle::HitCircle()
	:Hitbox(), m_Radius(1)
{
	m_Texture = ResourceManager::Get<Texture>("hitboxCircle");
	m_Rotation = 0.0f;
}

HitCircle::HitCircle(glm::vec2 hitboxPos, float radius, float rotation)
	:Hitbox(hitboxPos, glm::vec2(radius * 2.0f), rotation), m_Radius(radius)
{
	m_Texture = ResourceManager::Get<Texture>("hitboxCircle");
}

HitCircle::~HitCircle() {
	//holder
}

void HitCircle::Draw(SpriteRenderer& spriteRenderer, glm::vec3 color) {
	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, false, m_Rotation, color);
}
