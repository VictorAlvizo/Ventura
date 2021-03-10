#include "Textbox.h"

Textbox::Textbox(unsigned int winWidth, unsigned int winHeight, glm::vec2 pos, glm::vec2 size, std::string defaultText, std::string placeHolderText, glm::vec2 textOffset, unsigned int fontSize, std::string customFont, glm::vec2 hitboxOffset, glm::vec2 hitboxSize) 
	:m_Pos(pos), m_Size(size), m_HitboxOffset(hitboxOffset), m_Text(defaultText), m_PlaceholderText(placeHolderText), m_ShowText(defaultText), m_Active(false), m_ShowBlink(false), m_ActiveDelegateAdded(false), m_UnActiveDelegateAdded(false), m_HideIndex(0), m_CurrentKey(0), m_HoldEnabled(false)
{
	m_Texture = ResourceManager::Get<Texture>("textbox");
	m_TextCursor = ResourceManager::Get<Texture>("textcursor");

	m_TextOffset = (textOffset == glm::vec2(0.0f)) ? glm::vec2((m_Size.x * 10.0f) / 300.0f, (m_Size.y * 40.0f) / 100.0f) : textOffset;
	float fontSiz = (fontSize == 0) ? (m_Size.y * 35) / 100.0f : fontSize;

	m_TextRenderer = new TextRenderer(winWidth, winHeight, customFont, fontSiz);

	glm::vec2 hitSize = (hitboxSize == glm::vec2(0.0f)) ? m_Size : hitboxSize;
	m_Hitbox = new Hitbox(m_Pos + hitboxOffset, hitSize, glm::vec2(0.0f), 0.0f);

	m_CursorTimer = new Timer(450);
	m_CursorTimer->StartTimer([=]() {m_ShowBlink = !m_ShowBlink; });

	m_HoldTimer = new Timer(1000);
}

Textbox::Textbox(unsigned int winWidth, unsigned int winHeight, std::shared_ptr<Texture> textboxTexture, glm::vec2 pos, glm::vec2 size, std::string defaultText, std::string placeHolderText, glm::vec2 textOffset, unsigned int fontSize, std::string customFont, glm::vec2 hitboxOffset, glm::vec2 hitboxSize)
	:m_Texture(textboxTexture), m_Pos(pos), m_Size(size), m_HitboxOffset(hitboxOffset), m_Text(defaultText), m_PlaceholderText(placeHolderText), m_Active(false), m_ShowBlink(false), m_ActiveDelegateAdded(false), m_UnActiveDelegateAdded(false), m_HideIndex(0), m_CurrentKey(0), m_HoldEnabled(false)
{
	m_TextCursor = ResourceManager::Get<Texture>("textcursor");

	m_TextOffset = (textOffset == glm::vec2(0.0f)) ? glm::vec2((m_Size.x * 10.0f) / 300.0f, (m_Size.y * 40.0f) / 100.0f) : textOffset;
	float fontSiz = (fontSize == 0) ? (m_Size.y * 35) / 100.0f : fontSize;

	m_TextRenderer = new TextRenderer(winWidth, winHeight, customFont, fontSiz);

	glm::vec2 hitSize = (hitboxSize == glm::vec2(0.0f)) ? m_Size : hitboxSize;
	m_Hitbox = new Hitbox(m_Pos + hitboxOffset, hitSize, glm::vec2(0.0f), 0.0f);

	m_CursorTimer = new Timer(450);
	m_CursorTimer->StartTimer([=]() {m_ShowBlink = !m_ShowBlink; });

	m_HoldTimer = new Timer(1000);
}

Textbox::~Textbox() {
	delete m_TextRenderer;
	m_TextRenderer = nullptr;

	delete m_Hitbox;
	m_Hitbox = nullptr;

	delete m_CursorTimer;
	m_CursorTimer = nullptr;

	delete m_HoldTimer;
	m_HoldTimer = nullptr;
}

void Textbox::Draw(SpriteRenderer& spriteRenderer, bool drawHitbox, bool followCamera, float outlineWidth, glm::vec4 textboxColor, glm::vec3 hitboxColor, glm::vec3 textColor, glm::vec3 outlineColor, glm::vec3 cursorColor, float phOpacity, glm::vec3 phColor) {
	if (m_Active) {
		//8 and 15 are used for the proportion equation
		spriteRenderer.DrawSprite(*m_Texture, m_Pos - (8.0f * outlineWidth) / 15.0f, m_Size + outlineWidth, false, followCamera, 0.0f, glm::vec4(outlineColor, 1.0f));
	}

	spriteRenderer.DrawSprite(*m_Texture, m_Pos, m_Size, false, followCamera, 0.0f , textboxColor);
	m_TextRenderer->Text(m_ShowText, m_Pos.x + m_TextOffset.x, m_Pos.y + m_TextOffset.y, 1.0, textColor, followCamera, 1.0f);

	if (m_PlaceholderText != "" && m_Text == "") {
		m_TextRenderer->Text(m_PlaceholderText, m_Pos.x + m_TextOffset.x, m_Pos.y + m_TextOffset.y, 1.0, phColor, followCamera, phOpacity);
	}

	if (m_Pos.x + m_Size.x > m_TextRenderer->getInserationOffset()) {
		m_ShowText = m_Text.substr(m_HideIndex, m_Text.length());
	}
	else {
		m_ShowText = m_Text.substr(++m_HideIndex, m_Text.length());
	}

	if (m_ShowBlink && m_Active) {
		spriteRenderer.DrawSprite(*m_TextCursor, (m_Text != "") ? glm::vec2(m_TextRenderer->getInserationOffset(), m_Pos.y + (m_Size.y * 15.0f) / 100.0f) : m_Pos + m_TextOffset.x, glm::vec2(1.0f, (m_Size.y * 70.0f) / 100.0f), false, true, 0.0f, glm::vec4(cursorColor, 1.0f));
	}

	if (drawHitbox) {
		m_Hitbox->Draw(hitboxColor, followCamera);
	}
}

void Textbox::Move(glm::vec2 newPos) {
	m_Pos = newPos;
	m_Hitbox->Move(newPos + m_HitboxOffset);
}

void Textbox::ChangeFont(unsigned int fontSize, std::string fontPath) {
	m_TextRenderer->LoadFont((fontPath == "") ? m_TextRenderer->getCurrentFont() : fontPath, fontSize);
}

void Textbox::CheckClicked(bool buttonClicked, int& buttonAllowment, glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos) {
	if (buttonClicked && buttonAllowment == 1) {
		if (CollisionHandler::CollidePoint(mousePos, *m_Hitbox, followingCamera, cameraPos)) {
			m_Active = !m_Active;
			if (m_Active && m_ActiveDelegateAdded) {
				m_ActiveDelegate();
			}
			else if(m_UnActiveDelegateAdded) {
				m_UnactiveDelegate();
			}

			buttonAllowment = 0;
		}
	}
}

void Textbox::DetectKeys(const bool * keys, int * keyAllowment, bool capsLock) {
	if (m_Active) {
		//Backspace, deleting keys
		if (keys[259] && keyAllowment[259] == 1 && m_Text != "") {
			m_Text.pop_back();
			m_HideIndex = (m_HideIndex != 0) ? --m_HideIndex : m_HideIndex;
			keyAllowment[259] = 0;
		}

		for (int i = 32; i < 97; i++) {
			if (keys[i] && keyAllowment[i] == 1) {
				m_Text += ProcessKey(i, keys[340], keys[344], capsLock);
				keyAllowment[i] = 0;
			}
		}
	}
}

void Textbox::SetActiveDelegate(const std::function<void()>& func) {
	m_ActiveDelegate = func;
	m_ActiveDelegateAdded = true;
}

void Textbox::SetUnActiveDelegate(const std::function<void()>& func) {
	m_UnactiveDelegate = func;
	m_UnActiveDelegateAdded = true;
}

char Textbox::ProcessKey(int keyIndex, bool shiftLeft, bool capsLock, bool shiftRight) {
	int correctIndex = keyIndex;

	//Key versions WITHOUT shift active
	if (!shiftLeft && !shiftRight && !capsLock) {
		//Abc characters and some special keys (Refer to ASCII Table)
		if (keyIndex >= 65 && keyIndex <= 91) {
			correctIndex += 32;
		}
	}
	else {
		//Number keys
		if (keyIndex >= 48 && keyIndex <= 57) {
			if (keyIndex == 50) { // 2
				correctIndex += 14; // @
			}
			else if (keyIndex == 54) { // 6
				correctIndex += 40; // ^
			}
			else if (keyIndex == 55) { // 7
				correctIndex -= 17; // &
			}
			else if (keyIndex == 56) { // 8
				correctIndex -= 14; // *
			}
			else if (keyIndex == 57) { // 9
				correctIndex -= 17; // (
			}
			else if (keyIndex == 48) { // 0
				correctIndex -= 7; // )
			}
			else {
				correctIndex -= 16;
			}
		}else if (keyIndex == 96) { // `
			correctIndex += 30; // ~
		}
		else if (keyIndex == 45) { // -
			correctIndex += 50; // _
		}
		else if (keyIndex == 61) { // =
			correctIndex -= 18; // +
		}
		else if (keyIndex == 92) { // (\)
			correctIndex += 32; // |
		}
		else if (keyIndex == 39) { // '
			correctIndex -= 5; // "
		}
		else if (keyIndex == 59) { // ;
			correctIndex -= 1; // :
		}
		else if (keyIndex == 44) { // ,
			correctIndex += 16; // <
		}else if(keyIndex == 46){ // .
			correctIndex += 16; // >
		}
		else if (keyIndex == 47) { // /
			correctIndex += 16; // ?
		}
	}

	return static_cast<char>(correctIndex);
}