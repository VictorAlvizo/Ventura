#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "Texture.h"

class Game {
public:
	Game(unsigned int screenWidth, unsigned int screenHeight);

	void Init();

	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void Render();

	char m_Keys[1024];

private:
	unsigned int m_Width, m_Height;
};

