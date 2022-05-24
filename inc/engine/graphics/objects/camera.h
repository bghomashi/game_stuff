#pragma once

#include "utility/vector2.h"
#include "utility/rectangle.h"

struct Camera {
    Vec2 position;
    iRectangle viewport;

    void Draw() const;
    void SetViewport() const;
};
