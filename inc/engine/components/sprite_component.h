#pragma once

#include "engine/ecs/entity.h"
#include "engine/graphics/objects/sprite.h"

struct SpriteComponent : public Component<SpriteComponent> {
    Sprite sprite;
    ComponentID transform_handle;
};

