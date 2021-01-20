#include "Scene.h"

Scene::Scene(Game * m_GameClass) 
	:m_GameClass(m_GameClass)
{
	//holder
}

Scene::~Scene() {
	m_GameClass = nullptr;
}
