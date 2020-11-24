#pragma once
#include <glad/glad.h>
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H
#include "Texture.h"
#include "Shader.h"

struct Character {
	unsigned int m_TextureID;
	unsigned int m_Advance;

	glm::ivec2 m_Size;
	glm::ivec2 m_Bearing;
};

class TextRenderer {
public:
	TextRenderer(const unsigned int width, const unsigned int height, std::shared_ptr<Shader> shader);

	bool LoadFont(const std::string& fontPath, const unsigned int fontSize);
	
	void Text(const std::string& text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	std::unordered_map<char, Character> m_Characters;

	std::shared_ptr<Shader> m_Shader;
	unsigned int m_VAO, m_VBO;
};

