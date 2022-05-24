#pragma once

#include "engine/ecs/entity.h"

struct StatsComponent : public Component<StatsComponent> {
    float move_speed;
};

