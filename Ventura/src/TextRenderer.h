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

	//Load a font to be used by the Textrenderer
	bool LoadFont(const std::string& fontPath, const unsigned int fontSize);
	//Draw the text, if followcamera is true the textrenderer will remain unaffected by camera movement
	//Inseration limit refers to how many characters should be able to add onto the inseration length. Does not effect the max inseration offset
	void Text(const std::string& text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f), bool followCamera = false, float opacity = 1.0f, int inserationOffsetLimit = 0);

	//Returns the filepath of the current font in usage
	inline std::string getCurrentFont() const { return m_CurrentFont; }
	//Returns the current font size
	inline unsigned int getFontSize() const { return m_FontSize; }
	//Returns the character width offsets constrained by the limit inputted
	inline float getInserationOffset() const { return m_InserationOffset; }
	//Returns the max offset of by the characters widths added together
	inline float getInserationOffsetMax() const { return m_InserationOffsetMax; }

	float m_Rotation;

private:
	std::unordered_map<char, Character> m_Characters;

	std::shared_ptr<Shader> m_Shader;
	unsigned int m_VAO, m_VBO;

	std::string m_CurrentFont;
	unsigned int m_FontSize;

	float m_InserationOffset; //Used for the inseration cursor position offset in Textbox
	float m_InserationOffsetMax;
};