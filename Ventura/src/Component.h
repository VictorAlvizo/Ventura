#pragma once
#include <unordered_map>
#include "AnimationCycle.h"
#include "Audio.h"
#include "ParticleGenerator.h"

class Component {
public:
	Component();
	~Component();

	//Add a componenet of any type
	template<typename T>
	void Add(const std::string& compName, T& newComponent) {
		std::cout << "Error: Not a recognized component" << std::endl;
	}

	//Add an animation component
	template<>
	void Add<AnimationCycle>(const std::string& compName, AnimationCycle& cycle) {
		m_ACComponents[compName] = std::make_shared<AnimationCycle>(cycle);
	}

	//Add an audio component
	template<>
	void Add<Audio>(const std::string& compName, Audio& audio) {
		m_AudioComponents[compName] = std::make_shared<Audio>(audio);
		m_AudioComponents[compName]->m_DeathEnabled = true;
	}

	//Add a particle component
	template<>
	void Add<ParticleGenerator>(const std::string& compName, ParticleGenerator& particleGen) {
		m_ParticleComponents[compName] = std::make_shared<ParticleGenerator>(particleGen);
	}

	//Return the pointer to a component of an unknown type
	template<typename T>
	std::shared_ptr<T>& Get(const std::string& name) {
		std::cout << "Error: Not a recognized component" << std::endl;
		return nullptr;
	}

	//Return the pointer to an AnimationCycle component of the name
	template<> 
	std::shared_ptr<AnimationCycle>& Get(const std::string& name) {
		if (m_ACComponents.find(name) != m_ACComponents.end()) {
			return m_ACComponents[name];
		}
		else {
			std::cout << "Error: Animation component " << name << " could not be found" << std::endl;
		}
	}

	//Return the pointer to an Audio component of the name
	template<>
	std::shared_ptr<Audio>& Get(const std::string& name) {
		if (m_AudioComponents.find(name) != m_AudioComponents.end()) {
			return m_AudioComponents[name];
		}
		else {
			std::cout << "Error: Audio component " << name << " could not be found" << std::endl;
		}
	}

	template<>
	std::shared_ptr<ParticleGenerator>& Get(const std::string& name) {
		if (m_ParticleComponents.find(name) != m_ParticleComponents.end()) {
			return m_ParticleComponents[name];
		}
		else {
			std::cout << "Error: Particle Generator component " << name << " could not be found" << std::endl;
		}
	}

private:
	std::unordered_map<std::string, std::shared_ptr<AnimationCycle>> m_ACComponents;
	std::unordered_map<std::string, std::shared_ptr<Audio>> m_AudioComponents;
	std::unordered_map<std::string, std::shared_ptr<ParticleGenerator>> m_ParticleComponents;
};
