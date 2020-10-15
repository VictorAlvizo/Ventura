#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include "stb_image.h"

class Texture {
public:
	Texture();
	Texture(const std::string& path);
	~Texture();

	void Bind(int textureUnit = 0);
	void UnBind();

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }

private:
	unsigned int m_TextureID;

	int m_Width, m_Height, m_NrChannels;
};

