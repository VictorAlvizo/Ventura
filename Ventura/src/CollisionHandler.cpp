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

        switch (CollisionHandler::colDir(centerVector)) {
            case CollisionDirction::RIGHT:
                ent.MoveHitbox(glm::vec2(ent2.getHitboxPos().x + ent2.getHitboxSize().x, ent.getHitboxPos().y));
                break;

            case CollisionDirction::LEFT:
                ent.MoveHitbox(glm::vec2(ent2.getHitboxPos().x - ent.getHitboxSize().x, ent.getHitboxPos().y));
                break;
               
            case CollisionDirction::TOP:
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

bool CollisionHandler::CollideCircle(Circle * cir1, Circle * cir2, bool staticResolution) {
    glm::vec2 center1 = glm::vec2(cir1->getHitboxPos() + cir1->getHitboxRadius());
    glm::vec2 center2 = glm::vec2(cir2->getHitboxPos() + cir2->getHitboxRadius());

    float distance = glm::length(center2 - center1);

    if (distance <= cir1->getHitboxRadius() + cir2->getHitboxRadius()) {
        if (staticResolution) {
            glm::vec2 centerVector = center2 - center1;
            float diff = cir1->getHitboxRadius() + cir2->getHitboxRadius() - distance;

            switch (CollisionHandler::colDir(centerVector)) {
                case CollisionDirction::RIGHT:
                    cir1->MoveHitbox(glm::vec2(cir1->getHitboxPos().x + diff, cir1->getHitboxPos().y));
                    break;

                case CollisionDirction::LEFT:
                    cir1->MoveHitbox(glm::vec2(cir1->getHitboxPos().x - diff, cir1->getHitboxPos().y));
                    break;

                case CollisionDirction::TOP:
                    cir1->MoveHitbox(glm::vec2(cir1->getHitboxPos().x, cir1->getHitboxPos().y - diff));
                    break;

                default:
                    cir1->MoveHitbox(glm::vec2(cir1->getHitboxPos().x, cir1->getHitboxPos().y + diff));
                    break;
            }
        }

        return true;
    }

    return false;
}

bool CollisionHandler::CollideCircleAABB(Circle * cir, Entity& ent, bool staticResolution) {
    glm::vec2 circleCenter = glm::vec2(cir->getHitboxPos() + cir->getHitboxRadius());

    glm::vec2 entityHalfs = glm::vec2(ent.getHitboxSize().x / 2.0f, ent.getHitboxSize().y / 2.0f);
    glm::vec2 entityCenter = glm::vec2(ent.getHitboxPos().x + entityHalfs.x, ent.getHitboxPos().y + entityHalfs.y);

    //Difference vector between the two vectors
    glm::vec2 difference = circleCenter - entityCenter;
    glm::vec2 clamped = glm::clamp(difference, -entityHalfs, entityHalfs);
    //Position in the box closest to the circle
    glm::vec2 closest = entityCenter + clamped;

    difference = closest - circleCenter;

    if (glm::length(difference) < cir->getHitboxRadius()) {
        float diff = cir->getRadius() - glm::length(difference);

        switch (CollisionHandler::colDir(difference)) {
            case CollisionDirction::RIGHT:
                cir->MoveHitbox(glm::vec2(cir->getHitboxPos().x + diff, cir->getHitboxPos().y));
                break;

            case CollisionDirction::LEFT:
                cir->MoveHitbox(glm::vec2(cir->getHitboxPos().x - diff, cir->getHitboxPos().y));
                break;

        case CollisionDirction::TOP:
                cir->MoveHitbox(glm::vec2(cir->getHitboxPos().x, cir->getHitboxPos().y - diff));
                break;

            default:
                cir->MoveHitbox(glm::vec2(cir->getHitboxPos().x, cir->getHitboxPos().y + diff));
                break;
        }

        return true;
    }

    return false;
}

bool CollisionHandler::CollidePoint(glm::vec2 point, Entity& ent) {
    std::vector<glm::vec2> axes;

    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(ent.m_Rotation)), glm::sin(glm::radians(ent.m_Rotation)))));
    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(ent.m_Rotation + 90.0f)), glm::sin(glm::radians(ent.m_Rotation + 90.0f)))));

    std::vector<glm::vec2> corners = ent.GetHitboxCorners();

    float eMin = INFINITY, eMax = -INFINITY;
    float pointProduct;

    for (glm::vec2 axis : axes) {
        eMin = INFINITY;
        eMax = -INFINITY;

        //Don't have to reset everytime, as it's just one value
        pointProduct = glm::dot(point, axis);

        for (unsigned int i = 0; i < corners.size(); i++) {
            float product = glm::dot(corners[i], axis);
            eMax = glm::max(eMax, product);
            eMin = glm::min(eMin, product);
        }

        if (eMax < pointProduct || eMin > pointProduct) {
            return false;
        }
    }

    return true;
}

bool CollisionHandler::CollidePoint(glm::vec2 point, Circle * cir) {
    glm::vec2 circleCenter = cir->getHitboxPos() + cir->getHitboxRadius();
    return glm::length(circleCenter - point) <= cir->getHitboxRadius();
}

CollisionHandler::CollisionDirction CollisionHandler::colDir(glm::vec2 centerVector) {
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

    return static_cast<CollisionDirction>(index);
}
