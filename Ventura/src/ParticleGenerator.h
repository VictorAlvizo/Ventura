#pragma once
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "SpriteRenderer.h"

struct Particle {
	glm::vec2 m_Pos, m_Size, m_Velocity;
	glm::vec4 m_Color;
	float m_Life;
	float m_Rotation;

	Particle() :m_Pos(0.0f), m_Size(0.0f), m_Velocity(0.0f), m_Color(1.0f), m_Life(0.0f), m_Rotation(0.0f) { }
};

class ParticleGenerator {
public:
	//Constructer for the particle generator, default particle (1 single particle) lives for 1 second
	ParticleGenerator(std::shared_ptr<Texture> texture, glm::vec2 initialPos, glm::vec2 velocity, glm::ivec4 posRange, glm::ivec2 sizeRange, glm::vec3 color = glm::vec3(1.0f), bool rotate = false, int maxParticles = 500, float life = 1.0f);
	~ParticleGenerator();

	//Draw the particles
	void Draw(SpriteRenderer& spriteRenderer, bool followCamera = true);
	//Call this every frame, determine how many new particles should be spawned and if
	//the particle color should diminish as its life passes. Making spawnmore as false will forcefully stop it
	void Update(float deltaTime, int reviveAmount = 2, bool diminishColor = true, bool spawnMore = true);

	//If burst mode is true, will spawn new particles every interval for a set duration
	void BurstMode(bool activate, float duration = 2.0f, float coolDown = 2.0f);

	//Add more velocities
	void AppendVelocityList(std::vector<glm::vec2> velocityList);
	//Remove a certain velocity
	bool RemoveVelocity(glm::vec2 velocity);
	//Clears all the velocites except for the inital one
	void ClearVelocityList();

	//Add more colors
	void AppendColors(std::vector<glm::vec3> colors);
	//Remove a certain color
	bool RemoveColor(glm::vec3 color);
	//Clears all the colors except for the inital one
	void ClearColors();

	//Returns true if the particles are rotating
	inline bool rotatingEnabled() const { return m_ShouldRotate; }
	//Returns true if burst mode is active
	inline bool isBurstActive() const { return m_BurstEnabled; }

	glm::vec2 m_SpawnPos;
		
private:
	void SpawnParticle(Particle& particle);

	std::vector<Particle> m_Particles;

	std::shared_ptr<Texture> m_Texture;

	glm::ivec4 m_PosRange; //First vec2 is for the range in the x and the next vec2 is for the range in the y
	glm::ivec2 m_SizeRange;
	
	std::vector<glm::vec3> m_Colors;
	std::vector<glm::vec2> m_VelocityList;

	int m_MaxAmount;
	int m_CurrentIndex;
	float m_MaxLife;
	bool m_ShouldRotate;

	//Varibles regarding burst mode
	bool m_BurstEnabled, m_CooldownActive;
	float m_MaxDuration, m_Duration;
	float m_MaxCoolDown, m_CoolDown;
};