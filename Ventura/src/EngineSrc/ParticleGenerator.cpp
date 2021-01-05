#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Texture& texture, glm::vec2 velocity, glm::ivec4 posRange, glm::ivec2 sizeRange, glm::vec3 color, bool rotate, int maxParticles, float life) 
	:m_Texture(texture), m_PosRange(posRange), m_SizeRange(sizeRange), m_ShouldRotate(rotate), m_MaxAmount(maxParticles), m_MaxLife(life)
{
	std::srand(time(NULL));

	for (int i = 0; i < maxParticles; i++) {
		m_Particles.push_back(Particle());
	}

	m_VelocityList.push_back(velocity);
	m_Colors.push_back(color);
}

void ParticleGenerator::Draw(SpriteRenderer& spriteRenderer) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Gives the particles a nice glow effect

	for (Particle& particle : m_Particles) {
		if (particle.m_Life > 0.0f) {
			spriteRenderer.DrawSprite(m_Texture, particle.m_Pos, particle.m_Size, false, particle.m_Rotation, particle.m_Color);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Update(float deltaTime, int reviveAmount, bool diminishColor, bool spawnMore) {
	if (spawnMore) {
		for (unsigned int i = 0; i < reviveAmount; i++) {
			if (m_CurrentIndex + (i + 1) >= m_Particles.size()) {
				m_CurrentIndex = 0;
			}

			m_CurrentIndex++;
			SpawnParticle(m_Particles[m_CurrentIndex + i]);
		}
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

void ParticleGenerator::AppendVelocityList(std::vector<glm::vec2> velocityList) {
	m_VelocityList.insert(m_VelocityList.end(), velocityList.begin(), velocityList.end());
}

void ParticleGenerator::RemoveVelocity(glm::vec2 velocity) {
	bool found = false;
	for (unsigned int i = 0; i < m_VelocityList.size(); i++) {
		if (m_VelocityList[i] == velocity) {
			m_VelocityList.erase(m_VelocityList.begin() + i);
			found = true;
			break;
		}
	}

	if (!found) {
		std::cout << "Error: Velocity " << "(" << velocity.x << "," << velocity.y << ")" << " was not found" << std::endl;
	}
}

void ParticleGenerator::ClearVelocityList() {
	m_VelocityList.erase(m_VelocityList.begin() + 1, m_VelocityList.end());
}

void ParticleGenerator::AppendColors(std::vector<glm::vec3> colors) {
	m_Colors.insert(m_Colors.end(), colors.begin(), colors.end());
}

void ParticleGenerator::RemoveColor(glm::vec3 color) {
	bool found = false;
	for (unsigned int i = 0; i < m_Colors.size(); i++) {
		if (m_Colors[i] == color) {
			m_Colors.erase(m_Colors.begin() + i);
			found = true;
			break;
		}
	}

	if (!found) {
		std::cout << "Error: Color " << "(" << color.x << "," << color.y << "," << color.z << ")" << " was not found" << std::endl;
	}
}

void ParticleGenerator::ClearColors() {
	m_Colors.erase(m_Colors.begin() + 1, m_Colors.end());
}

void ParticleGenerator::SpawnParticle(Particle& particle) {
	float xPos = m_PosRange.x + rand() % (m_PosRange.y + 1 - m_PosRange.x);
	float yPos = m_PosRange.z + rand() % (m_PosRange.w + 1 - m_PosRange.z);
	particle.m_Pos = m_SpawnPos + glm::vec2(xPos, yPos);

	particle.m_Size = glm::vec2(m_SizeRange.x + rand() % (m_SizeRange.x + 1 - m_SizeRange.y));
	particle.m_Velocity = m_VelocityList[rand() % m_VelocityList.size()];
	particle.m_Color = glm::vec4(m_Colors[rand() % m_Colors.size()], 1.0f);
	particle.m_Life = m_MaxLife;
	particle.m_Rotation = (m_ShouldRotate) ? rand() % 361 : 0;
}