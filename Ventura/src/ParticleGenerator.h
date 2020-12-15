#pragma once
#include "SpriteRenderer.h"

struct Particle {
	glm::vec2 m_Pos, m_Size, m_Velocity;
	glm::vec4 m_Color;
	float m_Life;

	Particle() :m_Pos(0.0f), m_Size(0.0f), m_Velocity(0.0f), m_Color(1.0f), m_Life(0.0f) { }
};

class ParticleGenerator {
public:
	ParticleGenerator(Texture& texture, glm::vec2 velocity, glm::vec4 posRange, glm::vec2 sizeRange, int maxParticles = 500, float life = 1.0f);

	//TODO: Do I want an update loop or do I only want to update information everytime I draw?
	void Draw(SpriteRenderer& spriteRenderer);

private:
	std::vector<Particle> m_Particles;

	Texture m_Texture;

	glm::vec4 m_PosRange; //First vec2 is for the range in the x and the next vec2 is for the range in the y
	glm::vec2 m_SizeRange, m_Velocity;

	int m_MaxAmount;
	float m_MaxLife;
};