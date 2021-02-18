#include "Slider.h"

Slider::Slider(std::shared_ptr<Texture> slideTexture, glm::vec2 pos, glm::vec2 size, float rotation, float initPercentage)
	:m_Texture(slideTexture), m_Pos(pos), m_Size(size), m_Rotation(rotation)
{
	if (initPercentage <= 100.0f && initPercentage >= 0.0f) {
		m_Percent = initPercentage;
	}
	else {
		std::cout << "Error: Percentage inputted is larger than 100 or lower than 0" << std::endl;
		initPercentage = 100.0f;
	}

	m_Width = m_Texture->getWidth();
	m_Height = m_Texture->getHeight();

	//Easier to read this varible rather than declaring it again everytime I draw inside the function argument
	m_DefaultTexUVs = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f
	};
}

void Slider::Draw(SpriteRenderer& spriteRenderer, glm::vec3 color, bool flip, bool followCamera) {
	float rightX = m_Percent / 100.0f;
	std::vector<float> posUVs = {
		0.0f,   1.0f,
		rightX, 0.0f,
		0.0f,   0.0f,
		rightX, 1.0f
	};

	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, flip, followCamera, m_Rotation, glm::vec4(color, 1.0f), m_DefaultTexUVs, posUVs);
}

void Slider::SetPercentage(float newPercentage) {
	if (newPercentage <= 100.0f && newPercentage >= 0.0f) {
		m_Percent = newPercentage;
	}
	else {
		std::cout << "Error: Percentage inputted is larger than 100 or lower than 0" << std::endl;
	}
}

void Slider::AddPercentage(float addAmount) {
	m_Percent += addAmount;

	if (m_Percent > 100.0f) {
		m_Percent = 100.0f;
	}
	else if (m_Percent < 0.0f) {
		m_Percent = 0.0f;
	}
}