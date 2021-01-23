#pragma once
#include "EngineHeader.h"

class Game;

class Scene {
public:
	//Include the game class in the .cpp of the derived classes and in game friend class the derived classes
	Scene(Game * m_GameClass);
	virtual ~Scene();

	virtual void Init() = 0;
	virtual void Destroy() = 0;

	virtual void ProcessInput(float deltaTime) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void CheckCollisions();

protected:
	Game * m_GameClass;
};

