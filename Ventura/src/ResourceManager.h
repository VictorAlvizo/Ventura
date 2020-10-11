#pragma once
#include <unordered_map>
#include <memory>
#include "Shader.h"
#include "Texture.h"

class ResourceManager {
public:
	static void LoadShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& name);
	static void LoadTexture(const std::string& texturePath, const std::string& name);

	template<typename T>
	static std::shared_ptr<T>& Get(const std::string& name) {
		//holder
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

	static std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;

private:
	ResourceManager();
};
