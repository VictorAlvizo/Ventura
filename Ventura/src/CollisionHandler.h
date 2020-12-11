#pragma once
#include "Circle.h"

class CollisionHandler {
public:
	static bool CollideAABB(Hitbox& hb1, Hitbox& hb2, bool staticResolution = true);
	static bool CollideSAT(Hitbox& hb1, Hitbox& hb2, bool staticResolution = true);
	static bool CollideCircle(HitCircle * cir1, HitCircle * cir2, bool staticResolution = true);
	static bool CollideCircleAABB(Circle * cir, Entity& ent, bool staticResolution = true);
	static bool CollidePoint(glm::vec2 point, Entity& ent);
	static bool CollidePoint(glm::vec2 point, Circle * cir);

private:
	enum class CollisionDirction {
		RIGHT, LEFT, TOP, BOTTOM
	};

	static CollisionDirction colDir(glm::vec2 centerVector);

	CollisionHandler();
};

