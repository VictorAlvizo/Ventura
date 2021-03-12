#pragma once
#include <glad/glad.h>
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H
#include "ResourceManager.h"

struct Character {
	unsigned int m_TextureID;
	unsigned int m_Advance;

	glm::ivec2 m_Size;
	glm::ivec2 m_Bearing;
};

class TextRenderer {
public:
	//Width and height must = the width and height of the game window
	TextRenderer(const unsigned int width, const unsigned int height, std::string fontPath = "Fonts/arial.ttf", unsigned int fontSize = 24);
	~TextRenderer();

	bool LoadFont(const std::string& fontPath, const unsigned int fontSize);
	void Text(const std::string& text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f), bool followCamera = false, float opacity = 1.0f, int inserationOffsetLimit = 0);

	float m_Rotation;

	inline std::string getCurrentFont() const { return m_CurrentFont; }
	inline unsigned int getFontSize() const { return m_FontSize; }
	inline float getInserationOffset() const { return m_InserationOffset; }

private:
	std::unordered_map<char, Character> m_Characters;

	std::shared_ptr<Shader> m_Shader;
	unsigned int m_VAO, m_VBO;

	std::string m_CurrentFont;
	unsigned int m_FontSize;

	float m_InserationOffset; //Used for the inseration cursor position offset in Textbox
};