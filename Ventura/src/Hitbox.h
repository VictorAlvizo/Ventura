#pragma once
#include "HitboxRenderer.h"
#include "ResourceManager.h"

class Hitbox {
public:
	Hitbox();
	Hitbox(glm::vec2 hitboxSize, glm::vec2 hitboxOffset = glm::vec2(0.0f));
	~Hitbox();

	void Draw(glm::vec2 pos, float rotation = 0.0f, glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f));

	virtual std::vector<glm::vec2> GetHitboxCorners(glm::vec2 pos, float rotation = 0.0f);

	inline glm::vec2 getSize() const { return m_HitboxSize; }
	inline glm::vec2 getOffset() const { return m_HitboxOffset; }

	bool m_ShowHitbox;

private:
	HitboxRenderer * m_Renderer;

	glm::vec2 m_HitboxSize, m_HitboxOffset;
};

