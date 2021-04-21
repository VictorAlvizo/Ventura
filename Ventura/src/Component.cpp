#include "Component.h"

Component::Component() {
	//holder
}

Component::~Component() {
	m_ACComponents.clear();
	m_AudioComponents.clear();
}