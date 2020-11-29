#include "Circle.h"

Circle::Circle() 
	:Entity(), m_Radius(1), m_HBRadius(0.0f)
{
	//holder
}

Circle::Circle(std::shared_ptr<Texture>& texture, glm::vec2 pos, float radius, glm::vec2 hbPos, float hbRadius)
	:Entity(texture, pos, glm::vec2(radius * 2.0f), hbPos, glm::vec2(hbRadius * 2.0f)), m_Radius(radius)
{
	m_HBRadius = (hbRadius == 0.0f) ? radius : hbRadius;
}

Circle::~Circle() {
	//holder
}
