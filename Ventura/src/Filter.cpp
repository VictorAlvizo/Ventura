#include "Filter.h"

Filter::Filter(unsigned int width, unsigned int height) 
	:m_Width(width), m_Height(height)
{
	m_Shader = *ResourceManager::Get<Shader>("filter");

	glGenFramebuffers(1, &m_FBO);
	glGenRenderbuffers(1, &m_RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	m_Texture = Texture();
	m_Texture.EmptyTexture(m_Width, m_Height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.m_TextureID, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Error: Failed to initlize the Framebuffer! Error Meesage:" << std::endl << ErrorCode(glCheckFramebufferStatus(GL_FRAMEBUFFER)) << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	init();
}

Filter::~Filter() {
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteRenderbuffers(1, &m_RBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void Filter::BeginFilter() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Filter::FilterRender(float time) {
	m_Shader.Bind();
	glBindVertexArray(m_VAO);

	m_Texture.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	m_Shader.UnBind();

}

void Filter::EndFilter() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Filter::init() {
	float vertices[] = {
		//Coordinate   //Tex Coordinate
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int vbo;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &vbo);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

std::string Filter::ErrorCode(unsigned int code) {
	switch (code) {
		case GL_FRAMEBUFFER_UNDEFINED:
			return "Default framebuffer does not exist";
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			return "Framebuffer attachment points are framebuffer incomplete";
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			return "Framebuffer does not have atleast one image attached to it";
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			return "Color attachment is GL_NONE for GL_DRAW_BUFFER";
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			return "Color attachment is GL_NONE for GL_READ_BUFFER";
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			return "Combination of internal formats of the attached image violate an implementation-dependent set of restrictions";
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			return "value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES. Or GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures.";
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			return "Framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target.";
			break;

		default:
			return "Error unknown";
			break;
	}
}
