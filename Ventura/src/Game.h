#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include "CollisionHandler.h"
#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"
#include "TextRenderer.h"
#include "ParticleGenerator.h"

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

	void CheckCollisions();

	//Mouse and keyboard information
	char m_Keys[1024];
	bool m_MouseButtons[8];
	glm::vec2 m_MousePos;

	GameState m_State;

private:
	unsigned int m_Width, m_Height;

	SpriteRenderer * m_SpriteRenderer;
	Entity * m_TestEntity;

	ParticleGenerator * m_ParticleGenerator;
};

