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

	void CheckClicked(bool buttonClicked, glm::vec2 mousePos, bool followingCamera, glm::vec2 cameraPos, bool percentWithClick = true);

	void Draw(SpriteRenderer& spriteRenderer, bool showHitbox = true, bool flip = false, glm::vec3 foregroundColor = glm::vec3(1.0f), glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 hitboxColor = glm::vec3(0.0f, 1.0f, 0.0f), bool followCamera = true);
	void Move(glm::vec2 newPos);
	void Rotate(float newRotation);

	void SetPercentage(float newPercentage);
	void AddPercentage(float addAmount);
	void SetDelegate(const std::function<void(float)>& func);

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

	std::function<void(float)> m_Delegate;
	bool m_DelegateAdded;
};