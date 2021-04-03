#pragma once
#include <glad/glad.h>
#include "ResourceManager.h"

enum class FilterMode {
	NONE, SHARPEN, BLUR, EDGEDETECTION, EMBOSS
};

enum class Distortion {
	NONE, UPSIDEDOWN, MIRAGE, SHAKE
};

class Filter {
public:
	//The size of the window (width, height)
	Filter(unsigned int width, unsigned int height);
	~Filter();

	//Add a custom convulation kernal (3x3 matrix) and a name to associate with it
	void AddKernel(const std::string& kernelName, float kernel[]);

	//Call this before drawing whatever you want affected by the filter
	void BeginFilter();
	//Time given by glfwGetTime(), Do FilterMode::NONE if no mode wanted
	void FilterRender(float time, glm::vec3 colorOverlay = glm::vec3(1.0f), FilterMode mode = FilterMode::NONE, Distortion disEffect = Distortion::NONE, float strength = 0.1f, float offsetDiv = 300.0f);
	//Method to use if wanting to use a custom filter, time given by glfwGetTime()
	void FilterRender(float time, std::string filterName, glm::vec3 colorOverlay = glm::vec3(1.0f), Distortion disEffect = Distortion::NONE, float strength = 0.1f, float offsetDiv = 300.0f);
	//Needs the updated size of the window (if the window has been resized) call this after drawing
	//everything you affected by the filter
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

	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Texture> m_Texture;
};