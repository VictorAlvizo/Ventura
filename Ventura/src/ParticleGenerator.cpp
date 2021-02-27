#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(std::shared_ptr<Texture> texture, glm::vec2 initialPos, glm::vec2 velocity, glm::ivec4 posRange, glm::ivec2 sizeRange, glm::vec3 color, bool rotate, int maxParticles, float life) 
	:m_Texture(texture), m_SpawnPos(initialPos), m_PosRange(posRange), m_SizeRange(sizeRange), m_ShouldRotate(rotate), m_MaxAmount(maxParticles), m_MaxLife(life)
{
	std::srand(time(NULL));

	for (int i = 0; i < maxParticles; i++) {
		m_Particles.push_back(Particle());
	}

	m_VelocityList.push_back(velocity);
	m_Colors.push_back(color);

	m_BurstEnabled = false;
	m_CooldownActive = false;
	m_MaxDuration = 0.0f;
	m_Duration = 0.0f;
	m_MaxCoolDown = 0.0f;
	m_CoolDown = 0.0f;
}

void ParticleGenerator::Draw(SpriteRenderer& spriteRenderer) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Gives the particles a nice glow effect

	for (Particle& particle : m_Particles) {
		if (particle.m_Life > 0.0f) {
			spriteRenderer.DrawSprite(*m_Texture, particle.m_Pos, particle.m_Size, false, true, particle.m_Rotation, particle.m_Color);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Update(float deltaTime, int reviveAmount, bool diminishColor, bool spawnMore) {
	//Logic for burst mode
	if (m_BurstEnabled) {
		if (m_CooldownActive) {
			m_CoolDown -= deltaTime;

			if (m_CoolDown <= 0.0f) {
				m_CooldownActive = false;
				m_Duration = m_MaxDuration;
			}
		}
		else {
			m_Duration -= deltaTime;

			if(m_Duration <= 0.0f){
				m_CooldownActive = true;
				m_CoolDown = m_MaxCoolDown;
			}
		}
	}

	if (spawnMore && (!m_BurstEnabled || (m_BurstEnabled && m_Duration >= 0.0f))) {
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
				particle.m_Color.a = particle.m_Life / m_MaxLife; //Color based on % of life left
			}

			particle.m_Pos -= particle.m_Velocity * deltaTime;
		}
	}
}

void ParticleGenerator::BurstMode(bool activate, float duration, float coolDown) {
	m_BurstEnabled = activate;
	m_CooldownActive = false;
	m_MaxDuration = duration;
	m_Duration = m_MaxDuration; //Just so it starts with a burst ;)
	m_MaxCoolDown = coolDown;
}

void ParticleGenerator::AppendVelocityList(std::vector<glm::vec2> velocityList) {
	m_VelocityList.insert(m_VelocityList.end(), velocityList.begin(), velocityList.end());
}

bool ParticleGenerator::RemoveVelocity(glm::vec2 velocity) {
	if (m_VelocityList.size() == 1) {
		std::cout << "Error: Cannot remove as there is only 1 velocity in the list" << std::endl;
		return false;
	}

	bool found = false;
	for (unsigned int i = 0; i < m_VelocityList.size(); i++) {
		if (m_VelocityList[i] == velocity) {
			m_VelocityList.erase(m_VelocityList.begin() + i);
			found = true;
			break;
		}
	}

	if (!found) {
		std::cout << "Error: Velocity (" << velocity.x << "," << velocity.y << ") was not found" << std::endl;
	}

	return found;
}

void ParticleGenerator::ClearVelocityList() {
	m_VelocityList.erase(m_VelocityList.begin() + 1, m_VelocityList.end());
}

void ParticleGenerator::AppendColors(std::vector<glm::vec3> colors) {
	m_Colors.insert(m_Colors.end(), colors.begin(), colors.end());
}

bool ParticleGenerator::RemoveColor(glm::vec3 color) {
	if (m_Colors.size() == 1) {
		std::cout << "Error: Cannot remove as there is only 1 color in the list" << std::endl;
		return false;
	}

	bool found = false;
	for (unsigned int i = 0; i < m_Colors.size(); i++) {
		if (m_Colors[i] == color) {
			m_Colors.erase(m_Colors.begin() + i);
			found = true;
			break;
		}
	}

	if (!found) {
		std::cout << "Error: Color (" << color.x << "," << color.y << "," << color.z << ") was not found" << std::endl;
	}

	return found;
}

void ParticleGenerator::ClearColors() {
	m_Colors.erase(m_Colors.begin() + 1, m_Colors.end());
}

void ParticleGenerator::SpawnParticle(Particle& particle) {
	//Want to adjust whether it's adding or subtracing based if the min value is negative or positive
	float xPos = (m_PosRange.x >= 0.0f) ? m_PosRange.x + rand() % (m_PosRange.y + 1 - m_PosRange.x) : m_PosRange.x + rand() % (m_PosRange.y + 1 + m_PosRange.x);
	float yPos = (m_PosRange.z >= 0.0f) ? m_PosRange.z + rand() % (m_PosRange.w + 1 - m_PosRange.z) : m_PosRange.z + rand() % (m_PosRange.w + 1 + m_PosRange.z);

	particle.m_Pos = m_SpawnPos + glm::vec2(xPos, yPos);

	particle.m_Size = glm::vec2(m_SizeRange.x + rand() % (m_SizeRange.x + 1 - m_SizeRange.y));
	particle.m_Velocity = m_VelocityList[rand() % m_VelocityList.size()];
	particle.m_Color = glm::vec4(m_Colors[rand() % m_Colors.size()], 1.0f);
	particle.m_Life = m_MaxLife;
	particle.m_Rotation = (m_ShouldRotate) ? rand() % 361 : 0; //0 - 360 degrees
}