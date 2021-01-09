#include "Button.h"

Button::Button(unsigned int windowWidth, unsigned int windowHeight, Texture& buttonTexture, glm::vec2 pos, glm::vec2 size, std::string buttonText, float rotation, unsigned int fontSize, glm::vec3 fontColor, std::string customFont)
	:m_Pos(pos), m_Size(size), m_Rotation(rotation), m_ButtonText(buttonText), m_FontSize(fontSize), m_TextPath(customFont)
{
	for (int i = 0; i < 3; i++) {
		m_ButtonTextures[i] = buttonTexture;
	}

	m_Text = new TextRenderer(windowWidth, windowHeight);
	m_Text->LoadFont(customFont, m_FontSize);

	m_Hitbox = new Hitbox(m_Pos, m_Size, glm::vec2(0.0f), m_Rotation);

	m_CurrentStatus = Status::NORMAL;
}

Button::Button(unsigned int windowWidth, unsigned int windowHeight, Texture& buttonTexture, Texture& hoverTexture, Texture& clickTexture, glm::vec2 pos, glm::vec2 size, std::string buttonText, float rotation, unsigned int fontSize, glm::vec3 fontColor, std::string customFont)
	:m_Pos(pos), m_Size(size), m_Rotation(rotation), m_ButtonText(buttonText), m_FontSize(fontSize), m_TextPath(customFont)
{
	m_ButtonTextures[0] = buttonTexture;
	m_ButtonTextures[1] = hoverTexture;
	m_ButtonTextures[2] = clickTexture;

	m_Text = new TextRenderer(windowWidth, windowHeight);
	m_Text->LoadFont(customFont, m_FontSize);

	m_Hitbox = new Hitbox(m_Pos, m_Size, glm::vec2(0.0f), m_Rotation);

	m_CurrentStatus = Status::NORMAL;
}

Button::~Button() {
	delete m_Text;
	m_Text = nullptr;

	delete m_Hitbox;
	m_Hitbox = nullptr;
}

void Button::Draw(SpriteRenderer& spriteRenderer, bool drawHitbox, glm::vec4 buttonColor, glm::vec3 textColor, glm::vec3 hitboxColor, glm::vec2 textOffsets) {
	if (textOffsets == glm::vec2(0.0f)) {
		//Keep in mind, not perfect
		textOffsets.x = (m_Size.x / 2.0f) - (m_FontSize / 2);
		textOffsets.y = (m_Size.y / 2.0f) - (m_FontSize / 2);
	}

	spriteRenderer.DrawSprite(m_ButtonTextures[static_cast<int>(m_CurrentStatus)], m_Pos, m_Size, false, m_Rotation, buttonColor);
	m_Text->Text(m_ButtonText, m_Pos.x + textOffsets.x, m_Pos.y + textOffsets.y, 1, textColor);

	if (drawHitbox) {
		m_Hitbox->Draw(hitboxColor);
	}
}

void Button::SetPos(glm::vec2 newPos) {
	m_Pos = newPos;
	m_Hitbox->Move(m_Pos);
}

void Button::SetRotation(float newRotation) {
	m_Rotation = newRotation;
	m_Hitbox->m_Rotation = m_Rotation;

	//TODO: Add rotation here for text once I have that feature avalible
}

void Button::ChangeFont(unsigned int fontSize, std::string fontPath) {
	m_Text->LoadFont(fontPath, fontSize);
}


bool Button::isHovering(glm::vec2 mousePos) {
	if (CollisionHandler::CollidePoint(mousePos, *m_Hitbox)) {
		m_CurrentStatus = Status::HOVER;
		return true;
	}

	m_CurrentStatus = Status::NORMAL;
	return false;
}

bool Button::isClicked(glm::vec2 mousePos, bool mouseButton) {
	if (isHovering(mousePos) && mouseButton) {
		m_CurrentStatus = Status::CLICKED;
		return true;
	}

	m_CurrentStatus = Status::NORMAL;
	return false;
}
