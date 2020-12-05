#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <mutex>
#include "Vendor/glm/glm.hpp"

struct Cycle {
	Cycle() { }
	Cycle(std::vector<glm::ivec2> range, int speed, bool loop) 
		:m_Range(range), m_AnimationSpeed(speed), m_Loop(loop)
	{
		m_CycleComplete = (m_Loop) ? true : false;
	}

	std::vector<glm::ivec2> m_Range;
	int m_AnimationSpeed;
	bool m_Loop;
	bool m_CycleComplete;

	bool operator!=(const Cycle& c2) {
		return this->m_AnimationSpeed != c2.m_AnimationSpeed && this->m_Range != c2.m_Range;
	}
};

class AnimationCycle {
public:
	AnimationCycle();
	AnimationCycle(const AnimationCycle& copy); //Had to make my own as std::mutex cannot be copied
	~AnimationCycle();
	
	//Third argument will stay the same while it ranges the indexes linearly
	//Ex LinearX("Walking", 0, 4, 1) -> (0 - 4, 1)
	void LinearX(const std::string cycleName, int xStart, int xEnd, int y, int speed = 100, bool loop = true);
	void LinearY(const std::string cycleName, int yStart, int yEnd, int x, int speed = 100, bool loop = true);
	void CustomCycle(const std::string cycleName, std::vector<glm::ivec2> spritePoses, int speed = 100, bool loop = true);

	void Animate(const std::string cycleName);

	inline glm::ivec2 getSpritePos() const { return m_CurrentIndex; }
	inline std::string getCurrentAnimation() const { return m_CurrentAnimation; }

	bool m_TerminateAnimation;

private:
	void AnimationThread();

	std::unordered_map<std::string, Cycle> m_Cycles;

	std::string m_CurrentAnimation;
	glm::ivec2 m_CurrentIndex;
	Cycle m_CurrentCycle;

	std::mutex m_AnimationMutex;
};

