#include "Game.h"

Game::Game(unsigned int screenWidth, unsigned int screenHeight) 
	:m_Width(screenWidth), m_Height(screenHeight)
{
	m_State = GameState::ACTIVE; //TODO: Change to menu towards the end
}

void Game::Init() {
	//holder
}

void Game::ProcessInput(float deltaTime) {
	//holder
}

void Game::Update(float deltaTime) {
	//holder
}

void Game::Render() {
	if (m_State == GameState::MENU) {
		//holder
	}

	if (m_State == GameState::ACTIVE) {
		//holder
	}
}
