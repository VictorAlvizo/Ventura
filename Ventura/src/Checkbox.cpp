#include "Checkbox.h"

Checkbox::Checkbox(unsigned int windowWidth, unsigned int windowHeight, bool initalCheck, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 textOffset, std::string text, unsigned int fontSize, std::string customFont, glm::vec2 hitboxOffset, glm::vec2 hitboxSize) 
	:m_Checked(initalCheck), m_Rotation(rotation), m_Pos(pos), m_Size(size), m_HeaderText(text), m_TextOffset(textOffset), m_FontPath(customFont), m_HitboxOffset(hitboxOffset)
{
	m_UncheckedTexture = ResourceManager::Get<Texture>("checkbox");
	m_CheckedTexture = ResourceManager::Get<Texture>("checkboxTicked");

	m_Text = new TextRenderer(windowWidth, windowHeight, customFont, fontSize, false);
	m_Text->m_Rotation = m_Rotation;

	glm::vec2 hitSize = (hitboxSize == glm::vec2(0.0f)) ? m_Size : hitboxSize;
	m_Hitbox = new Hitbox(m_Pos + hitboxOffset, hitSize, glm::vec2(0.0f), m_Rotation);
}

Checkbox::~Checkbox() {
	delete m_Text;
	m_Text = nullptr;

	delete m_Hitbox;
	m_Hitbox = nullptr;
}

void Checkbox::checkClicked(bool buttonClicked, glm::vec2 mousePos) {
	if (buttonClicked) {
		if (CollisionHandler::CollidePoint(mousePos, *m_Hitbox)) {
			m_Checked = !m_Checked;
		}
	}
}

void Checkbox::Draw(SpriteRenderer& spriteRenderer, bool drawHitbox, bool flipped, glm::vec4 checkboxColor, glm::vec3 hitboxColor, glm::vec3 textColor) {
	spriteRenderer.DrawSprite((m_Checked) ? *m_CheckedTexture : *m_UncheckedTexture, m_Pos, m_Size, flipped, m_Rotation, checkboxColor);

	if (m_HeaderText != "") {
		m_Text->Text(m_HeaderText, m_Pos.x + m_TextOffset.x, m_Pos.y + m_TextOffset.y, 1.0f, textColor);
	}

	if (drawHitbox) {
		m_Hitbox->Draw(hitboxColor);
	}
}

void Checkbox::Move(glm::vec2 newPos) {
	m_Pos = newPos;
	m_Hitbox->Move(m_Pos + m_HitboxOffset);
}

void Checkbox::SetRotation(float newRotation) {
	m_Rotation = newRotation;
	m_Hitbox->m_Rotation = newRotation;
	m_Text->m_Rotation = newRotation;
}

void Checkbox::ChangeFont(unsigned int fontSize, std::string fontPath) {
	fontPath = (m_FontPath == "") ? fontPath : m_FontPath;
	m_Text->LoadFont(fontPath, fontSize);
}