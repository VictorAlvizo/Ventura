#pragma once
#include <vector>
#include "SpriteRenderer.h"
#include "ResourceManager.h"

class Slider {
public:
	//if want to use the default send the texture named "slider"
	Slider(std::shared_ptr<Texture> slideTexture, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, float initPercentage = 100.0f);

	void Draw(SpriteRenderer& spriteRenderer, glm::vec3 color = glm::vec3(1.0f), bool flip = false, bool followCamera = true);

	void SetPercentage(float newPercentage);
	void AddPercentage(float addAmount);

	glm::vec2 m_Pos, m_Size;
	float m_Rotation;

	inline float getPercentage() const { return m_Percent; }

private:
	std::shared_ptr<Texture> m_Texture;

	std::vector<float> m_DefaultTexUVs;

	float m_Percent;
	float m_Width, m_Height;
};