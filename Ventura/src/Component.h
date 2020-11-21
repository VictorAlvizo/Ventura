#pragma once
#include <unordered_map>
#include "AnimationCycle.h"

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
		m_ACComponents[compName] = std::make_unique<AnimationCycle>(cycle);
	}

	template<typename T>
	std::unique_ptr<T>& Get(const std::string& name) {
		std::cout << "Error: Not a recognized component" << std::endl;
		return nullptr;
	}

	template<> 
	std::unique_ptr<AnimationCycle>& Get(const std::string& name) {
		if (m_ACComponents.find(name) != m_ACComponents.end()) {
			return m_ACComponents[name];
		}
		else {
			std::cout << "Error: Animation Component " << name << " could not be found" << std::endl;
		}
	}

private:
	std::unordered_map<std::string, std::unique_ptr<AnimationCycle>> m_ACComponents;
};
