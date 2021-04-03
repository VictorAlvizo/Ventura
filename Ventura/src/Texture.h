#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include "stb_image.h"

class Texture {
public:
	//Default constructer
	Texture();
	//Give it the image filepath and any special modes wanted
	Texture(const std::string& path, unsigned int wrapModeS = GL_REPEAT, unsigned int wrapModeT = GL_REPEAT, unsigned int minFilter = GL_LINEAR, unsigned int magFilter = GL_LINEAR);
	~Texture();

	//Primarily for the framebuffer
	void EmptyTexture(unsigned int width, unsigned int height);

	//Bind the texture (when drawing) plus the offset if more then 1 active
	void Bind(int textureUnit = 0);
	//Unbind the texture
	void UnBind();

	//Returns the width of the texture
	inline int getWidth() const { return m_Width; }
	//Returns the height of the texture
	inline int getHeight() const { return m_Height; }
	//Returns the file path of the texture
	inline std::string getPath() const { return m_TexturePath; }

private:
	unsigned int m_TextureID;

	int m_Width, m_Height, m_NrChannels;
	std::string m_TexturePath;

	friend class Filter;
};

