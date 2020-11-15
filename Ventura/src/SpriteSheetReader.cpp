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
				(j * m_SpriteSize.x) / m_Width, ((i + 1) * m_SpriteSize.y) / m_Height,
				((j + 1) * m_SpriteSize.x) / m_Width, (i * m_SpriteSize.y) / m_Height,
				(j * m_SpriteSize.x) / m_Width, (i * m_SpriteSize.y) / m_Height,
				((j + 1) * m_SpriteSize.x) / m_Width, ((i + 1) * m_SpriteSize.y) / m_Height
			});
		}
	}
}

std::vector<float> SpriteSheetReader::getTexUV(int x, int y) {
	int xMax = m_Width / m_SpriteSize.x;
	int yMax = m_Height / m_SpriteSize.y;

	if (x > xMax - 1 || y > yMax - 1) {
		std::cout << "Error: Beyond valid sprite coordinates" << std::endl;
		return m_TexUVs[0];
	}
	else {
		//* by xMax to skip a row
		return m_TexUVs[(y * xMax) + x];
	}
}
