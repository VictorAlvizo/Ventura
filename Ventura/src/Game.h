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
#include "FileSystem.h"

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

private:
	void CheckCollisions();

	SpriteRenderer * m_SpriteRenderer;
	Camera * m_Camera;

	Entity * m_TestEntity;
	Hitbox * m_GroundFloor;

	Entity coolBeans;
};

