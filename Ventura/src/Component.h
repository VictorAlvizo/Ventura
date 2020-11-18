#pragma once
#include <unordered_map>
#include "AnimationCycle.h"

class Component {
public:
	Component();
	~Component();

private:
	//Note: Does it have to be a pointer?
	std::unordered_map<std::string, AnimationCycle> m_ACComponents;
};

