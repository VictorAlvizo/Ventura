#include "ResourceManager.h"

std::unordered_map<std::string, std::shared_ptr<Shader>>  ResourceManager::m_Shaders;
std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::m_Textures;

void ResourceManager::LoadShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& name) {
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexPath, fragmentPath);
	m_Shaders[name] = shader;
}

void ResourceManager::LoadShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, const std::string& name) {
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexPath, fragmentPath, geometryPath);
	m_Shaders[name] = shader;
}

void ResourceManager::LoadTexture(const std::string& texturePath, const std::string& name) {
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(texturePath);
	m_Textures[name] = texture;
}
