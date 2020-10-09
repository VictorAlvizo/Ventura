#include "Texture.h"

Texture::Texture() {
	m_TextureID = -1;
	m_Height = 0;
	m_Width = 0;
	m_NrChannels = 0;
}

Texture::Texture(const std::string& path) {
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char * data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);

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
	/*Nothing is here due to the same reason as in Shader.cpp*/
}

void Texture::Bind(int textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::UnBind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
