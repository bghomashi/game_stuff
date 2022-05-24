#pragma once

#include "engine/ecs/entity.h"
#include "engine/utility/vector2.h"

struct TransformComponent : public Component<TransformComponent> {
    Vec2 position;
    float angle;
    Vec2 prev_position;
    float prev_angle;

    // some api
    void SetPosition(const Vec2& pos) {
        prev_position = position;
        position = pos;
    }
    void SetAngle(const float a) {
        prev_angle = angle;
        angle = a;
    }
};