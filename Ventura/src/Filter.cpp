#include "Filter.h"

Filter::Filter(unsigned int width, unsigned int height) 
	:m_InitalWidth(width), m_InitalHeight(height)
{
	m_Shader = *ResourceManager::Get<Shader>("filter");

	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	m_Texture = Texture();
	m_Texture.EmptyTexture(m_InitalWidth, m_InitalHeight);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.m_TextureID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Error: Failed to initlize the Framebuffer! Error Meesage:" << std::endl << ErrorCode(glCheckFramebufferStatus(GL_FRAMEBUFFER)) << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Init();
}

Filter::~Filter() {
	for (auto const& kernel : m_Kernels) {
		free(kernel.second);
	}

	glDeleteFramebuffers(1, &m_FBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void Filter::AddKernel(const std::string& kernelName, float kernel[]) {
	if (sizeof(kernel) == 8) {
		//Don't want to data to be deleted after going out of scope; store on the heap
		float * arr = static_cast<float *>(std::malloc(9 * sizeof(float)));
		memcpy(arr, kernel, 9 * sizeof(float));
		m_Kernels[kernelName] = arr;
	}
	else {
		std::cout << "Error: Invalid kernal size, must contain 9 floats" << std::endl;
	}
}

void Filter::BeginFilter() {
	glViewport(0, 0, m_InitalWidth, m_InitalHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Filter::FilterRender(float time, glm::vec3 colorOverlay, FilterMode mode, Distortion disEffect, float strength, float offsetDiv) {
	float offset = 1.0f / offsetDiv;

	float offsetArr[9][2] = {
		{-offset, offset}, //Top left
		{0.0f, offset}, //Top center
		{offset, offset}, //Top right
		{-offset, 0.0f}, //Center left
		{0.0f, 0.0f}, //Center
		{offset, 0.0f}, //Center right
		{-offset, -offset}, //Bottom left
		{0.0f, -offset}, //Bottom center
		{offset, -offset} //Bottom right
	};

	//Set up default uniforms
	m_Shader.Bind();
	m_Shader.SetFloat("u_Time", time);
	m_Shader.SetFloat("u_Strength", strength);
	m_Shader.SetFloat2("u_Offsets", 9, (float **)offsetArr);
	m_Shader.SetVec3("u_ColorOverlay", colorOverlay);
	m_Shader.SetInt("u_DistortionType", static_cast<int>(disEffect));

	if (mode != FilterMode::NONE) {
		m_Shader.SetBool("u_KernelEnabled", true);
		m_Shader.SetFloatArr("u_Kernel", 9, getKernel(mode));
	}
	else {
		m_Shader.SetBool("u_KernelEnabled", false);
	}

	glBindVertexArray(m_VAO);

	m_Texture.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	m_Shader.UnBind();
}

void Filter::FilterRender(float time, std::string filterName, glm::vec3 colorOverlay, Distortion disEffect, float strength, float offsetDiv) {
	float offset = 1.0f / offsetDiv;

	float offsetArr[9][2] = {
		{-offset, offset}, //Top left
		{0.0f, offset}, //Top center
		{offset, offset}, //Top right
		{-offset, 0.0f}, //Center left
		{0.0f, 0.0f}, //Center
		{offset, 0.0f}, //Center right
		{-offset, -offset}, //Bottom left
		{0.0f, -offset}, //Bottom center
		{offset, -offset} //Bottom right
	};

	//Set up default uniforms
	m_Shader.Bind();
	m_Shader.SetFloat("u_Time", time);
	m_Shader.SetFloat("u_Strength", strength);
	m_Shader.SetFloat2("u_Offsets", 9, (float**)offsetArr);
	m_Shader.SetVec3("u_ColorOverlay", colorOverlay);
	m_Shader.SetInt("u_DistortionType", static_cast<int>(disEffect));

	m_Shader.SetBool("u_KernelEnabled", true);
	m_Shader.SetFloatArr("u_Kernel", 9, getKernel(filterName));

	glBindVertexArray(m_VAO);

	m_Texture.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	m_Shader.UnBind();
}

void Filter::EndFilter(unsigned int updatedWidth, unsigned int updatedHeight) {
	glViewport(0, 0, updatedWidth, updatedHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Filter::Init() {
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

float* Filter::getKernel(FilterMode mode) {
	if (mode == FilterMode::NONE) {
		return nullptr;
	}
	else if (mode == FilterMode::SHARPEN) {
		static float sharpenKernel[9] = {
			-1, -1, -1,
			-1,  9, -1,
			-1, -1, -1
		};

		return sharpenKernel;
	}
	else if (mode == FilterMode::BLUR) {
		static float blurKernel[9] = {
			1.0 / 16, 2.0 / 16, 1.0 / 16,
			2.0 / 16, 4.0 / 16, 2.0 / 16,
			1.0 / 16, 2.0 / 16, 1.0 / 16
		};

		return blurKernel;
	}
	else if (mode == FilterMode::EDGEDETECTION) {
		static float edgeKernel[9] = {
			1, 1, 1,
			1, 8, 1,
			1, 1, 1
		};

		return edgeKernel;
	}
}

float* Filter::getKernel(const std::string& name) {
	if (m_Kernels.find(name) != m_Kernels.end()) {
		return m_Kernels[name];
	}
	else {
		std::cout << "Error: " << "Kernel " << name << " was not found" << std::endl;
		return nullptr;
	}
}
