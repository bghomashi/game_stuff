#include "graphics/objects/atlas.h"
#include "utility/image.h"
#include "utility/logger.h"

#include <iostream>

Atlas::Atlas() : _texture_size({-1,-1}), _tile_size({-1,-1}) {
}
Atlas::Atlas(const std::string& filename, const Vec2& tile_size) : _texture_size({-1,-1}), _tile_size(tile_size) {
    Image image;
    if (!image.Load(filename)) {
        LOG_CRITICAL("Failed to load image \"" + filename +"\"");
        return;
    }

    _texture_size.x = image.width;
    _texture_size.y = image.height;
    // std::cout << image.num_channels << std::endl;

    _cols = _texture_size.x / _tile_size.x;
    _rows = _texture_size.y / _tile_size.y;

    _texture.Create();
    _texture.Bind(0);
    _texture.Data(  OGL::InternalFormat::RGBA, 
                    image.width, image.height,
                    OGL::PixelFormat::RGBA,
                    OGL::PixelType::Unsigned_Byte,
                    image.data);
    _texture.SetMinFilter(OGL::Filter::Nearest);
    _texture.SetMagFilter(OGL::Filter::Nearest);
    _texture.SetWrapping(OGL::Wrapping::Clamp_To_Edge);
    
    _texture.Unbind(0);
    
    image.Free();
}
Atlas::Atlas(OGL::Texture2D texture, const Vec2& tile_size, const Vec2& texture_size)  : _texture_size(texture_size), _tile_size(tile_size), _texture(texture) {
}
Atlas::~Atlas() {
    _texture.Destroy();
}

const Vec2& Atlas::getTileSize() const {
    return _tile_size;
}

void Atlas::Draw(int row, int col, const Vec2& pos, float scale) const {
    float x = col*_tile_size.x;
    float y = row*_tile_size.y;

    OGL::Draw::SquarePartialTexture(pos, scale*_tile_size, {x, y}, _tile_size, _texture);
}
void Atlas::Draw(int index, const Vec2& pos, float scale) const {
    float x = (index % _cols)*_tile_size.x;
    float y = (index / _cols)*_tile_size.y;

    // std::cout << _rows << " " << _cols << std::endl;
    // std::cout << _texture_size.x << " " << _texture_size.y << std::endl;
    // std::cout << _tile_size.x << " " << _tile_size.y << std::endl;

    OGL::Draw::SquarePartialTexture(pos, scale*_tile_size, {x, y}, _tile_size, _texture);
    // OGL::Draw::SquareTexture(pos, _tile_size*0.5f, _texture);
}