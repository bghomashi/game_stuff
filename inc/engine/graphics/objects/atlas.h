#pragma once

#include <string>
#include "graphics.h"
#include "engine/utility/vector2.h"

class Atlas {
    OGL::Texture2D _texture;

    int _rows, _cols;
    Vec2 _texture_size, _tile_size;
public:
    Atlas();
    Atlas(const std::string& filename, const Vec2& tile_size);
    Atlas(OGL::Texture2D texture, const Vec2& tile_size, const Vec2& texture_size);


    ~Atlas();

    const Vec2& getTileSize() const;

    void Draw(int row, int col, const Vec2& pos, float scale = 1.f) const;
    void Draw(int index, const Vec2& pos, float scale = 1.f) const;
};
