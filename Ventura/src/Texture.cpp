#include "Texture.h"

Texture::Texture() {
	m_TextureID = -1;
	m_Height = 0;
	m_Width = 0;
	m_NrChannels = 0;
	m_TexturePath = "";
}

Texture::Texture(const std::string& path, unsigned int wrapModeS, unsigned int wrapModeT, unsigned int minFilter, unsigned int magFilter) {
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	unsigned char * data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);
	m_TexturePath = path;

	if (data) {
		GLenum format = GL_RED;

		if (m_NrChannels == 1) {
			format = GL_RED;
		}
		else if (m_NrChannels == 3) {
			format = GL_RGB;
		}
		else if (m_NrChannels == 4) {
			format = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
	}
	else {
		std::cout << "Error: Texture failed to load at path: " << path << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_TextureID);
}

void Texture::EmptyTexture(unsigned int width, unsigned int height) {
	m_Width = width;
	m_Height = height;

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::Bind(int textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::UnBind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}