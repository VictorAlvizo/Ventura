#pragma once
#include <unordered_map>
#include "AnimationCycle.h"
#include "Audio.h"

class Component {
public:
	Component();
	~Component();

	template<typename T>
	void Add(const std::string& compName, T& newComponent) {
		std::cout << "Error: Not a recognized component" << std::endl;
	}

	template<>
	void Add<AnimationCycle>(const std::string& compName, AnimationCycle& cycle) {
		m_ACComponents[compName] = std::make_shared<AnimationCycle>(cycle);
	}

	template<>
	void Add<Audio>(const std::string& compName, Audio& audio) {
		m_AudioComponents[compName] = std::make_shared<Audio>(audio);
		m_AudioComponents[compName]->m_DeathEnabled = true;
	}

	template<typename T>
	std::shared_ptr<T>& Get(const std::string& name) {
		std::cout << "Error: Not a recognized component" << std::endl;
		return nullptr;
	}

	template<> 
	std::shared_ptr<AnimationCycle>& Get(const std::string& name) {
		if (m_ACComponents.find(name) != m_ACComponents.end()) {
			return m_ACComponents[name];
		}
		else {
			std::cout << "Error: Animation component " << name << " could not be found" << std::endl;
		}
	}

	template<>
	std::shared_ptr<Audio>& Get(const std::string& name) {
		if (m_AudioComponents.find(name) != m_AudioComponents.end()) {
			return m_AudioComponents[name];
		}
		else {
			std::cout << "Error: Audio component " << name << " could not be found" << std::endl;
		}
	}

private:
	std::unordered_map<std::string, std::shared_ptr<AnimationCycle>> m_ACComponents;
	std::unordered_map<std::string, std::shared_ptr<Audio>> m_AudioComponents;
};
