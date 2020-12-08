#pragma once
#include "HitboxRenderer.h"
#include "ResourceManager.h"

class Hitbox {
public:
	Hitbox();
	Hitbox(glm::vec2 hitboxPos, glm::vec2 hitboxSize, float rotation = 0.0f);
	~Hitbox();

	void Draw(glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f));
	void Move(glm::vec2 newPos);
	void Translate(glm::vec2 trans, float deltaTime);

	virtual std::vector<glm::vec2> GetHitboxCorners();

	inline glm::vec2 getPos() const { return m_Pos; }
	inline glm::vec2 getSize() const { return m_Size; }

	bool m_ShowHitbox;
	bool m_ActiveHitbox;

	float m_Rotation = 0.0f;

private:
	HitboxRenderer * m_Renderer;

	glm::vec2 m_Pos, m_Size;
};