#pragma once
#include "ResourceManager.h"
#include "TextRenderer.h"
#include "Hitbox.h"
#include "CollisionHandler.h"
#include "Timer.h"

//TODO: Set delegates when the user activates the textbox
class Textbox {
public:
	Textbox(unsigned int winWidth, unsigned int winHeight, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, std::string defaultText = "", glm::vec2 textOffset = glm::vec2(0.0f), unsigned int fontSize = 0, std::string customFont = "Fonts/arial.ttf", glm::vec2 hitboxOffset = glm::vec2(0.0f), glm::vec2 hitboxSize = glm::vec2(0.0f));
	~Textbox();

	void Draw(SpriteRenderer& spriteRenderer, bool drawHitbox = false, bool followCamera = false, float outlineWidth = 10, glm::vec4 textboxColor = glm::vec4(1.0f), glm::vec3 hitboxColor = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 textColor = glm::vec3(0.0f), glm::vec3 outlineColor = glm::vec3(0.0f, 0.0f, 1.0f));
	void Move(glm::vec2 newPos);
	void SetRotation(float rotation);

	void CheckClicked(bool buttonClicked, int& buttonAllowment, glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos);
	void DetectKeys(const bool * keys, int * keyAllowment, bool shouldHold = false);

private:
	std::shared_ptr<Texture> m_Texture;
	std::shared_ptr<Texture> m_TextCursor;

	glm::vec2 m_Pos, m_Size;
	float m_Rotation;

	TextRenderer * m_TextRenderer;
	glm::vec2 m_TextOffset;
	std::string m_Text;
	Timer * m_CursorTimer;
	bool m_ShowBlink;

	Hitbox * m_Hitbox;
	glm::vec2 m_HitboxOffset;

	bool m_Active; //Is the textbox currently being written to
};