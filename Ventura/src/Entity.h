#pragma once
#include "SpriteRenderer.h"
#include "SpriteSheetReader.h"

class Entity {
public:
	Entity();
	Entity(std::shared_ptr<Texture>& texture, glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 hbSize = glm::vec2(0.0f));
	//Only if you want animated sprites / use of a spritesheet. Provide the dimensions of 1 sprite
	Entity(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 hbSize = glm::vec2(0.0f));
	~Entity();

	virtual void Draw(SpriteRenderer& spriteRenderer, glm::vec3 color = glm::vec3(1.0f));
	virtual void Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec3 color = glm::vec3(1.0f));

	virtual std::vector<glm::vec2> GetCorners();

	void Move(glm::vec2 newPos);
	void Flip();

	inline glm::vec2 getPos() const { return m_Pos; }
	inline glm::vec2 getSize() const { return m_Size; }
	inline glm::vec2 getHitbox() const { return m_Hitbox; }
	inline glm::vec3 getColor() const { return m_Color; }
	inline bool isFlipped() const { return m_Flipped; }

	float m_Rotation;

protected:
	std::shared_ptr<Texture> m_Texture;
	SpriteSheetReader * m_SpriteSheet;

	glm::vec2 m_Pos, m_Size, m_Hitbox;
	glm::vec3 m_Color;

	bool m_Destroyed;
	bool m_Flipped;
};

