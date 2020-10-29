#pragma once
#include "Entity.h"

class CollisionHandler
{
public:
	static bool CollideAABB(Entity& ent, Entity& ent2);
	static bool CollideSAT(Entity& ent, Entity& ent2);

private:
	CollisionHandler();

	static std::vector<glm::ivec2> getCorners(Entity& entity);
};

