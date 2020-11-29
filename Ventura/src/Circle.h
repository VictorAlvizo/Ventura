#pragma once
#include "Entity.h"

class Circle : public Entity {
public:
	Circle();
	Circle(std::shared_ptr<Texture>& texture, glm::vec2 pos, float radius, glm::vec2 hbPos = glm::vec2(0.0f), float hbRadius = 0.0f);
	~Circle();

	inline float getRadius() const { return m_Radius; }
	inline float getHitboxRadius() const { return m_HBRadius; }

private:
	float m_Radius;
	float m_HBRadius;
};

