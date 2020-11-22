#include "CollisionHandler.h"

bool CollisionHandler::CollideAABB(Entity& ent, Entity& ent2) {
    bool xCol = ent.getHitboxPos().x + ent.getHitboxSize().x >= ent2.getHitboxPos().x
        && ent2.getHitboxPos().x + ent2.getHitboxSize().x >= ent.getHitboxPos().x;

    bool yCol = ent.getHitboxPos().y + ent.getHitboxSize().y >= ent2.getHitboxPos().y
        && ent2.getHitboxPos().y + ent2.getHitboxSize().y >= ent.getHitboxPos().y;

    return xCol && yCol;
}

bool CollisionHandler::CollideSAT(Entity& ent, Entity& ent2) {
    std::vector<glm::vec2> axes;

    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(ent.m_Rotation)), glm::sin(glm::radians(ent.m_Rotation)))));
    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(ent.m_Rotation + 90.0f)), glm::sin(glm::radians(ent.m_Rotation + 90.0f)))));

    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(ent2.m_Rotation)), glm::sin(glm::radians(ent2.m_Rotation)))));
    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(ent2.m_Rotation + 90.0f)), glm::sin(glm::radians(ent2.m_Rotation + 90.0f)))));

    float e1Min = INFINITY, e1Max = -INFINITY;
    float e2Min = INFINITY, e2Max = -INFINITY;

    std::vector<glm::vec2> corners1 = ent.GetHitboxCorners();
    std::vector<glm::vec2> corners2 = ent2.GetHitboxCorners();

    for (glm::vec2 axis : axes) {
        e1Min = INFINITY;
        e2Min = INFINITY;
        e1Max = -INFINITY;
        e2Max = -INFINITY;

        //First entity corners
        for (unsigned int i = 0; i < corners1.size(); i++) {
            float product = glm::dot(corners1[i], axis);
            e1Min = glm::min(e1Min, product);
            e1Max = glm::max(e1Max, product);
        }

        //Second entity corners
        for (unsigned int i = 0; i < corners2.size(); i++) {
            float product = glm::dot(corners2[i], axis);
            e2Min = glm::min(e2Min, product);
            e2Max = glm::max(e2Max, product);
        }

        if (e1Max < e2Min || e2Max < e1Min) {
            return false;
        }
    }

    return true;
}
