#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "SpriteSheetReader.h"
#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"

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

private:
	unsigned int m_Width, m_Height;

	SpriteRenderer * m_SpriteRenderer;

	SpriteSheetReader * m_SSR; //NOTE: TESTING VARIBLE, REMOVE LATER
};

