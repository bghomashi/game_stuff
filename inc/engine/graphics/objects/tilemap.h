#pragma once


#include "graphics/objects/atlas.h"
#include <memory>

template <typename T>
class Grid2D {
    std::vector<T> _data;
    int _rows, _cols;
public:
    Grid2D() : _rows(0), _cols(0) {}
    Grid2D(int rows, int cols) : _rows(rows), _cols(cols), _data(rows*cols) {}

    inline int Rows() const {
        return _rows;
    }
    inline int Cols() const {
        return _cols;
    }
    T& operator() (int row, int col) {
        return _data[col + row*_cols];
    }
    const T& operator() (int row, int col) const {
        return _data[col + row*_cols];
    }
};

class TileMapResource {
    std::vector<std::string> _atlas_names;
    std::vector<Vec2> _atlas_offset;
    std::vector<Vec2> _atlas_sprite_size;
    std::vector<float> _atlas_tile_scale;
    std::vector<Atlas> _atlases;

    int _grid_rows, _grid_cols;
    Vec2 _tile_half_size;
    std::vector<Grid2D<int>> _layers;
    std::vector<std::string> _layer_names;
    std::vector<std::string> _layer_images;
public:
    typedef std::shared_ptr<TileMapResource> Ptr_t;
    
    bool Load(const std::string& filename);
    void Destroy();


    void Draw(const std::string& atlas_name, int row, int col, const Vec2& pos) const;
    void Draw(const std::string& atlas_name, int tile_index, const Vec2& pos) const;
    
    void DrawLayer(int layer, const Vec2& pos) const;
    void DrawGrid(const Vec2& pos) const;
};

class TileMap {

};