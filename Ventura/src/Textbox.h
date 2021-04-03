#pragma once
#include <functional>
#include "ResourceManager.h"
#include "TextRenderer.h"
#include "Hitbox.h"
#include "CollisionHandler.h"
#include "Timer.h"

class Textbox {
public:
	//Constructer to use if the default textures should be used
	Textbox(unsigned int winWidth, unsigned int winHeight, glm::vec2 pos, glm::vec2 size, std::string defaultText = "", std::string placeHolderText = "", glm::vec2 textOffset = glm::vec2(0.0f), unsigned int fontSize = 0, std::string customFont = "Fonts/arial.ttf", glm::vec2 hitboxOffset = glm::vec2(0.0f), glm::vec2 hitboxSize = glm::vec2(0.0f));
	//Constructer to be use if a custom texture should be used
	Textbox(unsigned int winWidth, unsigned int winHeight, std::shared_ptr<Texture> textboxTexture, glm::vec2 pos, glm::vec2 size, std::string defaultText = "", std::string placeHolderText = "", glm::vec2 textOffset = glm::vec2(0.0f), unsigned int fontSize = 0, std::string customFont = "Fonts/arial.ttf", glm::vec2 hitboxOffset = glm::vec2(0.0f), glm::vec2 hitboxSize = glm::vec2(0.0f));
	~Textbox();

	//Draw the textbox, if passwordmode is true only * will show
	void Draw(SpriteRenderer& spriteRenderer, bool passwordMode = false, bool drawHitbox = false, bool followCamera = false, float outlineWidth = 10, glm::vec4 textboxColor = glm::vec4(1.0f), glm::vec3 hitboxColor = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 textColor = glm::vec3(0.0f), glm::vec3 outlineColor = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 cursorColor = glm::vec3(0.0f), float phOpacity = 0.7f, glm::vec3 phColor = glm::vec3(0.71f, 0.553f, 0.714f));
	//Move the textbox to a set position
	void Move(glm::vec2 newPos);
	//Change the font used in textbox, if fontpath is left as "", the same font will remain in usage
	void ChangeFont(unsigned int fontSize, std::string fontPath = "");
	
	//Check if the textbox has been clicked to active or deactive the textbook
	void CheckClicked(bool buttonClicked, int& buttonAllowment, glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos);
	//Process the keys being pressed if the textbok is active
	void DetectKeys(const bool * keys, int * keyAllowment, bool capsLock, float deltaTime);

	//Set a method to be called if the textbox becomes activated
	void SetActiveDelegate(const std::function<void()>& func);
	//Set a method to be called if the textbox is deactived
	void SetUnActiveDelegate(const std::function<void()>& func);

	//Returns the text of the textbox
	inline std::string getText() const { return m_Text; }
	//Returns if the textbox is currently active or not
	inline bool isActive() const { return m_Active; }

private:
	char ProcessKey(int keyIndex, bool shiftLeft, bool capsLock, bool shiftRight);

	std::shared_ptr<Texture> m_Texture;
	std::shared_ptr<Texture> m_TextCursor;

	glm::vec2 m_Pos, m_Size;

	TextRenderer * m_TextRenderer;
	glm::vec2 m_TextOffset;
	std::string m_Text; //Full text
	std::string m_ShowText; //Text that is being drawn
	std::string m_PlaceholderText;
	Timer * m_CursorTimer;
	bool m_ShowBlink;
	unsigned int m_HideIndex;
	bool m_BlockEntire; //Whether to show entire text or not
	int m_BackBlockAmount;

	float m_HoldTime;
	int m_CurrentKey;
	bool m_HoldEnabled; //User has been holding the key for a specified time, spam the key

	int m_CurrentKeyPos; //Where the user is typing

	bool m_MaxKeyPos; //Inseration cursor is all the way at the last character and no keys being hidden
	unsigned int m_ShadowHide; //Special hide to account for when maxkeypos turns false

	Hitbox * m_Hitbox;
	glm::vec2 m_HitboxOffset;

	bool m_Active; //Is the textbox currently being written to
	std::function<void()> m_ActiveDelegate;
	std::function<void()> m_UnactiveDelegate;
	bool m_ActiveDelegateAdded, m_UnActiveDelegateAdded;
};