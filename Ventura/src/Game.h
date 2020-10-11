#pragma once
#include <glad/glad.h>
#include "ResourceManager.h"
#include "SpriteRenderer.h"

enum class GameState {
	MENU, ACTIVE
};

class Game {
public:
	Game(unsigned int screenWidth, unsigned int screenHeight);
	~Game();

	void Init();

	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void Render();

	char m_Keys[1024];
	GameState m_State;

	unsigned int vao;

private:
	unsigned int m_Width, m_Height;

	SpriteRenderer * m_SpriteRenderer;
};

