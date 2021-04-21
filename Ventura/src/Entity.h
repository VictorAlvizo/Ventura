#pragma once
#include "Hitbox.h"
#include "SpriteSheetReader.h"
#include "Component.h"
#include "ResourceManager.h"

class Camera;

class Entity {
public:
	//Should not be used if you want to use the entity, won't initlize the texture and other essential varibles
	Entity();
	//Entity concstructer for a static image
	Entity(std::shared_ptr<Texture>& texture, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, glm::vec2 hbPos = glm::vec2(0.0f), glm::vec2 hbSize = glm::vec2(0.0f), float mass = 1.0f, bool childClass = false);
	//Only if you want animated sprites / use of a spritesheet. Provide the dimensions of 1 sprite
	Entity(std::shared_ptr<Texture>& texture, float spriteX, float spriteY, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, glm::vec2 hbPos = glm::vec2(0.0f), glm::vec2 hbSize = glm::vec2(0.0f), float mass = 1.0f, bool childClass = false);
	//Custom copy constructer for the entity class
	Entity(const Entity& copy);
	~Entity();

	//Use if the entity uses a static image and not an animation
	virtual void Draw(SpriteRenderer& spriteRenderer, glm::vec4 color = glm::vec4(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f));
	//Use if the entity uses an animation. Provide the spritepos through the AnimationCycle object associsated with it
	virtual void Draw(SpriteRenderer& spriteRenderer, glm::ivec2 spritePos, glm::vec4 color = glm::vec4(1.0f), glm::vec3 hbColor = glm::vec3(0.0f, 1.0f, 0.0f));

	//Returns each of the corners of the entity, override if the entity is not of a rectangle shape, affects hitbox
	virtual std::vector<glm::vec2> GetCorners();

	//Move the entity to a position
	void Move(glm::vec2 newPos);
	//Gradually move the entity in accordance to its velocity
	void Translate(float deltaTime);
	//Call this method if you want the entity to be affected by gravity
	void GravityMovement(float gravity, float deltaTime);
	//Flip the sprite of this entity
	void Flip(bool flip);
	//Set the rotation of the entity
	void SetRotation(float newRotation);

	//Add a componenet of an unknown type to the entity
	template<typename T>
	void AddComponent(const std::string& compName, T& newComponent) {
		m_Components.Add(compName, newComponent);
	}

	//Add an Animationcycle component
	template<>
	void AddComponent<AnimationCycle>(const std::string& compName, AnimationCycle& newAnimationCycle) {
		m_Components.Add<AnimationCycle>(compName, newAnimationCycle);
	}

	//Add an Audio component
	template<>
	void AddComponent<Audio>(const std::string& compName, Audio& newAudio) {
		m_Components.Add<Audio>(compName, newAudio);
	}

	//Add a ParticleGenerator component
	template<>
	void AddComponent<ParticleGenerator>(const std::string& compName, ParticleGenerator& newParticleGen) {
		m_Components.Add<ParticleGenerator>(compName, newParticleGen);
	}

	//Retrive an unknown type of components
	template<typename T>
	std::shared_ptr<T>& GetComponent(const std::string& name) {
		return m_Components.Get(name);
	}

	//Retrive the pointer of an AnimationCycle component with the name
	template<>
	std::shared_ptr<AnimationCycle>& GetComponent(const std::string& name) {
		return m_Components.Get<AnimationCycle>(name);
	}

	//Retrive the pointer of an Audio componenet with the name
	template<>
	std::shared_ptr<Audio>& GetComponent(const std::string& name) {
		return m_Components.Get<Audio>(name);
	}

	//Retrive the pointer of a particle generator with the name
	template<>
	std::shared_ptr<ParticleGenerator>& GetComponent(const std::string& name) {
		return m_Components.Get<ParticleGenerator>(name);
	}
	
	//Returns the position  of the entity
	inline glm::vec2 getPos() const { return m_Pos; }
	//Returns the size of the entity
	inline glm::vec2 getSize() const { return m_Size; }
	//Returns the rotation of the entity
	inline float getRotation() const { return m_Rotation; }
	//Returns the mass of the entity
	inline float getMass() const { return m_Mass; }
	//Returns the hitbox pointer object of the entity
	inline Hitbox* getHitbox() const { return m_Hitbox; }
	//Returns if the entity is flipped or not
	inline bool isFlipped() const { return m_Flipped; }

	std::string m_Tag;
	glm::vec2 m_Velocity;
	bool m_Destroyed;

	//Operator overloading the assigment operator to copy it correctly
	void operator=(const Entity& copy); //Same code as copy constructer
	
	//When storing the entity data
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

	//When retriving the entity data
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