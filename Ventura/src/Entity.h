#pragma once
#include "Hitbox.h"
#include "SpriteSheetReader.h"
#include "Component.h"
#include "ResourceManager.h"

class Camera;

class Entity {
public:
	Entity();
	Entity(std::shared_ptr<Texture>& texture, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, glm::vec2 hbPos = glm::vec2(0.0f), glm::vec2 hbSize = glm::vec2(0.0f), float mass = 1.0f, bool childClass = false);
	//Only if you want animated sprites / use of a spritesheet. Provide the dimensions of 1 sprite
	Entity(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, glm::vec2 hbPos = glm::vec2(0.0f), glm::vec2 hbSize = glm::vec2(0.0f), float mass = 1.0f, bool childClass = false);
	Entity(const Entity& copy);
	~Entity();

	virtual void Draw(SpriteRenderer& spriteRenderer, glm::vec3 color = glm::vec3(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f));
	virtual void Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec3 color = glm::vec3(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f));

	virtual std::vector<glm::vec2> GetCorners();

	void Move(glm::vec2 newPos);
	void Translate(float deltaTime);
	void GravityMovement(float gravity, float deltaTime);
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
	std::shared_ptr<T>& GetComponent(const std::string& name) {
		return m_Components.Get(name);
	}

	template<>
	std::shared_ptr<AnimationCycle>& GetComponent(const std::string& name) {
		return m_Components.Get<AnimationCycle>(name);
	}

	template<>
	std::shared_ptr<Audio>& GetComponent(const std::string& name) {
		return m_Components.Get<Audio>(name);
	}

	inline glm::vec2 getPos() const { return m_Pos; }
	inline glm::vec2 getSize() const { return m_Size; }
	inline float getRotation() const { return m_Rotation; }
	inline float getMass() const { return m_Mass; }
	inline Hitbox* getHitbox() const { return m_Hitbox; }
	inline bool isFlipped() const { return m_Flipped; }

	std::string m_Tag;
	glm::vec2 m_Velocity;
	bool m_Destroyed;

	void operator=(const Entity& copy); //Same code as copy constructer

	friend std::ostream& operator<<(std::ostream& output, Entity& entity) {
		output << entity.m_Pos.x << " " << entity.m_Pos.y << " " << entity.m_Size.x << " " << entity.m_Size.y << " "
			<< entity.m_Velocity.x << " " << entity.m_Velocity.y << " " << entity.m_Destroyed << " "
			<< entity.m_Flipped << " " << entity.m_Tag << " " << entity.m_Rotation << " " << entity.m_Mass << " "
			<< entity.m_Texture->getPath() << " " << entity.m_HitboxOffset.x << " " << entity.m_HitboxOffset.y << " "
			<< entity.getHitbox()->getPos().x << " " << entity.getHitbox()->getPos().y << " "
			<< entity.getHitbox()->getSize().x << " " << entity.getHitbox()->getSize().y << " "
			<< entity.getHitbox()->getMass() << " " << entity.getHitbox()->m_Rotation << " ";

		if (entity.m_SpriteSheet) {
			output << entity.m_SpriteSheet->getSpriteSize().x << " " << entity.m_SpriteSheet->getSpriteSize().y;
		}
		else {
			output << 0 << " " << 0;
		}
		
		return output;
	}

	friend std::istream& operator>>(std::istream& input, Entity& entity) {
		std::string texturePath;
		glm::vec2 hitboxPos, hitboxSize, spriteSize;
		float hitboxMass, hitboxRotation;

		input >> entity.m_Pos.x >> entity.m_Pos.y >> entity.m_Size.x >> entity.m_Size.y >> entity.m_Velocity.x >> entity.m_Velocity.y >> entity.m_Destroyed
			>> entity.m_Flipped >> entity.m_Tag >> entity.m_Rotation >> entity.m_Mass >> texturePath >> entity.m_HitboxOffset.x
			>> entity.m_HitboxOffset.y >> hitboxPos.x >> hitboxPos.y >> hitboxSize.x >> hitboxSize.y >> hitboxMass >> hitboxRotation
			>> spriteSize.x >> spriteSize.y;

		entity.m_FromFile = true;
		entity.m_Texture = ResourceManager::GetTextureFromFileName(texturePath);
		entity.m_Hitbox = new Hitbox(hitboxPos, hitboxSize, entity.m_Velocity, hitboxRotation, hitboxMass, &entity);
		entity.m_SpriteSheet = (spriteSize != glm::vec2(0.0f)) ? new SpriteSheetReader(entity.m_Texture, spriteSize) : nullptr;

		return input;
	}

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
	float m_Mass; //For any physics calculations

	bool m_Flipped;
	//If true don't delete the pointers because it is a copy of another Entity
	//If it's an entity loaded from a file deem it as non-copy so it can delete its pointers
	bool m_Copied, m_FromFile;

	Component m_Components;

	friend class Hitbox;
	friend class Camera;
};