#pragma once
#include <stdlib.h>
#include <time.h>
#include "SpriteRenderer.h"

struct Particle {
	glm::vec2 m_Pos, m_Size, m_Velocity;
	glm::vec4 m_Color;
	float m_Life;

	Particle() :m_Pos(0.0f), m_Size(0.0f), m_Velocity(0.0f), m_Color(1.0f), m_Life(0.0f) { }
};

class ParticleGenerator {
public:
	ParticleGenerator(Texture& texture, glm::vec2 velocity, glm::ivec4 posRange, glm::ivec2 sizeRange, glm::vec3 color = glm::vec3(1.0f), int maxParticles = 500, float life = 1.0f);

	void Draw(SpriteRenderer& spriteRenderer);
	void Update(float deltaTime, int reviveAmount = 2, bool diminishColor = true);

	glm::vec2 m_SpawnPos;
		
private:
	void SpawnParticle(Particle& particle);

	std::vector<Particle> m_Particles;

	Texture m_Texture;

	glm::ivec4 m_PosRange; //First vec2 is for the range in the x and the next vec2 is for the range in the y
	glm::ivec2 m_SizeRange;
	glm::vec2 m_Velocity;
	glm::vec3 m_Color;

	int m_MaxAmount;
	float m_MaxLife;
	int m_CurrentIndex;
};