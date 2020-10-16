#pragma once
#include <vector>
#include "Texture.h"
#include "Vendor/glm/glm.hpp"

struct TexUVInfo {
	std::vector<float> m_TexUV;
	int m_X;
	int m_Y;
};

class SpriteSheetReader {
public:
	SpriteSheetReader(std::shared_ptr<Texture> texture, glm::vec2 spriteSize);

	std::vector<float> getTexUV(int x, int y);

private:
	std::vector<TexUVInfo> m_TexUVs;
};

