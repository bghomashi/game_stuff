#pragma once

#include "engine/components/sprite_component.h"
#include "engine/components/transform_component.h"

class SpriteRender {
public:
    static void Update(float dt);
    static void Draw(float alpha);
};