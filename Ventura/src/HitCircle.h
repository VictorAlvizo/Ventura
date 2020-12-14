#pragma once
#include "Hitbox.h"
#include "HitCircle.h"

class HitCircle : public Hitbox {
public:
	HitCircle();
	HitCircle(glm::vec2 hitboxPos, float radius, float rotation = 0.0f, Entity * parentEntity = nullptr);
	~HitCircle();

	void Draw(SpriteRenderer& spriteRenderer, glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f));

	inline float getRadius() const { return m_Radius; }

private:
	std::shared_ptr<Texture> m_Texture;
	float m_Radius;
};

