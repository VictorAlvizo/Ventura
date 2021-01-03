#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Vendor/glm/gtc/type_ptr.hpp"

class Shader {
public:
	Shader();
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);
	~Shader();

	void Bind();
	void UnBind();

	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetFloatArr(const std::string& name, int arrSize, float vals[]);
	void SetFloat2(const std::string& name, int rowSize, float ** vals); //TODO: Set custom
	void SetVec2(const std::string& name, glm::vec2 vec2);
	void SetVec3(const std::string& name, glm::vec3 vec3);
	void SetVec4(const std::string& name, glm::vec4 vec4);
	void SetMat4(const std::string& name, glm::mat4 mat4);

	inline std::string getVertexPath() const { return m_VertexPath; }
	inline std::string getFragmentPath() const { return m_FragmentPath; }
	inline std::string getGeometryPath() const { return m_GeometryPath; }

private:
	std::string readShader(const std::string& shaderPath);
	unsigned int createShader(unsigned int type, const char * src);

	unsigned int GetUniLocation(const std::string& name);

	unsigned int m_ProgramID;

	std::unordered_map<std::string, unsigned int> m_UniformCache;
	std::string m_VertexPath, m_FragmentPath, m_GeometryPath;
};

