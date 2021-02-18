#include "HitCircle.h"

HitCircle::HitCircle()
	:Hitbox(), m_Radius(1)
{
	m_Texture = ResourceManager::Get<Texture>("hitboxCircle");
}

HitCircle::HitCircle(glm::vec2 hitboxPos, float radius, glm::vec2 velocity, float rotation, float mass, Entity * parentEntity)
	:Hitbox(hitboxPos, glm::vec2(radius * 2.0f), velocity, rotation, mass, parentEntity), m_Radius(radius)
{
	m_Texture = ResourceManager::Get<Texture>("hitboxCircle");
}

HitCircle::~HitCircle() {
	//holder
}

void HitCircle::Draw(SpriteRenderer& spriteRenderer, glm::vec3 color, bool followCamera) {
	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, false, followCamera, m_Rotation, glm::vec4(color, 1.0f));
}
