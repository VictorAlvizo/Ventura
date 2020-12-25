#pragma once
#include <glad/glad.h>
#include "ResourceManager.h"

enum class FilterMode {
	NONE, SHARPEN, BLUR, EDGEDETECTION
};

enum class Distortion {
	NONE, UPSIDEDOWN, MIRAGE, SHAKE
};

class Filter {
public:
	//TODO: May add an option for custom shaders, custom kernal map
	Filter(unsigned int width, unsigned int height);
	~Filter();

	void BeginFilter();
	//Time given by glfwGetTime()
	void FilterRender(float time, glm::vec3 colorOverlay = glm::vec3(1.0f), FilterMode mode = FilterMode::NONE, Distortion disEffect = Distortion::NONE, float strength = 0.1f, float offsetDiv = 300.0f);
	//Needs the updated size of the window
	void EndFilter(unsigned int updatedWidth, unsigned int updatedHeight);

private:
	void Init();
	std::string ErrorCode(unsigned int code);

	float* getKernel(FilterMode mode);

	unsigned int m_FBO;
	unsigned int m_VAO;

	unsigned int m_InitalWidth, m_InitalHeight;

	Shader m_Shader;
	Texture m_Texture;
};

