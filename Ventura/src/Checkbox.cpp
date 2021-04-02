#include "Checkbox.h"

Checkbox::Checkbox(unsigned int windowWidth, unsigned int windowHeight, bool initalCheck, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 textOffset, std::string text, unsigned int fontSize, std::string customFont, glm::vec2 hitboxOffset, glm::vec2 hitboxSize) 
	:m_Checked(initalCheck), m_Rotation(rotation), m_Pos(pos), m_Size(size), m_HeaderText(text), m_TextOffset(textOffset), m_HitboxOffset(hitboxOffset), m_CheckDelegateAdded(false), m_UncheckDelegateAdded(false)
{
	m_UncheckedTexture = ResourceManager::Get<Texture>("checkbox");
	m_CheckedTexture = ResourceManager::Get<Texture>("checkboxTicked");

	m_TextOffset = (textOffset == glm::vec2(0.0f)) ? glm::vec2((220.0f * m_Size.x) / 200.0f, (90.0f * m_Size.y) / 200.0f) : textOffset;
	fontSize = (fontSize == 0) ? (32 * m_Size.y) / 200.0f : fontSize;

	m_Text = new TextRenderer(windowWidth, windowHeight, customFont, fontSize);
	m_Text->m_Rotation = m_Rotation;

	glm::vec2 hitSize = (hitboxSize == glm::vec2(0.0f)) ? m_Size : hitboxSize;
	m_Hitbox = new Hitbox(m_Pos + hitboxOffset, hitSize, glm::vec2(0.0f), m_Rotation);
}

Checkbox::Checkbox(unsigned int windowWidth, unsigned int windowHeight, bool initalCheck, std::shared_ptr<Texture> uncheckedTexture, std::shared_ptr<Texture> checkedTexture, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 textOffset, std::string text, unsigned int fontSize, std::string customFont, glm::vec2 hitboxOffset, glm::vec2 hitboxSize) 
	:m_Checked(initalCheck), m_UncheckedTexture(uncheckedTexture), m_CheckedTexture(checkedTexture), m_Rotation(rotation), m_Pos(pos), m_Size(size), m_HeaderText(text), m_TextOffset(textOffset), m_HitboxOffset(hitboxOffset), m_CheckDelegateAdded(false), m_UncheckDelegateAdded(false)
{
	m_Text = new TextRenderer(windowWidth, windowHeight, customFont, fontSize);
	m_Text->m_Rotation = m_Rotation;

	m_TextOffset = (textOffset == glm::vec2(0.0f)) ? glm::vec2((220.0f * m_Size.x) / 200.0f, (90.0f * m_Size.y) / 200.0f) : textOffset;
	fontSize = (fontSize == 0) ? (32 * m_Size.y) / 200.0f : fontSize;

	glm::vec2 hitSize = (hitboxSize == glm::vec2(0.0f)) ? m_Size : hitboxSize;
	m_Hitbox = new Hitbox(m_Pos + hitboxOffset, hitSize, glm::vec2(0.0f), m_Rotation);
}

Checkbox::~Checkbox() {
	delete m_Text;
	m_Text = nullptr;

	delete m_Hitbox;
	m_Hitbox = nullptr;
}

void Checkbox::CheckClicked(bool buttonClicked, int& buttonAllowment, glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos) {
	if (buttonClicked && buttonAllowment == 1) {
		if (CollisionHandler::CollidePoint(mousePos, *m_Hitbox, followingCamera, cameraPos)) {
			m_Checked = !m_Checked;

			//Check to see if a delegate has been added and call it if it has.
			if (m_Checked && m_CheckDelegateAdded) {
				m_CheckDelegate();
			}
			else if (!m_Checked && m_UncheckDelegateAdded) {
				m_UncheckDelegate();
			}

			buttonAllowment = 0;
		}
	}
}

void Checkbox::Draw(SpriteRenderer& spriteRenderer, bool drawHitbox, bool flipped, bool followCamera, glm::vec4 checkboxColor, glm::vec3 hitboxColor, glm::vec3 textColor) {
	spriteRenderer.DrawSprite((m_Checked) ? *m_CheckedTexture : *m_UncheckedTexture, m_Pos, m_Size, flipped, followCamera, m_Rotation, checkboxColor);

	if (m_HeaderText != "") {
		m_Text->Text(m_HeaderText, m_Pos.x + m_TextOffset.x, m_Pos.y + m_TextOffset.y, 1.0f, textColor, followCamera);
	}

	if (drawHitbox) {
		m_Hitbox->Draw(hitboxColor, followCamera);
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
	m_Text->LoadFont((fontPath == "") ? m_Text->getCurrentFont() : fontPath, fontSize);
}

void Checkbox::SetCheckDelegate(const std::function<void()>& func) {
	m_CheckDelegate = func;
	m_CheckDelegateAdded = true;
}

void Checkbox::SetUncheckDelegate(const std::function<void()>& func) {
	m_UncheckDelegate = func;
	m_UncheckDelegateAdded = true;
}