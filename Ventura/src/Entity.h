#pragma once
#include "HitboxRenderer.h"
#include "SpriteSheetReader.h"
#include "Component.h"
#include "ResourceManager.h"

class Entity {
public:
	Entity();
	Entity(std::shared_ptr<Texture>& texture, glm::vec2 pos, glm::vec2 size, glm::vec2 hbPos = glm::vec2(0.0f), glm::vec2 hbSize = glm::vec2(0.0f));
	//Only if you want animated sprites / use of a spritesheet. Provide the dimensions of 1 sprite
	Entity(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, glm::vec2 size, glm::vec2 hbPos = glm::vec2(0.0f), glm::vec2 hbSize = glm::vec2(0.0f));
	~Entity();

	virtual void Draw(SpriteRenderer& spriteRenderer, glm::vec3 color = glm::vec3(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f));
	virtual void Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec3 color = glm::vec3(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f));

	virtual std::vector<glm::vec2> GetCorners();
	virtual std::vector<glm::vec2> GetHitboxCorners();

	void Move(glm::vec2 newPos);
	void MoveHitbox(glm::vec2 newPos); //Hitbox focused movement
	void Translate(glm::vec2 trans, float deltaTime);
	void Flip(bool flip);

	template<typename T>
	void AddComponent(const std::string& compName, T& newComponent) {
		m_Components.Add(compName, newComponent);
	}

	template<>
	void AddComponent<AnimationCycle>(const std::string& compName, AnimationCycle& newComponent) {
		m_Components.Add<AnimationCycle>(compName, newComponent);
	}

	template<typename T>
	std::unique_ptr<T>& GetComponent(const std::string& name) {
		return m_Components.Get(name);
	}

	template<>
	std::unique_ptr<AnimationCycle>& GetComponent(const std::string& name) {
		return m_Components.Get<AnimationCycle>(name);
	}

	inline glm::vec2 getPos() const { return m_Pos; }
	inline glm::vec2 getSize() const { return m_Size; }
	inline glm::vec2 getHitboxSize() const { return m_Hitbox; }
	inline glm::vec2 getHitboxPos() const { return m_HitboxPos; }
	inline glm::vec2 getHitboxOffset() const { return m_HitboxOffset; }
	inline bool isFlipped() const { return m_Flipped; }

	float m_Rotation;
	bool m_ShowHitbox;
	std::string m_Tag;

protected:
	std::shared_ptr<Texture> m_Texture;
	SpriteSheetReader * m_SpriteSheet;
	HitboxRenderer * m_HBRenderer;

	//m_Hitbox is the size of the hitbox
	glm::vec2 m_Pos, m_Size, m_Hitbox, m_HitboxPos, m_HitboxOffset;

	bool m_Destroyed;
	bool m_Flipped;

	Component m_Components;
};
