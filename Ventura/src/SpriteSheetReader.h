#pragma once
#include <vector>
#include "Texture.h"
#include "Vendor/glm/glm.hpp"

class SpriteSheetReader {
public:
	SpriteSheetReader();
	SpriteSheetReader(std::shared_ptr<Texture> texture, glm::vec2 spriteSize);

	std::vector<float> getTexUV(int x, int y);

private:
	std::vector<std::vector<float>> m_TexUVs;

	float m_Width, m_Height; //Texture width and height
	glm::vec2 m_SpriteSize;
};

