#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Texture& texture, glm::vec2 velocity, glm::vec4 posRange, glm::vec2 sizeRange, int maxParticles, float life) 
	:m_Texture(texture), m_Velocity(0.0f), m_PosRange(posRange), m_SizeRange(sizeRange), m_MaxAmount(maxParticles), m_MaxLife(life)
{
	for (int i = 0; i < maxParticles; i++) {
		m_Particles.push_back(Particle());
	}
}

void ParticleGenerator::Draw(SpriteRenderer& spriteRenderer) {
	//holder
}
