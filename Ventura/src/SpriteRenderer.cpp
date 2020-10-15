#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(std::shared_ptr<Shader>& shader) 
	:m_Shader(shader)
{
	init();
}

void SpriteRenderer::DrawSprite(std::shared_ptr<Texture>& texture, glm::vec2 pos, glm::vec2 size, float rotate, glm::vec3 color, std::vector<float> texUV) {
	m_Shader->Bind();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos, 0.0f));

	//Setting origin to the center then translating back
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_Shader->SetMat4("u_Model", model);
	m_Shader->SetVec3("u_Color", color);

	texture->Bind(0);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texUV), &texUV[0]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::init() {
	float posVertices[] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f
	};

	float texVertices[] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 3, 1
	};

	unsigned int posVBO, ibo;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &posVBO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &ibo);

	glBindVertexArray(m_VAO);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(posVertices), posVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texVertices), texVertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
