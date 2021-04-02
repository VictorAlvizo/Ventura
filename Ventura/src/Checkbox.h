#pragma once
#include <functional>
#include "Hitbox.h"
#include "CollisionHandler.h"
#include "TextRenderer.h"

class Checkbox
{
public:
	//Constructer if you wish to keep the default checkbox textures
	Checkbox(unsigned int windowWidth, unsigned int windowHeight, bool initalCheck, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, glm::vec2 textOffset = glm::vec2(0.0f), std::string text = "Checkbox", unsigned int fontSize = 0, std::string customFont = "Fonts/arial.ttf", glm::vec2 hitboxOffset = glm::vec2(0.0f), glm::vec2 hitboxSize = glm::vec2(0.0f));
	//Constructer for custom checkbox textures
	Checkbox(unsigned int windowWidth, unsigned int windowHeight, bool initalCheck, std::shared_ptr<Texture> uncheckedTexture, std::shared_ptr<Texture> checkedTexture, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, glm::vec2 textOffset = glm::vec2(0.0f), std::string text = "Checkbox", unsigned int fontSize = 0, std::string customFont = "Fonts/arial.ttf", glm::vec2 hitboxOffset = glm::vec2(0.0f), glm::vec2 hitboxSize = glm::vec2(0.0f));
	~Checkbox();

	//Will invert from the current checked state if clicked
	void CheckClicked(bool buttonClicked, int& buttonAllowment, glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos);

	//Drawn the checkbox, if followCamera is true, the checkbox will always remain in its constant position regardless of camera movement
	void Draw(SpriteRenderer& spriteRenderer, bool drawHitbox = false, bool flipped = false, bool followCamera = true, glm::vec4 checkboxColor = glm::vec4(1.0f), glm::vec3 hitboxColor = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 textColor = glm::vec3(1.0f));
	//Move the camera to the set position
	void Move(glm::vec2 newPos);

	//Set the rotation of the Checkbox
	void SetRotation(float newRotation);
	//Change the font of the checkbox, if fontpath is left empty, the same font will remain in usage
	void ChangeFont(unsigned int fontSize, std::string fontPath = "");

	//Set a method to be called everytime the checkbox is checked
	void SetCheckDelegate(const std::function<void()>& func);
	//Set a method to be called everytime the checkbox is unchecked
	void SetUncheckDelegate(const std::function<void()>& func);

	//Return if the checkbox's current state is true or false (checked or unchecked)
	inline bool isChecked() const { return m_Checked; }

	std::string m_HeaderText;

private:
	bool m_Checked;

	std::shared_ptr<Texture> m_UncheckedTexture;
	std::shared_ptr<Texture> m_CheckedTexture;

	Hitbox * m_Hitbox;
	glm::vec2 m_HitboxOffset;

	TextRenderer * m_Text;
	glm::vec2 m_TextOffset;

	glm::vec2 m_Pos, m_Size;

	float m_Rotation;

	std::function<void()> m_CheckDelegate;
	std::function<void()> m_UncheckDelegate;
	bool m_CheckDelegateAdded;
	bool m_UncheckDelegateAdded;
};