#pragma once
#include "Circle.h"

class CollisionHandler {
public:
	static bool CollideAABB(Hitbox& hb1, Hitbox& hb2, bool staticResolution = true);
	static bool CollideSAT(Hitbox& hb1, Hitbox& hb2, bool staticResolution = true);
	static bool CollideCircle(HitCircle * cir1, HitCircle * cir2, bool staticResolution = true);
	static bool CollideCircleAABB(HitCircle * cir, Hitbox& hitbox, bool staticResolution = true);
	static bool CollidePoint(glm::vec2 point, Hitbox& hitbox);
	static bool CollidePoint(glm::vec2 point, HitCircle * cir);

private:
	enum class CollisionDirction {
		RIGHT, LEFT, TOP, BOTTOM
	};

	static CollisionDirction colDir(glm::vec2 centerVector);

	CollisionHandler();
};

