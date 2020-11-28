#pragma once
#include "Entity.h"

class CollisionHandler {
public:
	static bool CollideAABB(Entity& ent, Entity& ent2, bool staticResolution = true);
	static bool CollideSAT(Entity& ent, Entity& ent2, bool staticResolution = true);

private:
	CollisionHandler();
};

