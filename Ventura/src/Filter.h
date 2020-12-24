#pragma once
#include <glad/glad.h>
#include "ResourceManager.h"

class Filter {
public:
	//TODO: May add an option for custom shaders
	Filter(unsigned int width, unsigned int height);
	~Filter();

	void BeginFilter();
	void FilterRender(float time);
	void EndFilter();

private:
	void init();
	std::string ErrorCode(unsigned int code);

	unsigned int m_FBO, m_RBO;
	unsigned int m_VAO;

	unsigned int m_Width, m_Height;

	Shader m_Shader;
	Texture m_Texture;
};

