#include "CollisionHandler.h"

bool CollisionHandler::CollideAABB(Entity& ent, Entity& ent2) {
    bool xCol = ent.getPos().x + ent.getSize().x >= ent2.getPos().x
        && ent2.getPos().x + ent2.getSize().x >= ent.getPos().x;

    bool yCol = ent.getPos().y + ent.getSize().y >= ent2.getPos().y
        && ent2.getPos().y + ent2.getSize().y >= ent.getPos().y;

    return xCol && yCol;
}

bool CollisionHandler::CollideSAT(Entity& ent, Entity& ent2) {
    std::vector<glm::ivec2> corners1 = getCorners(ent);
    std::vector<glm::ivec2> corners2 = getCorners(ent2);

    //TODO: Get axes and find min and max for each axis

    return false;
}

std::vector<glm::ivec2> CollisionHandler::getCorners(Entity& entity) {
    std::vector<glm::ivec2> corners;

    //TODO: So far only for a rectangle / square. Implment corners within entity itself?
    corners.push_back({ entity.getPos().x, entity.getPos().y }); //Top left
    corners.push_back({ entity.getPos().x + entity.getSize().x, entity.getPos().y }); //Top right
    corners.push_back({ entity.getPos().x, entity.getPos().y + entity.getSize().y }); //Bottom left
    corners.push_back({ entity.getPos().x + entity.getSize().x, entity.getPos().y + entity.getSize().y }); //Bottom right

    return corners;
}
