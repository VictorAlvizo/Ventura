#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include "stb_image.h"

class Texture {
public:
	Texture();
	Texture(const std::string& path, unsigned int wrapModeS = GL_REPEAT, unsigned int wrapModeT = GL_REPEAT, unsigned int minFilter = GL_LINEAR, unsigned int magFilter = GL_LINEAR);
	~Texture();

	//Primarily for the framebuffer
	void EmptyTexture(unsigned int width, unsigned int height);

	void Bind(int textureUnit = 0);
	void UnBind();

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline std::string getPath() const { return m_TexturePath; }

private:
	unsigned int m_TextureID;

	int m_Width, m_Height, m_NrChannels;
	std::string m_TexturePath;

	friend class Filter;
};

