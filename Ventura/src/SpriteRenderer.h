#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "Texture.h"

class SpriteRenderer {
public:
	SpriteRenderer();
	//Only need to set childClass as true when wanting to override the Init() method
	SpriteRenderer(std::shared_ptr<Shader> shader, bool childClass = false);
	~SpriteRenderer();

	virtual void DrawSprite(Texture& texture, glm::vec2 pos, 
		glm::vec2 size = glm::vec2(10.0f), bool flipped = false, bool followCamera = true, float rotate = 0.0f, glm::vec4 color = glm::vec4(1.0f),
		std::vector<float> texUV = {
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f
		}, std::vector<float> posUV = {
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f
		});

protected:
	virtual void init();

	unsigned int m_VAO, m_TexVBO, m_PosVBO;

	std::shared_ptr<Shader> m_Shader;
};

