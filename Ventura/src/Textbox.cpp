#include "Textbox.h"

Textbox::Textbox(unsigned int winWidth, unsigned int winHeight, glm::vec2 pos, glm::vec2 size, float rotation, std::string defaultText, glm::vec2 textOffset, unsigned int fontSize, std::string customFont, glm::vec2 hitboxOffset, glm::vec2 hitboxSize) 
	:m_Pos(pos), m_Size(size), m_Rotation(rotation), m_HitboxOffset(hitboxOffset), m_Text(defaultText), m_Active(false), m_ShowBlink(false)
{
	m_Texture = ResourceManager::Get<Texture>("textbox");
	m_TextCursor = ResourceManager::Get<Texture>("textcursor");

	m_TextOffset = (textOffset == glm::vec2(0.0f)) ? glm::vec2((m_Size.x * 10.0f) / 300.0f, (m_Size.y * 40.0f) / 100.0f) : textOffset;
	float fontSiz = (fontSize == 0) ? (m_Size.y * 35) / 100.0f : fontSize;

	m_TextRenderer = new TextRenderer(winWidth, winHeight, customFont, fontSiz);
	m_TextRenderer->m_Rotation = this->m_Rotation;

	glm::vec2 hitSize = (hitboxSize == glm::vec2(0.0f)) ? m_Size : hitboxSize;
	m_Hitbox = new Hitbox(m_Pos + hitboxOffset, hitSize, glm::vec2(0.0f), m_Rotation);

	m_CursorTimer = new Timer(450);
	m_CursorTimer->StartTimer([=]() {m_ShowBlink = !m_ShowBlink; });
}

Textbox::~Textbox() {
	delete m_TextRenderer;
	m_TextRenderer = nullptr;

	delete m_Hitbox;
	m_Hitbox = nullptr;

	delete m_CursorTimer;
	m_CursorTimer = nullptr;
}

void Textbox::Draw(SpriteRenderer& spriteRenderer, bool drawHitbox, bool followCamera, float outlineWidth, glm::vec4 textboxColor, glm::vec3 hitboxColor, glm::vec3 textColor, glm::vec3 outlineColor) {
	if (m_Active) {
		//8 and 15 are used for the proportion equation
		spriteRenderer.DrawSprite(*m_Texture, m_Pos - (8.0f * outlineWidth) / 15.0f, m_Size + outlineWidth, false, followCamera, m_Rotation, glm::vec4(outlineColor, 1.0f));
	}

	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, false, followCamera, m_Rotation, textboxColor);
	m_TextRenderer->Text(m_Text, m_Pos.x + m_TextOffset.x, m_Pos.y + m_TextOffset.y, 1.0, textColor, followCamera, 1.0f);

	if (m_ShowBlink && m_Active) {
		//TODO: Have to find a way to place the cursor into the right x position
		spriteRenderer.DrawSprite(*m_TextCursor, glm::vec2(m_Pos.x + m_TextOffset.x, m_Pos.y + (m_Size.y * 15.0f) / 100.0f), glm::vec2(1.0f, (m_Size.y * 70.0f) / 100.0f), false, true, 0.0f, glm::vec4(glm::vec3(0.0f), 1.0f));
	}

	if (drawHitbox) {
		m_Hitbox->Draw(hitboxColor, followCamera);
	}
}

void Textbox::Move(glm::vec2 newPos) {
	m_Pos = newPos;
	m_Hitbox->Move(newPos + m_HitboxOffset);
}

void Textbox::SetRotation(float rotation) {
	m_Rotation = rotation;
	m_Hitbox->m_Rotation = rotation;
	m_TextRenderer->m_Rotation = rotation;
}

void Textbox::CheckClicked(bool buttonClicked, int& buttonAllowment, glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos) {
	if (buttonClicked && buttonAllowment == 1) {
		if (CollisionHandler::CollidePoint(mousePos, *m_Hitbox, followingCamera, cameraPos)) {
			m_Active = !m_Active;
			buttonAllowment = 0;
		}
	}
}

void Textbox::DetectKeys(const bool * keys, int * keyAllowment, bool shouldHold) {
	//TODO: Check if shift or caps is on to uppercase the letters. Shift in general for checking
	//if it's an ! or a 1 for example
	if (m_Active) {
		//Normal keys
		for (int i = 32; i < 97; i++) {
			if (keys[i] && keyAllowment[i] == 1) {
				m_Text += static_cast<char>(i);
				if (!shouldHold) {
					keyAllowment[i] = 0;
				}
			}
		}

		//TODO: More specialized keys
	}
}