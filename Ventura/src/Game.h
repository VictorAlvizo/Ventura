#pragma once
#include <glad/glad.h>
#include "ResourceManager.h"

enum class GameState {
	MENU, ACTIVE
};

class Game {
public:
	Game(unsigned int screenWidth, unsigned int screenHeight);

	void Init();

	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void Render();

	char m_Keys[1024];
	GameState m_State;

private:
	unsigned int m_Width, m_Height;
};

