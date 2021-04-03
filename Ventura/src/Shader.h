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
	//Shader constructer, should not be used for an actual shader object with files
	Shader();
	//Load a shader that has a vertex and fragment shder
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	//Load a shader that has a vertex, fragment, and geometry shader
	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);
	~Shader();

	//Bind the shader (when in usage)
	void Bind();
	//Unbind the shader (finished using it)
	void UnBind();

	//Set a uniform bool varible for a shader
	void SetBool(const std::string& name, bool value);
	//Set a uniform int varible for a shader
	void SetInt(const std::string& name, int value);
	//Set a uniform int array varible for a shader, give the size of the array as well
	void SetIntArr(const std::string& name, int arrSize, int vals[]);
	//Set a uniform 2D int array for a shader. Give the size of each row
	void SetInt2(const std::string& name, int rowSize, int * vals);
	//Set a uniform float varible for a shader
	void SetFloat(const std::string& name, float value);
	//Set a uniform float array for a shader, give it the array size
	void SetFloatArr(const std::string& name, int arrSize, float vals[]);
	//Set a uniform of a 2D float array, give it the size of each row
	void SetFloat2(const std::string& name, int rowSize, float * vals);
	//Set a uniform of 2D vector for a shader
	void SetVec2(const std::string& name, glm::vec2 vec2);
	//Set a uniform of a 3D vector for a shader
	void SetVec3(const std::string& name, glm::vec3 vec3);
	//Set a uniform of a 4D vector for a shader
	void SetVec4(const std::string& name, glm::vec4 vec4);
	//Set a uniform of a 4x4 matrix for a shader
	void SetMat4(const std::string& name, glm::mat4 mat4);

	//Retrive the filepath of the vertex shader
	inline std::string getVertexPath() const { return m_VertexPath; }
	//Retrive the filepath of the fragment shader
	inline std::string getFragmentPath() const { return m_FragmentPath; }
	//Retrive the filepath of the geometry shader
	inline std::string getGeometryPath() const { return m_GeometryPath; }
	//Returns the shader program ID
	inline unsigned int getProgram() const { return m_ProgramID; }

private:
	std::string readShader(const std::string& shaderPath);
	unsigned int createShader(unsigned int type, const char * src);

	unsigned int GetUniLocation(const std::string& name);

	unsigned int m_ProgramID;

	std::unordered_map<std::string, unsigned int> m_UniformCache;
	std::string m_VertexPath, m_FragmentPath, m_GeometryPath;
};

