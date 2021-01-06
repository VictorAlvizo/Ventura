#pragma once
#include "HitboxRenderer.h"
#include "ResourceManager.h"

class Entity;

class Hitbox {
public:
	Hitbox();
	Hitbox(glm::vec2 hitboxPos, glm::vec2 hitboxSize, float rotation = 0.0f, float mass = 1.0f, Entity * parentEntity = nullptr);
	~Hitbox();

	void Draw(glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f));
	void Move(glm::vec2 newPos);
	void Translate(float deltaTime);
	void GravityMovement(float gravity, float deltaTime);

	virtual std::vector<glm::vec2> GetHitboxCorners();

	inline glm::vec2 getPos() const { return m_Pos; }
	inline glm::vec2 getSize() const { return m_Size; }
	inline float getMass() const { return m_Mass; }

	bool m_ShowHitbox;
	bool m_ActiveHitbox;

	float m_Rotation;

	glm::vec2 m_Velocity;

protected:
	HitboxRenderer * m_Renderer;
	Entity * m_Entity;

	glm::vec2 m_Pos, m_Size;

	float m_Mass;
};