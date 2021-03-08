#pragma once
#include <functional>
#include "ResourceManager.h"
#include "TextRenderer.h"
#include "Hitbox.h"
#include "CollisionHandler.h"
#include "Timer.h"

class Textbox {
public:
	Textbox(unsigned int winWidth, unsigned int winHeight, glm::vec2 pos, glm::vec2 size, std::string defaultText = "", glm::vec2 textOffset = glm::vec2(0.0f), unsigned int fontSize = 0, std::string customFont = "Fonts/arial.ttf", glm::vec2 hitboxOffset = glm::vec2(0.0f), glm::vec2 hitboxSize = glm::vec2(0.0f));
	Textbox(unsigned int winWidth, unsigned int winHeight, std::shared_ptr<Texture> textboxTexture, std::shared_ptr<Texture> textCursorTexture, glm::vec2 pos, glm::vec2 size, std::string defaultText = "", glm::vec2 textOffset = glm::vec2(0.0f), unsigned int fontSize = 0, std::string customFont = "Fonts/arial.ttf", glm::vec2 hitboxOffset = glm::vec2(0.0f), glm::vec2 hitboxSize = glm::vec2(0.0f));
	~Textbox();

	void Draw(SpriteRenderer& spriteRenderer, bool drawHitbox = false, bool followCamera = false, float outlineWidth = 10, glm::vec4 textboxColor = glm::vec4(1.0f), glm::vec3 hitboxColor = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 textColor = glm::vec3(0.0f), glm::vec3 outlineColor = glm::vec3(0.0f, 0.0f, 1.0f));
	void Move(glm::vec2 newPos);
	void ChangeFont(unsigned int fontSize, std::string fontPath = "");

	void CheckClicked(bool buttonClicked, int& buttonAllowment, glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos);
	void DetectKeys(const bool * keys, int * keyAllowment, bool capsLock, bool shouldHold = false);

	void SetActiveDelegate(const std::function<void()>& func);
	void SetUnActiveDelegate(const std::function<void()>& func);

private:
	char ProcessKey(int keyIndex, bool shiftLeft, bool capsLock, bool shiftRight);

	std::shared_ptr<Texture> m_Texture;
	std::shared_ptr<Texture> m_TextCursor;

	glm::vec2 m_Pos, m_Size;

	TextRenderer * m_TextRenderer;
	glm::vec2 m_TextOffset;
	std::string m_Text;
	Timer * m_CursorTimer;
	bool m_ShowBlink;

	Hitbox * m_Hitbox;
	glm::vec2 m_HitboxOffset;

	bool m_Active; //Is the textbox currently being written to
	std::function<void()> m_ActiveDelegate;
	std::function<void()> m_UnactiveDelegate;
	bool m_ActiveDelegateAdded, m_UnActiveDelegateAdded;
};