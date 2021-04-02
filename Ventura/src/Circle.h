#pragma once
#include "Entity.h"
#include "HitCircle.h"

class Circle : public Entity {
public:
	//Best not to use this constructer as many varibles will be left uninitilzied
	Circle();
	//Constructer for a circle entity with a static image
	Circle(std::shared_ptr<Texture>& texture, glm::vec2 pos, float radius, float rotation = 0.0f, glm::vec2 hbPos = glm::vec2(0.0f), float hbRadius = 0.0f, float mass = 1.0f);
	//Constructer for a circle entity that has an animation, provide the dimesions of each sprite
	Circle(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, float radius, float rotation = 0.0f, glm::vec2 hbPos = glm::vec2(0.0f), float hbRadius = 0.0f, float mass = 1.0f);
	~Circle();

	//Draw the circle entity that uses a static image.
	void Draw(SpriteRenderer& spriteRenderer, glm::vec4 color = glm::vec4(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f)) override;
	//Draw the circle entity that uses animation, spritepos shold be provided by the animation object associated with the entity
	void Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec4 color = glm::vec4(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f)) override;

	inline float getRadius() const { return m_Radius; }

private:
	float m_Radius;
};

