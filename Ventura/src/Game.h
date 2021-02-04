#pragma once
#include "EngineHeader.h"

class Game {
public:
	Game(unsigned int screenWidth, unsigned int screenHeight, float gravity);
	~Game();

	void Init();

	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void Render();

	//Mouse and keyboard information
	char m_Keys[1024];
	int m_KeyAllowment[1024]; //Counter for when you don't want ghost presses/key holds detected
	bool m_MouseButtons[8];
	glm::vec2 m_MousePos;

	unsigned int m_Width, m_Height;
	float m_Gravity;

	bool m_CloseWindow = false;

private:
	void CheckCollisions();

	void EngineInit();
	void EngineUpdate();

	SpriteRenderer * m_SpriteRenderer;
	Camera * m_Camera;

	Timer * timer;
};