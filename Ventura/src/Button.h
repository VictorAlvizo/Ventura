#pragma once
#include "Texture.h"
#include "TextRenderer.h"
#include "Hitbox.h"
#include "CollisionHandler.h"

enum class Status {
	NORMAL, HOVER, CLICKED
};

class Button {
public:
	Button(unsigned int windowWidth, unsigned int windowHeight, Texture& buttonTexture, glm::vec2 pos, glm::vec2 size, std::string buttonText = "", float rotation = 0.0f, unsigned int fontSize = 24, glm::vec3 fontColor = glm::vec3(1.0f), std::string customFont = "Fonts/arial.ttf");
	Button(unsigned int windowWidth, unsigned int windowHeight, Texture& buttonTexture, Texture& hoverTexture, Texture& clickTexture, glm::vec2 pos, glm::vec2 size, std::string buttonText = "", float rotation = 0.0f, unsigned int fontSize = 24, glm::vec3 fontColor = glm::vec3(1.0f), std::string customFont = "Fonts/arial.ttf");
	~Button();

	void Draw(SpriteRenderer& spriteRenderer, bool drawHitbox = false, glm::vec4 buttonColor = glm::vec4(1.0f), glm::vec3 textColor = glm::vec3(1.0f), glm::vec3 hitboxColor = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2 textOffsets = glm::vec2(0.0f));

	void SetPos(glm::vec2 newPos);
	void SetRotation(float newRotation);

	void ChangeFont(unsigned int fontSize, std::string fontPath);

	bool isHovering(glm::vec2 mousePos);
	//Pass the mouse button you wanted detcted (m_MouseButtons[index])
	bool isClicked(glm::vec2 mousePos, bool mouseButton);

	inline Status currentStatus() const { return m_CurrentStatus; }
	inline glm::vec2 getPos() const { return m_Pos; }
	inline glm::vec2 getSize() const { return m_Size; }

	std::string m_ButtonText;

private:
	Status m_CurrentStatus;

	Hitbox * m_Hitbox;
	TextRenderer * m_Text;

	Texture m_ButtonTextures[3]; //Array of 3 to match the Status enum

	glm::vec2 m_Pos, m_Size;
	float m_Rotation;

	std::string m_TextPath;
	unsigned int m_FontSize;
};

