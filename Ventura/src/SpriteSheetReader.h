#pragma once
#include <vector>
#include "Texture.h"
#include "Vendor/glm/glm.hpp"

class SpriteSheetReader {
public:
	SpriteSheetReader();
	SpriteSheetReader(std::shared_ptr<Texture> texture, glm::vec2 spriteSize);
	~SpriteSheetReader();

	std::vector<float> getTexUV(glm::ivec2 spritePos, bool isFlipped);

	inline glm::vec2 getSpriteSize() const { return m_SpriteSize; }

private:
	std::vector<std::vector<float>> m_TexUVs;

	float m_Width, m_Height; //Texture width and height
	glm::vec2 m_SpriteSize;
};

