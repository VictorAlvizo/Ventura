#pragma once
#include "Entity.h"

class CollisionHandler
{
public:
	static bool CollideAABB(Entity& ent, Entity& ent2);

private:
	CollisionHandler();
};

