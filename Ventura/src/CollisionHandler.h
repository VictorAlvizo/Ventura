#pragma once
#include "Circle.h"
#include "Camera.h"

class CollisionHandler {
public:
	//Collision for mainly two non-rotated rectangles. Fastest collision algorithm
	static bool CollideAABB(Hitbox& hb1, Hitbox& hb2, bool staticResolution = true);
	//Collisions between two convex (no interior angles is > 180) shapes that are rotated, very percise
	static bool CollideSAT(Hitbox& hb1, Hitbox& hb2, bool staticResolution = true);
	//Collision between two circles
	static bool CollideCircle(HitCircle * cir1, HitCircle * cir2, bool staticResolution = true);
	//Collision between a circle and a rectangle. Downside is that if the rectangle is rotated not as accurate
	static bool CollideCircleAABB(HitCircle * cir, Hitbox& hitbox, bool staticResolution = true);
	//Check if a point lies inside the object (mainly a rectangle)
	static bool CollidePoint(glm::vec2 point, Hitbox& hitbox, bool cameraFollowed, glm::vec2 cameraPos);
	//Check if a point lies inside the object (mainly a circle)
	static bool CollidePoint(glm::vec2 point, HitCircle * cir, bool cameraFollowed, glm::vec2 cameraPos);
	//Chcek if the object is still in the camera's view. Note that if wether you're drawing the object or not dosen't matter 
	static bool isInvisible(Camera& camera, Hitbox& hitbox);

private:
	enum class CollisionDirction {
		RIGHT, LEFT, TOP, BOTTOM
	};

	static CollisionDirction colDir(glm::vec2 centerVector);

	CollisionHandler();
};

