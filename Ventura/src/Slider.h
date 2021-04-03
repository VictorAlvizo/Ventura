#pragma once
#include <vector>
#include <functional>
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Hitbox.h"
#include "CollisionHandler.h"

class Slider {
public:
	//if want to use the default send the texture named "slider"
	Slider(std::shared_ptr<Texture> slideTexture, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, float initPercentage = 100.0f, glm::vec2 hitboxOffset = glm::vec2(0.0f), glm::vec2 hitboxSize = glm::vec2(0.0f));
	~Slider();

	//Call to check if the slider has been clicked on, if percent with click is true, it will change the slider volume with the mouse.
	//If followcamera is true, the slider will remain unaffected by the movement of the camera
	void CheckClicked(bool buttonClicked, glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos, bool percentWithClick = true);

	//Draw the slider, if followcamera is true, the slider will remain unaffected by the movement of the camera
	void Draw(SpriteRenderer& spriteRenderer, bool showHitbox = true, bool flip = false, glm::vec3 foregroundColor = glm::vec3(1.0f), glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 hitboxColor = glm::vec3(0.0f, 1.0f, 0.0f), bool followCamera = true);
	//Move the slider to a set position
	void Move(glm::vec2 newPos);
	//Rotate the slider
	void Rotate(float newRotation);

	//Set a new value for the slider (must range between 0.0f - 100.0)
	void SetPercentage(float newPercentage);
	//Add to the percentage value
	void AddPercentage(float addAmount);
	//Set a method to be called everytime the slider is clicked, will pass the slider value
	void SetDelegate(const std::function<void(float)>& func);
	//Set a method to be called everytime the slider value changes
	void SetValueChangedDelegate(const std::function<void(float)>& func);

	//Return the current value of the slider
	inline float getPercentage() const { return m_Percent; }

private:
	std::shared_ptr<Texture> m_Texture;

	std::vector<float> m_DefaultTexUVs;

	Hitbox * m_Hitbox;
	glm::vec2 m_HitboxOffset;

	glm::vec2 m_Pos, m_Size;

	float m_Percent;
	float m_Width, m_Height;
	float m_Rotation;

	std::function<void(float)> m_Delegate; //Delegate to call when clicked
	std::function<void(float)> m_ValueChangedDelegate;
	bool m_DelegateAdded;
	bool m_ValueChangedDelegateAdded;
};