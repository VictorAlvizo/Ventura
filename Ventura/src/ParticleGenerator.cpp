#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Texture& texture, glm::vec2 velocity, glm::ivec4 posRange, glm::ivec2 sizeRange, glm::vec3 color, int maxParticles, float life) 
	:m_Texture(texture), m_Velocity(velocity), m_PosRange(posRange), m_SizeRange(sizeRange), m_Color(color), m_MaxAmount(maxParticles), m_MaxLife(life)
{
	std::srand(time(NULL));

	for (int i = 0; i < maxParticles; i++) {
		m_Particles.push_back(Particle());
	}
}

void ParticleGenerator::Draw(SpriteRenderer& spriteRenderer) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Gives the particles a nice glow effect

	for (Particle& particle : m_Particles) {
		if (particle.m_Life > 0.0f) {
			spriteRenderer.DrawSprite(m_Texture, particle.m_Pos, particle.m_Size, false, 0.0f, particle.m_Color);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Update(float deltaTime, int reviveAmount, bool diminishColor) {
	//TODO: Find a way to reduce spawn rate. Even at 1 particles spawn like crazy
	for (unsigned int i = 0; i < reviveAmount; i++) {
		if (m_CurrentIndex + (i + 1) >= m_Particles.size()) {
			m_CurrentIndex = 0;
		}

		m_CurrentIndex++;
		SpawnParticle(m_Particles[m_CurrentIndex + i]);
	}

	for (Particle& particle : m_Particles) {
		particle.m_Life -= deltaTime;

		if (particle.m_Life > 0.0f) {
			if (diminishColor) {
				particle.m_Color.a = (particle.m_Color.a <= 0.0f) ? 0.0f : particle.m_Color.a - deltaTime;
			}

			particle.m_Pos -= particle.m_Velocity * deltaTime;
		}
	}
}

void ParticleGenerator::SpawnParticle(Particle& particle) {
	float xPos = m_PosRange.x + rand() % (m_PosRange.y + 1 - m_PosRange.x);
	float yPos = m_PosRange.z + rand() % (m_PosRange.w + 1 - m_PosRange.z);
	particle.m_Pos = m_SpawnPos + glm::vec2(xPos, yPos);

	particle.m_Size = glm::vec2(m_SizeRange.x + rand() % (m_SizeRange.x + 1 - m_SizeRange.y));
	//TODO: Color not working if not all 1's, really weird bug, seems to reset color in Spriterenderer with the w = 0, when trying to force it back dosen't work
	particle.m_Velocity = this->m_Velocity;
	particle.m_Color = glm::vec4(this->m_Color, 1.0f);
	particle.m_Life = m_MaxLife;
}