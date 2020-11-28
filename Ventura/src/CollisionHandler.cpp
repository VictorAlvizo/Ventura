#include "CollisionHandler.h"

bool CollisionHandler::CollideAABB(Entity& ent, Entity& ent2, bool staticResolution) {
    bool xCol = ent.getHitboxPos().x + ent.getHitboxSize().x >= ent2.getHitboxPos().x
        && ent2.getHitboxPos().x + ent2.getHitboxSize().x >= ent.getHitboxPos().x;

    bool yCol = ent.getHitboxPos().y + ent.getHitboxSize().y >= ent2.getHitboxPos().y
        && ent2.getHitboxPos().y + ent2.getHitboxSize().y >= ent.getHitboxPos().y;

    //Static resolution
    if (xCol && yCol && staticResolution) {
        glm::vec2 hb1Center = glm::vec2(ent.getHitboxPos().x + (ent.getHitboxSize().x / 2), ent.getHitboxPos().y + (ent.getHitboxSize().y / 2));
        glm::vec2 hb2Center = glm::vec2(ent2.getHitboxPos().x + (ent2.getHitboxSize().x / 2), ent2.getHitboxPos().y + (ent.getHitboxSize().y / 2));
        glm::vec2 centerVector = hb2Center - hb1Center;

        glm::vec2 directions[4] = {
            glm::vec2(1.0f, 0.0f), //Right
            glm::vec2(-1.0f, 0.0f), //Left
            glm::vec2(0.0f, -1.0f), //Top
            glm::vec2(0.0f, 1.0f) //Bottom
        };

        float closest = 0.0f;
        int index = 0;
        for (int i = 0; i < 4; i++) {
            float product = glm::dot(directions[i], glm::normalize(centerVector));

            if (closest > product) {
                closest = product;
                index = i;
            }
        }
        
        switch (index) {
            case 0:
                ent.MoveHitbox(glm::vec2(ent2.getHitboxPos().x + ent2.getHitboxSize().x, ent.getHitboxPos().y));
                break;

            case 1:
                ent.MoveHitbox(glm::vec2(ent2.getHitboxPos().x - ent.getHitboxSize().x, ent.getHitboxPos().y));
                break;
               
            case 2:
                ent.MoveHitbox(glm::vec2(ent.getHitboxPos().x, ent2.getHitboxPos().y - ent.getHitboxSize().y));
                break;

            default:
                ent.MoveHitbox(glm::vec2(ent.getHitboxPos().x, ent2.getHitboxPos().y + ent2.getHitboxSize().y));
                break;
        }
    }

    return xCol && yCol;
}

bool CollisionHandler::CollideSAT(Entity& ent, Entity& ent2, bool staticResolution) {
    std::vector<glm::vec2> axes;

    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(ent.m_Rotation)), glm::sin(glm::radians(ent.m_Rotation)))));
    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(ent.m_Rotation + 90.0f)), glm::sin(glm::radians(ent.m_Rotation + 90.0f)))));

    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(ent2.m_Rotation)), glm::sin(glm::radians(ent2.m_Rotation)))));
    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(ent2.m_Rotation + 90.0f)), glm::sin(glm::radians(ent2.m_Rotation + 90.0f)))));

    float e1Min = INFINITY, e1Max = -INFINITY;
    float e2Min = INFINITY, e2Max = -INFINITY;
    float overlap = INFINITY;

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

        //Store the minimum overlap
        overlap = glm::min(glm::min(e1Max, e2Max) - glm::max(e1Min, e2Min), overlap);

        if (e1Max < e2Min || e2Max < e1Min) {
            return false;
        }
    }

    if (staticResolution) {
        glm::vec2 d = glm::vec2(ent.getHitboxPos().x - ent2.getHitboxPos().x, ent2.getHitboxPos().y - ent.getHitboxPos().y);
        float s = glm::sqrt(d.x * d.x + d.y * d.y);

        float moveX = ent.getHitboxPos().x - overlap * d.x / s;
        float moveY = ent.getHitboxPos().y - overlap * d.y / s;

        ent.MoveHitbox(glm::vec2(moveX, moveY));
    }

    return true;
}
