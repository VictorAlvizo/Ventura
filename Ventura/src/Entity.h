#pragma once
#include "Hitbox.h"
#include "SpriteSheetReader.h"
#include "Component.h"
#include "ResourceManager.h"

class Camera;

class Entity {
public:
	Entity();
	Entity(std::shared_ptr<Texture>& texture, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, glm::vec2 hbPos = glm::vec2(0.0f), glm::vec2 hbSize = glm::vec2(0.0f), bool childClass = false);
	//Only if you want animated sprites / use of a spritesheet. Provide the dimensions of 1 sprite
	Entity(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, glm::vec2 hbPos = glm::vec2(0.0f), glm::vec2 hbSize = glm::vec2(0.0f), bool childClass = false);
	~Entity();

	virtual void Draw(SpriteRenderer& spriteRenderer, glm::vec3 color = glm::vec3(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f));
	virtual void Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec3 color = glm::vec3(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f));

	virtual std::vector<glm::vec2> GetCorners();

	void Move(glm::vec2 newPos);
	void Translate(glm::vec2 trans, float deltaTime);
	void Flip(bool flip);
	void SetRotation(float newRotation);

	template<typename T>
	void AddComponent(const std::string& compName, T& newComponent) {
		m_Components.Add(compName, newComponent);
	}

	template<>
	void AddComponent<AnimationCycle>(const std::string& compName, AnimationCycle& newComponent) {
		m_Components.Add<AnimationCycle>(compName, newComponent);
	}

	template<>
	void AddComponent<Audio>(const std::string& compName, Audio& newAudio) {
		m_Components.Add<Audio>(compName, newAudio);
	}

	template<typename T>
	std::unique_ptr<T>& GetComponent(const std::string& name) {
		return m_Components.Get(name);
	}

	template<>
	std::unique_ptr<AnimationCycle>& GetComponent(const std::string& name) {
		return m_Components.Get<AnimationCycle>(name);
	}

	template<>
	std::unique_ptr<Audio>& GetComponent(const std::string& name) {
		return m_Components.Get<Audio>(name);
	}

	inline glm::vec2 getPos() const { return m_Pos; }
	inline glm::vec2 getSize() const { return m_Size; }
	inline float getRotation() const { return m_Rotation; }
	inline Hitbox* getHitbox() const { return m_Hitbox; }
	inline bool isFlipped() const { return m_Flipped; }

	std::string m_Tag;

protected:
	void MovePos(); //When hitbox moves, adjust the entity position
	void AttachCamera(Camera * camera);
	void DetachCamera();

	std::shared_ptr<Texture> m_Texture;
	SpriteSheetReader * m_SpriteSheet;

	Hitbox * m_Hitbox;
	glm::vec2 m_HitboxOffset;

	Camera * m_Camera;

	glm::vec2 m_Pos, m_Size;

	//This is an anti-pattern but it actually works as I need to update the hitbox rotation when this does as well
	float m_Rotation; 

	bool m_Destroyed;
	bool m_Flipped;

	Component m_Components;

	friend class Hitbox;
	friend class Camera;
};
