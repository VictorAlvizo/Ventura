#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "Texture.h"

class SpriteRenderer {
public:
	SpriteRenderer(std::shared_ptr<Shader> shader);

	void DrawSprite(Texture& texture, glm::vec2 pos, 
		glm::vec2 size = glm::vec2(10.0f), bool flipped = false, float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f),
		std::vector<float> texUV = {
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f
		});

private:
	void init();

	unsigned int m_VAO, m_TexVBO;

	std::shared_ptr<Shader> m_Shader;
};

