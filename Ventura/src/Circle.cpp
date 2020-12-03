#include "Circle.h"

Circle::Circle() 
	:Entity(), m_Radius(1), m_HBRadius(0.0f)
{
	//holder
}

Circle::Circle(std::shared_ptr<Texture>& texture, glm::vec2 pos, float radius, glm::vec2 hbPos, float hbRadius)
	:Entity(texture, pos, glm::vec2(radius * 2.0f), hbPos, glm::vec2(hbRadius * 2.0f)), m_Radius(radius)
{
	m_HBRadius = (hbRadius == 0.0f) ? radius : hbRadius;
}

Circle::~Circle() {
	//holder
}

void Circle::Draw(SpriteRenderer& spriteRenderer, glm::vec3 color, glm::vec3 hbColor) {
	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, m_Rotation, color);

	if (m_ShowHitbox) {
		spriteRenderer.DrawSprite(*ResourceManager::Get<Texture>("hitboxCircle"), m_HitboxPos, m_Hitbox, false, 0.0f, hbColor);
	}
}

void Circle::Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec3 color, glm::vec3 hbColor) {
	if (!m_SpriteSheet) {
		std::cout << "Error: Entity does not have a dedicated sprite sheet" << std::endl;
	}
	else {
		spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, m_Rotation, color, 
			m_SpriteSheet->getTexUV(spritePos.x, spritePos.y, m_Flipped));

		if (m_ShowHitbox) {
			spriteRenderer.DrawSprite(*ResourceManager::Get<Texture>("hitboxCircle"), m_HitboxPos, m_Hitbox, false, 0.0f, hbColor);
		}
	}
}
