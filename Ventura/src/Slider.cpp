#include "Slider.h"

Slider::Slider(std::shared_ptr<Texture> slideTexture, glm::vec2 pos, glm::vec2 size, float rotation, float initPercentage, glm::vec2 hitboxOffset, glm::vec2 hitboxSize)
	:m_Texture(slideTexture), m_Pos(pos), m_Size(size), m_Rotation(rotation), m_HitboxOffset(hitboxOffset), m_DelegateAdded(false)
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

	glm::vec2 hitSize = (hitboxSize == glm::vec2(0.0f)) ? m_Size : hitboxSize;
	m_Hitbox = new Hitbox(m_Pos + m_HitboxOffset, hitSize);
}

Slider::~Slider() {
	delete m_Hitbox;
	m_Hitbox = nullptr;
}

void Slider::CheckClicked(bool buttonClicked, glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos, bool percentWithClick) {
	if (buttonClicked) {
		if (CollisionHandler::CollidePoint(mousePos, *m_Hitbox, followingCamera, cameraPos)) {
			if (percentWithClick) {
				if (!followingCamera) {
					m_Percent = ((mousePos.x - m_Pos.x) * 100) / m_Size.x;
				}
				else {
					m_Percent = ((cameraPos.x + mousePos.x - m_Pos.x) * 100) / m_Size.x;
				}
			}

			if (m_DelegateAdded) {
				m_Delegate(m_Percent);
			}
		}
	}
}

void Slider::Draw(SpriteRenderer& spriteRenderer, bool showHitbox, bool flip, glm::vec3 foregroundColor, glm::vec3 color, glm::vec3 hitboxColor, bool followCamera) {
	float rightX = m_Percent / 100.0f;
	std::vector<float> posUVs;

	if (flip) {
		posUVs = {
			rightX, 1.0f,
			1.0f,   0.0f,
			rightX, 0.0f,
			1.0f,   1.0f
		};
	}
	else {
		posUVs = {
			0.0f,   1.0f,
			rightX, 0.0f,
			0.0f,   0.0f,
			rightX, 1.0f
		};
	}

	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, flip, followCamera, m_Rotation, glm::vec4(foregroundColor, 1.0f));
	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, flip, followCamera, m_Rotation, glm::vec4(color, 1.0f), m_DefaultTexUVs, posUVs);

	if (showHitbox) {
		m_Hitbox->Draw(hitboxColor, followCamera);
	}
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

void Slider::Move(glm::vec2 newPos) {
	m_Pos = newPos;
	m_Hitbox->Move(m_Pos + m_HitboxOffset);
}

void Slider::Rotate(float newRotation) {
	m_Rotation = newRotation;
	m_Hitbox->m_Rotation = newRotation;
}

void Slider::SetDelegate(const std::function<void(float)>& func) {
	m_Delegate = func;
	m_DelegateAdded = true;
}