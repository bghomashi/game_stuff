#pragma once

#include "engine/utility/vector2.h"
#include "engine/utility/rectangle.h"

struct Camera {
    Vec2 position;
    iRectangle viewport;

    void Draw() const;
    void SetViewport() const;
};
