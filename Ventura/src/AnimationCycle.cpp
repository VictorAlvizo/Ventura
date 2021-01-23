#include "AnimationCycle.h"

AnimationCycle::AnimationCycle() 
	:m_CurrentIndex(glm::ivec2(0))
{
	m_TerminateAnimation = true; //Set to true by default so I can just start the animation thread once
	m_CurrentAnimation = "";
}

AnimationCycle::AnimationCycle(const AnimationCycle& copy) {
	m_TerminateAnimation = true; //Want to end any running threads when copying
	m_CurrentAnimation = "";
	m_Cycles = copy.m_Cycles;
	m_CurrentIndex = copy.m_CurrentIndex;
	m_CurrentCycle = copy.m_CurrentCycle;
}

AnimationCycle::~AnimationCycle() {
	//Check incase TerminateAnimation() has already been called to avoid joining or detaching twice
	if (!m_TerminateAnimation) {
		m_TerminateAnimation = true;
		m_AnimationThread->join(); //Join here as the class will be destroyed, so it dosen't use varibles from a defunct class
		delete m_AnimationThread;
		m_AnimationThread = nullptr;
	}

	m_CurrentAnimation = "";
}

void AnimationCycle::LinearX(const std::string cycleName, int xStart, int xEnd, int y, int speed, bool loop) {
	std::vector<glm::ivec2> indexes;

	for (int i = xStart; i <= xEnd; i++) {
		indexes.push_back(glm::ivec2(i, y));
	}

	m_Cycles[cycleName] = Cycle(indexes, speed, loop);
}

void AnimationCycle::LinearY(const std::string cycleName, int yStart, int yEnd, int x, int speed, bool loop) {
	std::vector<glm::ivec2> indexes;

	for (int i = yStart; i <= yEnd; i++) {
		indexes.push_back(glm::ivec2(x, i));
	}

	m_Cycles[cycleName] = Cycle(indexes, speed, loop);
}

void AnimationCycle::CustomCycle(const std::string cycleName, std::vector<glm::ivec2> spritePoses, int speed, bool loop) {
	m_Cycles[cycleName] = Cycle(spritePoses, speed, loop);
}

void AnimationCycle::Animate(std::string cycleName) {
	if (m_Cycles.find(cycleName) == m_Cycles.end()) {
		std::cout << "Error: Animation cycle " << cycleName << " not found" << std::endl;
	}
	else if (m_CurrentAnimation != cycleName || m_TerminateAnimation) { //To avoid starting an animation that is currently running, again
		std::lock_guard<std::mutex> lock(m_AnimationMutex);

		//Skip setting a new one if a one cycle animation is currently ongoing
		//Note that this will discard the animation requested
		if (m_CurrentCycle.m_Loop || m_CurrentCycle.m_CycleComplete || m_CurrentAnimation == "") {
			if (!m_CurrentCycle.m_Loop) {
				m_CurrentCycle.m_CycleComplete = false; //In case this was one cycle animation that has been completed, reset to false
			}

			m_CurrentCycle = m_Cycles[cycleName];
			m_CurrentAnimation = cycleName;
		}

		//Thread will die once m_TerminateAnimation is true, allows for a max of 1 thread per AnimationCycle class
		if (m_TerminateAnimation) {
			m_TerminateAnimation = false; 
			m_AnimationThread = new std::thread(&AnimationCycle::AnimationThread, this);
		}
	}
}

void AnimationCycle::TerminateAnimation() {
	if (!m_TerminateAnimation) {
		//Delete pointer and detach the thread as the class will still remain alive
		m_AnimationThread->detach();
		m_TerminateAnimation = true;
		delete m_AnimationThread;
		m_AnimationThread = nullptr;
	}
}

void AnimationCycle::AnimationThread() {
	int index = 0;
	Cycle lastCycle = m_CurrentCycle;

	while (!m_TerminateAnimation) {
		std::this_thread::sleep_for(std::chrono::milliseconds(m_CurrentCycle.m_AnimationSpeed));

		//Dpouble check just to make sure, incase while the thread was asleep TerminateAnimation() was called
		if (m_TerminateAnimation) {
			break;
		}

		//Don't want to lock the entire while loop, so encapsulated within a custom scope. Avoid a race condition with m_CurrentCycle
		{
			std::lock_guard<std::mutex> lock(m_AnimationMutex);
			if (lastCycle != m_CurrentCycle) {
				lastCycle = m_CurrentCycle;
				index = 0;
			}

			m_CurrentIndex = m_CurrentCycle.m_Range[index];

			//Animation is not set to loop, don't change the index anymore
			if (index + 1 == m_CurrentCycle.m_Range.size() && !m_CurrentCycle.m_Loop) {
				if (!m_CurrentCycle.m_CycleComplete) {
					m_CurrentCycle.m_CycleComplete = true;
				}
			}
			else {
				index = (index + 1 == m_CurrentCycle.m_Range.size()) ? 0 : index + 1;
			}
		}
	}
}
