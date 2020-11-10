#include "CollisionHandler.h"

bool CollisionHandler::CollideAABB(Entity& ent, Entity& ent2) {
    bool xCol = ent.getPos().x + ent.getSize().x >= ent2.getPos().x
        && ent2.getPos().x + ent2.getSize().x >= ent.getPos().x;

    bool yCol = ent.getPos().y + ent.getSize().y >= ent2.getPos().y
        && ent2.getPos().y + ent2.getSize().y >= ent.getPos().y;

    return xCol && yCol;
}

bool CollisionHandler::CollideSAT(Entity& ent, Entity& ent2) { //FIXME: SAT collision not correct
    std::vector<glm::vec2> axes;

    axes.push_back(glm::vec2(glm::cos(glm::radians(ent.m_Rotation)), glm::sin(glm::radians(ent.m_Rotation))));
    axes.push_back(glm::vec2(glm::cos(glm::radians(ent.m_Rotation + 90.0f)), glm::sin(glm::radians(ent.m_Rotation + 90.0f))));

    axes.push_back(glm::vec2(glm::cos(glm::radians(ent2.m_Rotation)), glm::sin(glm::radians(ent2.m_Rotation))));
    axes.push_back(glm::vec2(glm::cos(glm::radians(ent2.m_Rotation + 90.0f)), glm::sin(glm::radians(ent2.m_Rotation + 90.0f))));

    float e1Min = INFINITY, e1Max = -INFINITY;
    float e2Min = INFINITY, e2Max = -INFINITY;

    std::vector<glm::vec2> corners1 = ent.GetCorners();
    std::vector<glm::vec2> corners2 = ent2.GetCorners();

    for (glm::vec2 axis : axes) {
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

bool CollisionHandler::CollideRadius(Entity& ent, Entity& ent2) {
    glm::vec2 center1 = glm::vec2(ent.getPos().x + (ent.getSize().x / 2.0f), ent.getPos().y + (ent.getSize().y / 2.0f));
    glm::vec2 center2 = glm::vec2(ent2.getPos().x + (ent2.getSize().x / 2.0f), ent2.getPos().y + (ent2.getSize().y / 2.0f));

    //0.97 and 0.94 are breaking points, switch to a diagnol radius
    float centerDot = glm::abs(glm::dot(glm::normalize(center1), glm::normalize(center2)));
    //std::cout << centerDot << std::endl;

    float rad1 = 0.0f, rad2 = 0.0f;
    if (centerDot >= 0.97f || centerDot <= 0.94f) {
        rad1 = glm::length(center1 - glm::vec2(ent.getPos().x, ent.getPos().y));
        rad2 = glm::length(center2 - glm::vec2(ent2.getPos().x, ent2.getPos().y));
    }
    else {
        rad1 = glm::length(center1 - glm::vec2(ent.getPos().x, ent.getPos().y));
        rad2 = glm::length(center2 - glm::vec2(ent2.getPos().x, ent2.getPos().y));
    }

    //Distance between the two centers, must be positive
    float distance = glm::abs(glm::length(center2 - center1));
    return distance <= rad1 + rad2;
}
