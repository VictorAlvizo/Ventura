#pragma once
#include <unordered_map>
#include <memory>
#include "Shader.h"
#include "Texture.h"

class ResourceManager {
public:
	//Load a custom shader with a vertex and fragment shaders
	static void LoadShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& name);
	//Load a custom shader with a vertex, fragment, and geometry shaders
	static void LoadShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, const std::string& name);
	//Load a texture
	static void LoadTexture(const std::string& texturePath, const std::string& name);

	//Returns the shader pointer by only the shader filepaths
	static std::shared_ptr<Shader>& GetShaderFromFileName(const std::string& vertexPath, const std::string& fragmentPath, const std::string geometryPath = "");
	//Returns the texture pointer by only the texture filepath
	static std::shared_ptr<Texture>& GetTextureFromFileName(const std::string& fileName);

	//BUG HERE: Program is most likely going to crash if a name is given that can't be found. Reason being
	//there is literally nothing to return. Can't return std::shared_ptr<T>() because I made it 
	//return a reference value; so an rvalue cannot be returned.
	template<typename T>
	static std::shared_ptr<T>& Get(const std::string& name) {
		std::cout << "Error: Undefined type" << std::endl;
		return nullptr;
	}

	template<>
	static std::shared_ptr<Shader>& Get<Shader>(const std::string& name) {
		if (m_Shaders.find(name) != m_Shaders.end()) {
			return m_Shaders[name];
		}
		else {
			std::cout << "Error: Shader: " << name << " could not be found" << std::endl;
		}
	}

	template<>
	static std::shared_ptr<Texture>& Get<Texture>(const std::string& name) {
		if (m_Textures.find(name) != m_Textures.end()) {
			return m_Textures[name];
		}
		else {
			std::cout << "Error: Texture: " << name << " could not be found" << std::endl;
		}
	}

private:
	static std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;

	ResourceManager() = delete;
};
