#include "Circle.h"

Circle::Circle() 
	:Entity(), m_Radius(1)
{
	//holder
}

Circle::Circle(std::shared_ptr<Texture>& texture, glm::vec2 pos, float radius, float rotation, glm::vec2 hbPos, float hbRadius) 
	:Entity(texture, pos, glm::vec2(radius * 2.0f), rotation, glm::vec2(0.0f), glm::vec2(0.0f), true), m_Radius(radius)
{
	float hitRad = (hbRadius == 0.0f) ? radius : hbRadius;
	m_Hitbox = new HitCircle(hbPos + pos, hitRad, rotation, this);
}

Circle::Circle(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, float radius, float rotation, glm::vec2 hbPos, float hbRadius)
	:Entity(texture, spriteX, spriteY, pos, glm::vec2(radius * 2.0f), rotation, glm::vec2(0.0f), glm::vec2(0.0f), true), m_Radius(radius)
{
	float hitRad = (hbRadius == 0.0f) ? radius : hbRadius;
	m_Hitbox = new HitCircle(hbPos + pos, hitRad, rotation, this);
}

Circle::~Circle() {
	//holder
}

void Circle::Draw(SpriteRenderer& spriteRenderer, glm::vec3 color, glm::vec3 hbColor) {
	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, m_Rotation, glm::vec4(color, 1.0f));

	if (m_Hitbox->m_ShowHitbox) {
		dynamic_cast<HitCircle *>(m_Hitbox)->Draw(spriteRenderer, hbColor);
	}
}

void Circle::Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec3 color, glm::vec3 hbColor) {
	if (!m_SpriteSheet) {
		std::cout << "Error: Entity does not have a dedicated sprite sheet" << std::endl;
	}
	else {
		spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, m_Flipped, m_Rotation, glm::vec4(color, 1.0f), 
			m_SpriteSheet->getTexUV(spritePos.x, spritePos.y, m_Flipped));

		if (m_Hitbox->m_ShowHitbox) {
			dynamic_cast<HitCircle *>(m_Hitbox)->Draw(spriteRenderer, hbColor);
		}
	}
}
