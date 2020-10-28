#include "CollisionHandler.h"

bool CollisionHandler::CollideAABB(Entity& ent, Entity& ent2) {
    bool xCol = ent.getPos().x + ent.getSize().x >= ent2.getPos().x
        && ent2.getPos().x + ent2.getSize().x >= ent.getPos().x;

    bool yCol = ent.getPos().y + ent.getSize().y >= ent2.getPos().y
        && ent2.getPos().y + ent2.getSize().y >= ent.getPos().y;

    return xCol && yCol;
}

bool CollisionHandler::CollideSAT(Entity& ent, Entity& ent2) {
    //Get corners, shadows, etc SAT code

    return false;
}
