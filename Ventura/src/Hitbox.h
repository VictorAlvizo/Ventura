#pragma once
#include "HitboxRenderer.h"
#include "ResourceManager.h"

class Entity;

class Hitbox {
public:
	//Constructer with no unique information
	Hitbox();
	//If the hitbox belongs to an entity, pass in the pointer to the entity, if not leave it as is
	Hitbox(glm::vec2 hitboxPos, glm::vec2 hitboxSize, glm::vec2 velocity = glm::vec2(0.0f), float rotation = 0.0f, float mass = 1.0f, Entity * parentEntity = nullptr);
	~Hitbox();

	//Follow camera should be true if the hitbox should remain unaffected by camera movement
	void Draw(glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f), bool followCamera = true);
	//Move the hitbox to a new position
	void Move(glm::vec2 newPos);
	//Gradually move the hitbox using its velocity
	void Translate(float deltaTime);
	//Call this method if the hitbox should be affected by gravity
	void GravityMovement(float gravity, float deltaTime);

	//Get the corners of the hitbox, override this method if the hitbox is not a square
	virtual std::vector<glm::vec2> GetHitboxCorners();

	//Returns the position of the hitbox
	inline glm::vec2 getPos() const { return m_Pos; }
	//Retuurns the size of the hitbox
	inline glm::vec2 getSize() const { return m_Size; }
	//Returns the mass of the hitbox
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