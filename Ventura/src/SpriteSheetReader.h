#pragma once
#include <vector>
#include "Texture.h"
#include "Vendor/glm/glm.hpp"

class SpriteSheetReader {
public:
	SpriteSheetReader(std::shared_ptr<Texture> texture, glm::vec2 spriteSize);

	std::vector<float> getTexUV(int index) const { return m_TexUVs[index]; }

private:
	std::vector<std::vector<float>> m_TexUVs;
};

