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
	//If you want to use the default texture put in "button" for the texture argument
	Button(unsigned int windowWidth, unsigned int windowHeight, std::shared_ptr<Texture> buttonTexture, glm::vec2 pos, glm::vec2 size, std::string buttonText = "", float rotation = 0.0f, unsigned int fontSize = 24, glm::vec3 fontColor = glm::vec3(0.0f, 1.0f, 0.0f), std::string customFont = "Fonts/arial.ttf", glm::vec2 hitboxOffset = glm::vec2(0.0f), glm::vec2 hitboxSize = glm::vec2(0.0f));
	Button(unsigned int windowWidth, unsigned int windowHeight, std::shared_ptr<Texture> buttonTexture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> clickTexture, glm::vec2 pos, glm::vec2 size, std::string buttonText = "", float rotation = 0.0f, unsigned int fontSize = 24, glm::vec3 fontColor = glm::vec3(1.0f), std::string customFont = "Fonts/arial.ttf", glm::vec2 hitboxOffset = glm::vec2(0.0f), glm::vec2 hitboxSize = glm::vec2(0.0f));
	~Button();

	void Draw(SpriteRenderer& spriteRenderer, bool drawHitbox = false, bool followCamera = true, glm::vec4 buttonColor = glm::vec4(1.0f), glm::vec3 textColor = glm::vec3(1.0f), glm::vec3 hitboxColor = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2 textOffsets = glm::vec2(0.0f));

	void SetPos(glm::vec2 newPos);
	void SetRotation(float newRotation);

	//If you want to keep the same font only put in the fontsize parameter and that's it
	void ChangeFont(unsigned int fontSize, std::string fontPath = "");

	bool isHovering(glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos);
	//Pass the mouse button you wanted detcted (m_MouseButtons[index])
	bool isClicked(glm::vec2 mousePos, bool mouseButton, bool followingCamera, glm::vec2 cameraPos);

	inline Status currentStatus() const { return m_CurrentStatus; }
	inline glm::vec2 getPos() const { return m_Pos; }
	inline glm::vec2 getSize() const { return m_Size; }

	std::string m_ButtonText;

private:
	Status m_CurrentStatus;

	Hitbox * m_Hitbox;
	TextRenderer * m_Text;

	std::shared_ptr<Texture> m_ButtonTextures[3]; //Array of 3 to match the Status enum

	glm::vec2 m_Pos, m_Size, m_HitboxOffset;
	float m_Rotation;

	std::string m_TextPath;
	unsigned int m_FontSize;
};

