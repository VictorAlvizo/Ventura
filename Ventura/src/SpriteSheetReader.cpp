#include "SpriteSheetReader.h"

SpriteSheetReader::SpriteSheetReader(std::shared_ptr<Texture> texture, glm::vec2 spriteSize) {
	float width = static_cast<float>(texture->getWidth());
	float height = static_cast<float>(texture->getHeight());

	TexUVInfo uvInfo;
	uvInfo.m_X = 0;
	uvInfo.m_Y = 0;
	std::vector<float> currentUV;

	for (float i = 0; i < height / spriteSize.y; i++) {
		uvInfo.m_Y = i;

		for (float j = 0; j < width / spriteSize.x; j++) {
			currentUV = {
				(j * spriteSize.x) / width, ((i + 1) * spriteSize.y) / height,
				((j + 1) * spriteSize.x) / width, (i * spriteSize.y) / height,
				(j * spriteSize.x) / width, (i * spriteSize.y) / height,
				((j + 1) * spriteSize.x) / width, ((i + 1) * spriteSize.y) / height
			};

			uvInfo.m_X = j;
			uvInfo.m_TexUV = currentUV;

			m_TexUVs.push_back(uvInfo);
		}
	}
}

std::vector<float> SpriteSheetReader::getTexUV(int x, int y) { //TODO: There has a to be a better way to do this at n(1)
	for (unsigned int i = 0; i < m_TexUVs.size(); i++) {
		if (m_TexUVs[i].m_X == x && m_TexUVs[i].m_Y == y) {
			return m_TexUVs[i].m_TexUV;
		}
	}
}
