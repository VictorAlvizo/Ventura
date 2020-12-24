#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include "CollisionHandler.h"
#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"
#include "TextRenderer.h"
#include "ParticleGenerator.h"
#include "Camera.h"
#include "Filter.h"

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

	TextRenderer * m_Text;

	SpriteRenderer * m_SpriteRenderer;
	ParticleGenerator * m_ParticleGenerator;
	Camera * m_Camera;

	Filter * m_Filter;

	Entity * m_TestEntity;
};

