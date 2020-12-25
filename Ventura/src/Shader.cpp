#include "Shader.h"

Shader::Shader() {
	m_ProgramID = -1;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexSrc = readShader(vertexPath);
	std::string fragmentSrc = readShader(fragmentPath);

	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexSrc.c_str());
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());

	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, vertexShader);
	glAttachShader(m_ProgramID, fragmentShader);
	glLinkProgram(m_ProgramID);

	int success;
	char infoLog[512];
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(m_ProgramID, 512, nullptr, infoLog);
		std::cout << "Error: linking the shader \n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
	std::string vertexSrc = readShader(vertexPath);
	std::string fragmentSrc = readShader(fragmentPath);
	std::string geometrySrc = readShader(geometryPath);

	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexSrc.c_str());
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());
	unsigned int geometryShader = createShader(GL_GEOMETRY_SHADER, geometrySrc.c_str());

	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, vertexShader);
	glAttachShader(m_ProgramID, fragmentShader);
	glAttachShader(m_ProgramID, geometryShader);
	glLinkProgram(m_ProgramID);

	int success;
	char infoLog[512];
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(m_ProgramID, 512, nullptr, infoLog);
		std::cout << "Error: linking the shader" << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);
}

Shader::~Shader() {
	glDeleteProgram(m_ProgramID);
}

void Shader::Bind() {
	glUseProgram(m_ProgramID);
}

void Shader::UnBind() {
	glUseProgram(0);
}

void Shader::SetBool(const std::string& name, bool value) {
	glUniform1i(GetUniLocation(name), (int)value);
}

void Shader::SetInt(const std::string& name, int value) {
	glUniform1i(GetUniLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value) {
	glUniform1f(GetUniLocation(name), value);
}

void Shader::SetFloatArr(const std::string& name, int arrSize, float vals[]) {
	glUniform1fv(GetUniLocation(name), arrSize, vals);
}

void Shader::SetFloat2(const std::string& name, int rowSize, float ** vals) {
	glUniform2fv(GetUniLocation(name), rowSize, (float *)vals);
}

void Shader::SetVec2(const std::string& name, glm::vec2 vec2) {
	glUniform2fv(GetUniLocation(name), 1, &vec2[0]);
}

void Shader::SetVec3(const std::string& name, glm::vec3 vec3) {
	glUniform3fv(GetUniLocation(name), 1, &vec3[0]);
}

void Shader::SetVec4(const std::string& name, glm::vec4 vec4) {
	glUniform4fv(GetUniLocation(name), 1, &vec4[0]);
}

void Shader::SetMat4(const std::string& name, glm::mat4 mat4) {
	glUniformMatrix4fv(GetUniLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
}

std::string Shader::readShader(const std::string& shaderPath) {
	std::ifstream shaderFile(shaderPath);
	std::stringstream fileStream;

	if (!shaderFile.is_open()) {
		std::cout << "Error: Unable to open shader file at: " << shaderPath << std::endl;
	}

	fileStream << shaderFile.rdbuf();
	shaderFile.close();

	return fileStream.str();
}

unsigned int Shader::createShader(unsigned int type, const char * src) {
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	std::string typeStr = "Unknown Shader";
	switch (type) {
		case GL_VERTEX_SHADER:
			typeStr = "Vertex Shader";
			break;

		case GL_FRAGMENT_SHADER:
			typeStr = "Fragment Shader";
			break;

		case GL_GEOMETRY_SHADER:
			typeStr = "Geometry Shader";
			break;
	}

	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "Error: " << typeStr << " not compilied successfully \n" << infoLog << std::endl;
	}

	return shader;
}

unsigned int Shader::GetUniLocation(const std::string& name) {
	if (m_UniformCache.find(name) != m_UniformCache.end()) {
		return m_UniformCache[name];
	}

	int location = glGetUniformLocation(m_ProgramID, name.c_str());

	if (location == -1) {
		std::cout << "Error: Could not find the uniform named: " << name << std::endl;
	}
	else {
		m_UniformCache[name] = location;
	}

	return location;
}
