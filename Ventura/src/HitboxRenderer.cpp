#include "HitboxRenderer.h"

HitboxRenderer::HitboxRenderer(std::shared_ptr<Shader> shader) 
	:SpriteRenderer(shader, true)
{
	init();
}

HitboxRenderer::~HitboxRenderer() {
	//holder
}

void HitboxRenderer::DrawOutline(glm::vec2 pos, glm::vec2 size, float rotate, bool followCamera, glm::vec3 outlineColor) {
	m_Shader->Bind();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos, 0.0f));

	//Setting origin to the center then translating back
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_Shader->SetMat4("u_Model", model);
	m_Shader->SetVec3("u_Color", outlineColor);
	m_Shader->SetBool("u_FollowCamera", followCamera);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	m_Shader->UnBind();
}

void HitboxRenderer::init() {
	float posVertices[] = {
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
	glGenBuffers(1, &ibo);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(posVertices), posVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
