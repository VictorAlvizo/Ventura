#include "TextRenderer.h"

TextRenderer::TextRenderer(const unsigned int width, const unsigned int height, std::shared_ptr<Shader> shader) {
	m_Shader = shader;
	m_Shader->Bind();

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
	m_Shader->SetMat4("u_Projection", projection); //Note: Custom shader must have a uniformed called "u_Projection" to work
	m_Shader->SetInt("u_Text", 0); //Note: There also has to be a sampler2D uniform called "u_Text" to work
	m_Shader->UnBind();

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindVertexArray(0);
}

bool TextRenderer::LoadFont(const std::string& fontPath, const unsigned int fontSize) {
	m_Characters.clear();

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "Error: Could not initlize the FreeType library" << std::endl;
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		std::cout << "Error: Failed to load the font at: " << fontPath << std::endl;
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Disable byte-alignment restriction, may not be multiple of 4 bytes

	for (unsigned char c = 0; c < 128; c++) {
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Error: Failed to load glyph: " << c << std::endl;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			face->glyph->advance.x,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top)
		};

		m_Characters[c] = character;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return true;
}

void TextRenderer::Text(const std::string& text, float x, float y, float scale, glm::vec3 color) {
	m_Shader->Bind();
	m_Shader->SetVec3("u_TextColor", color); //Note: Same thing as above ^, needs this uniform to work

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = m_Characters[*c];

		float xPos = x + ch.m_Bearing.x * scale;
		float yPos = y + (m_Characters['H'].m_Bearing.y - ch.m_Bearing.y) * scale;

		float w = ch.m_Size.x * scale;
		float h = ch.m_Size.y * scale;

		float vertices[6][4] = {
			{xPos, yPos + h, 0.0f, 1.0f},
			{xPos + w, yPos, 1.0f, 0.0f},
			{xPos, yPos, 0.0f, 0.0f},

			{xPos, yPos + h, 0.0f, 1.0f},
			{xPos + w, yPos + h, 1.0f, 1.0f},
			{xPos + w, yPos, 1.0f, 0.0f}
		};

		glBindTexture(GL_TEXTURE_2D, ch.m_TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.m_Advance >> 6) * scale;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_Shader->UnBind();
}
