#include "SpriteSheetReader.h"

SpriteSheetReader::SpriteSheetReader(std::shared_ptr<Texture> texture, glm::vec2 spriteSize) {
	float width = static_cast<float>(texture->getWidth());
	float height = static_cast<float>(texture->getHeight());

	std::vector<float> currentUV;

	for (float i = 0; i < height / spriteSize.y; i++) {
		for (float j = 0; j < width / spriteSize.x; j++) {
			currentUV = {
				(j * spriteSize.x) / width, ((i + 1) * spriteSize.y) / height,
				((j + 1) * spriteSize.x) / width, (i * spriteSize.y) / height,
				(j * spriteSize.x) / width, (i * spriteSize.y) / height,
				((j + 1) * spriteSize.x) / width, ((i + 1) * spriteSize.y) / height
			};

			m_TexUVs.push_back(currentUV);
		}
	}
}
