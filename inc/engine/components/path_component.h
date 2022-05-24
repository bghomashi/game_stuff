#pragma once

#include "engine/ecs/entity.h"
#include "utility/vector2.h"
#include <queue>

struct PathComponent : Component<PathComponent> {
    std::queue<Vec2> points;
};