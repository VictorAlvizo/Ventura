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
	int m_AnimationSpeed; //Int as the chrono time method only takes in an int
	bool m_Loop;
	bool m_CycleComplete;

	bool operator!=(const Cycle& c2) {
		return this->m_AnimationSpeed != c2.m_AnimationSpeed && this->m_Range != c2.m_Range;
	}
};

class AnimationCycle {
public:
	AnimationCycle();
	//Custom copy constructer to due to data type transferability issues
	AnimationCycle(const AnimationCycle& copy); //Had to make my own as std::mutex cannot be copied
	~AnimationCycle();
	
	//Third argument will stay the same while it ranges the indexes linearly
	//Ex LinearX("Walking", 0, 4, 1) -> (0 - 4, 1)
	void LinearX(const std::string cycleName, int xStart, int xEnd, int y, int speed = 100, bool loop = true);
	//Third argument will stay the same while it ranges the indexes linearly. X remains constant, moves through y
	void LinearY(const std::string cycleName, int yStart, int yEnd, int x, int speed = 100, bool loop = true);
	//Custom x and y position of the wanted sprites if a linear approach is not wanted
	void CustomCycle(const std::string cycleName, std::vector<glm::ivec2> spritePoses, int speed = 100, bool loop = true);
	
	//Commence the animation of a sprite with that name. If the current animation is one 
	//that dosen't loop, new animations will be 
	//discarded until that animation finished
	void Animate(const std::string cycleName);
	//Stop the animation
	void TerminateAnimation();

	//Get the current x and y position of the current sprite image
	inline glm::ivec2 getSpritePos() const { return m_CurrentIndex; }
	//Returns the name of the current animation running
	inline std::string getCurrentAnimation() const { return m_CurrentAnimation; }
	//Check if the animation is still running or not
	inline bool isTerminated() const { return m_TerminateAnimation; }

private:
	void AnimationThread();

	bool m_TerminateAnimation;
	bool m_Copied;

	std::unordered_map<std::string, Cycle> m_Cycles;

	std::string m_CurrentAnimation;
	glm::ivec2 m_CurrentIndex;
	Cycle m_CurrentCycle;

	std::thread * m_AnimationThread;
	std::mutex m_AnimationMutex;
};