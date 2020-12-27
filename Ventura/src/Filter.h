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
	Filter(unsigned int width, unsigned int height);
	~Filter();

	void AddKernel(const std::string& kernelName, float kernel[]);

	void BeginFilter();
	//Time given by glfwGetTime(), Do FilterMode::NONE if no mode wanted
	void FilterRender(float time, glm::vec3 colorOverlay = glm::vec3(1.0f), FilterMode mode = FilterMode::NONE, Distortion disEffect = Distortion::NONE, float strength = 0.1f, float offsetDiv = 300.0f);
	void FilterRender(float time, std::string filterName, glm::vec3 colorOverlay = glm::vec3(1.0f), Distortion disEffect = Distortion::NONE, float strength = 0.1f, float offsetDiv = 300.0f);
	//Needs the updated size of the window
	void EndFilter(unsigned int updatedWidth, unsigned int updatedHeight);

private:
	void Init();
	std::string ErrorCode(unsigned int code);

	float* getKernel(FilterMode mode);
	float* getKernel(const std::string& name);

	std::unordered_map<std::string, float *> m_Kernels;

	unsigned int m_FBO;
	unsigned int m_VAO;

	unsigned int m_InitalWidth, m_InitalHeight;

	Shader m_Shader;
	Texture m_Texture;
};

