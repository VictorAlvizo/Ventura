#pragma once
#include "Entity.h"

class Circle : public Entity {
public:
	Circle();
	Circle(std::shared_ptr<Texture>& texture, glm::vec2 pos, float radius, glm::vec2 hbPos = glm::vec2(0.0f), float hbRadius = 0.0f);
	~Circle();

	void Draw(SpriteRenderer& spriteRenderer, glm::vec3 color = glm::vec3(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f)) override;
	void Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec3 color = glm::vec3(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f)) override;

	inline float getRadius() const { return m_Radius; }
	inline float getHitboxRadius() const { return m_HBRadius; }

private:
	float m_Radius;
	float m_HBRadius;
};

