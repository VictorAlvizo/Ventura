#include "CollisionHandler.h"

bool CollisionHandler::CollideAABB(Hitbox& hb1, Hitbox& hb2, bool staticResolution) {
    if (!hb1.m_ActiveHitbox || !hb2.m_ActiveHitbox) {
        return false;
    }

    bool xCol = hb1.getPos().x + hb1.getSize().x >= hb2.getPos().x
        && hb2.getPos().x + hb2.getSize().x >= hb1.getPos().x;

    bool yCol = hb1.getPos().y + hb1.getSize().y >= hb2.getPos().y
        && hb2.getPos().y + hb2.getSize().y >= hb1.getPos().y;

    if (xCol && yCol && staticResolution) {
        glm::vec2 hb1Center = glm::vec2(hb1.getPos().x + (hb1.getSize().x / 2), hb1.getPos().y + (hb1.getSize().y / 2));
        glm::vec2 hb2Center = glm::vec2(hb2.getPos().x + (hb2.getSize().x / 2), hb2.getPos().y + (hb2.getSize().y / 2));
        glm::vec2 centerVector = hb2Center - hb1Center;

        switch (CollisionHandler::colDir(centerVector)) {
        case CollisionDirction::RIGHT:
            hb1.Move(glm::vec2(hb2.getPos().x + hb2.getSize().x, hb1.getPos().y));
            break;

        case CollisionDirction::LEFT:
            hb1.Move(glm::vec2(hb2.getPos().x - hb1.getSize().x, hb1.getPos().y));
            break;

        case CollisionDirction::TOP:
            hb1.Move(glm::vec2(hb1.getPos().x, hb2.getPos().y - hb1.getSize().y));
            break;

        default:
            hb1.Move(glm::vec2(hb1.getPos().x, hb2.getPos().y + hb2.getSize().y));
            break;
        }
    }

    return xCol && yCol;
}

bool CollisionHandler::CollideSAT(Hitbox& hb1, Hitbox& hb2, bool staticResolution) {
    if (!hb1.m_ActiveHitbox || !hb2.m_ActiveHitbox) {
        return false;
    }

    std::vector<glm::vec2> axes;

    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(hb1.m_Rotation)), glm::sin(glm::radians(hb1.m_Rotation)))));
    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(hb1.m_Rotation + 90.0f)), glm::sin(glm::radians(hb1.m_Rotation + 90.0f)))));

    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(hb2.m_Rotation)), glm::sin(glm::radians(hb2.m_Rotation)))));
    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(hb2.m_Rotation + 90.0f)), glm::sin(glm::radians(hb2.m_Rotation + 90.0f)))));

    float e1Min = INFINITY, e1Max = -INFINITY;
    float e2Min = INFINITY, e2Max = -INFINITY;
    float overlap = INFINITY;

    std::vector<glm::vec2> corners1 = hb1.GetHitboxCorners();
    std::vector<glm::vec2> corners2 = hb2.GetHitboxCorners();

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
        glm::vec2 d = glm::vec2(hb1.getPos().x - hb2.getPos().x, hb2.getPos().y - hb1.getPos().y);
        float s = glm::sqrt(d.x * d.x + d.y * d.y);

        float moveX = hb1.getPos().x + overlap * d.x / s;
        float moveY = hb1.getPos().y - overlap * d.y / s;

        hb1.Move(glm::vec2(moveX, moveY));
    }

    return true;
}

bool CollisionHandler::CollideCircle(HitCircle * cir1, HitCircle * cir2, bool staticResolution) {
    if (!cir1->m_ActiveHitbox || !cir2->m_ActiveHitbox) {
        return false;
    }

    glm::vec2 center1 = glm::vec2(cir1->getPos() + cir1->getRadius());
    glm::vec2 center2 = glm::vec2(cir2->getPos() + cir2->getRadius());

    float distance = glm::length(center2 - center1);

    if (distance <= cir1->getRadius() + cir2->getRadius()) {
        if (staticResolution) {
            glm::vec2 centerVector = center2 - center1;
            float diff = cir1->getRadius() + cir2->getRadius() - distance;

            switch (CollisionHandler::colDir(centerVector)) {
                case CollisionDirction::RIGHT:
                    cir1->Move(glm::vec2(cir1->getPos().x + diff, cir1->getPos().y));
                    break;

                case CollisionDirction::LEFT:
                    cir1->Move(glm::vec2(cir1->getPos().x - diff, cir1->getPos().y));
                    break;

                case CollisionDirction::TOP:
                    cir1->Move(glm::vec2(cir1->getPos().x, cir1->getPos().y - diff));
                    break;

                default:
                    cir1->Move(glm::vec2(cir1->getPos().x, cir1->getPos().y + diff));
                    break;
            }
        }

        return true;
    }

    return false;
}

bool CollisionHandler::CollideCircleAABB(HitCircle * cir, Hitbox& hitbox, bool staticResolution) {
    if (!cir->m_ActiveHitbox || !hitbox.m_ActiveHitbox) {
        return false;
    }

    glm::vec2 circleCenter = glm::vec2(cir->getPos() + cir->getRadius());

    glm::vec2 entityHalfs = glm::vec2(hitbox.getSize().x / 2.0f, hitbox.getSize().y / 2.0f);
    glm::vec2 entityCenter = glm::vec2(hitbox.getPos().x + entityHalfs.x, hitbox.getPos().y + entityHalfs.y);

    //Difference vector between the two vectors
    glm::vec2 difference = circleCenter - entityCenter;
    glm::vec2 clamped = glm::clamp(difference, -entityHalfs, entityHalfs);
    //Position in the box closest to the circle
    glm::vec2 closest = entityCenter + clamped;

    difference = closest - circleCenter;

    if (glm::length(difference) < cir->getRadius()) {
        float diff = cir->getRadius() - glm::length(difference);

        switch (CollisionHandler::colDir(difference)) {
        case CollisionDirction::RIGHT:
            cir->Move(glm::vec2(cir->getPos().x + diff, cir->getPos().y));
            break;

        case CollisionDirction::LEFT:
            cir->Move(glm::vec2(cir->getPos().x - diff, cir->getPos().y));
            break;

        case CollisionDirction::TOP:
            cir->Move(glm::vec2(cir->getPos().x, cir->getPos().y - diff));
            break;

        default:
            cir->Move(glm::vec2(cir->getPos().x, cir->getPos().y + diff));
            break;
        }

        return true;
    }

    return false;
}

bool CollisionHandler::CollidePoint(glm::vec2 point, Hitbox& hitbox) {
    if (!hitbox.m_ActiveHitbox) {
        return false;
    }

    std::vector<glm::vec2> axes;

    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(hitbox.m_Rotation)), glm::sin(glm::radians(hitbox.m_Rotation)))));
    axes.push_back(glm::normalize(glm::vec2(glm::cos(glm::radians(hitbox.m_Rotation + 90.0f)), glm::sin(glm::radians(hitbox.m_Rotation + 90.0f)))));

    std::vector<glm::vec2> corners = hitbox.GetHitboxCorners();

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

bool CollisionHandler::CollidePoint(glm::vec2 point, HitCircle * cir) {
    glm::vec2 circleCenter = cir->getPos() + cir->getRadius();
    return glm::length(circleCenter - point) <= cir->getRadius() && cir->m_ActiveHitbox;
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
