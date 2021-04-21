#include "SpriteSheetReader.h"

SpriteSheetReader::SpriteSheetReader()
	:m_Width(0.0f), m_Height(0.0f), m_SpriteSize(glm::vec2(0.0f))
{
	//holder
}

SpriteSheetReader::SpriteSheetReader(std::shared_ptr<Texture> texture, glm::vec2 spriteSize) {
	m_Width = static_cast<float>(texture->getWidth());
	m_Height = static_cast<float>(texture->getHeight());
	m_SpriteSize = spriteSize;

	for (float i = 0; i < m_Height / m_SpriteSize.y; i++) {
		for (float j = 0; j < m_Width / m_SpriteSize.x; j++) {
			m_TexUVs.push_back({
				(j * m_SpriteSize.x) / m_Width, ((i + 1) * m_SpriteSize.y) / m_Height, //Top left
				((j + 1) * m_SpriteSize.x) / m_Width, (i * m_SpriteSize.y) / m_Height, //Bottom right
				(j * m_SpriteSize.x) / m_Width, (i * m_SpriteSize.y) / m_Height, //Bottom left
				((j + 1) * m_SpriteSize.x) / m_Width, ((i + 1) * m_SpriteSize.y) / m_Height //Top Right
			});
		}
	}
}

SpriteSheetReader::~SpriteSheetReader() {
	//holder
}

std::vector<float> SpriteSheetReader::getTexUV(glm::ivec2 spritePos, bool isFlipped) {
	int xMax = m_Width / m_SpriteSize.x;
	int yMax = m_Height / m_SpriteSize.y;

	if (spritePos.x > xMax - 1 || spritePos.y > yMax - 1) {
		std::cout << "Error: Beyond valid sprite coordinates" << std::endl;
		return m_TexUVs[0];
	}
	else {
		//* by xMax to skip a row, if the texture is flipped, need to inverse my x coordinate
		return (!isFlipped) ? m_TexUVs[(spritePos.y * xMax) + spritePos.x] : m_TexUVs[(spritePos.y * xMax) + (xMax - 1) - spritePos.x];
	}
}