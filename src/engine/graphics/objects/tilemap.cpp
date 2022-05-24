#include <iostream>
#include <fstream>
#include "graphics/objects/tilemap.h"
#include "utility/json.hpp"
#include "utility/logger.h"

static std::string ToLower(std::string data) {
    std::transform(data.begin(), data.end(), data.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return data;
}

static bool ValidateImageJson(const nlohmann::json& image_file);
static bool ValidateLayerJson(const nlohmann::json& image_file);


bool TileMapResource::Load(const std::string& filename) {
    std::ifstream i(filename);
    if (!i.is_open()) return false;

    nlohmann::json input;
    i >> input;

    // validate
    if (!input.contains("resource_type") || !input["resource_type"].is_string()) {
        LOG_CRITICAL("\"resource_type\" is not a string.");
        return false;
    }
    if (ToLower(input["resource_type"]) != "tilemap") return false;
    
    if (!input.contains("tile_size") || !input["tile_size"].is_array()) {
        LOG_CRITICAL("tilemap resource does not contain \"tile_size\" array.");
        return false;
    }

    if (!input.contains("grid_dim") || !input["grid_dim"].is_array()) {
        LOG_CRITICAL("tilemap resource does not contain \"grid_dim\" array.");
        return false;
    }
    
    if (!input.contains("image_files") || !input["image_files"].is_array()) {
        LOG_CRITICAL("tilemap resource does not contain \"image_files\" array.");
        return false;
    }

    if (!input.contains("layers") || !input["layers"].is_array()) {
        LOG_CRITICAL("tilemap resource does not contain \"layers\" array.");
        return false;
    }
    _tile_half_size.x = 0.5f*input["tile_size"][0].get<float>();
    _tile_half_size.y = 0.5f*input["tile_size"][1].get<float>();

    _atlases.reserve(input["image_files"].size());
    _atlas_names.reserve(input["image_files"].size());
    _atlas_offset.reserve(input["image_files"].size());
    _atlas_sprite_size.reserve(input["image_files"].size());
    _atlas_tile_scale.reserve(input["image_files"].size());
    for (auto& image_file : input["image_files"]) {
        if (!ValidateImageJson(image_file))
            return false;


        Vec2 tile_size, sprite_size, sprite_offset = {0,0};
        std::string name = image_file["name"];
        std::string file = image_file["image"];
        tile_size.x = image_file["tile_size"][0];
        tile_size.y = image_file["tile_size"][1];
        sprite_size.x = image_file["sprite_size"][0];
        sprite_size.y = image_file["sprite_size"][1];
        Vec2 scale = 2.f*Vec2{_tile_half_size.x/sprite_size.x, _tile_half_size.y/sprite_size.y};
        if (image_file.contains("sprite_offset")) {
            sprite_offset.x = image_file["sprite_offset"][0].get<float>()*scale.x;
            sprite_offset.y = image_file["sprite_offset"][1].get<float>()*scale.y;
        }

        _atlases.emplace_back(file, tile_size);
        _atlas_names.push_back(name);
        _atlas_offset.push_back(sprite_offset);
        _atlas_sprite_size.push_back(sprite_size);
        _atlas_tile_scale.push_back(scale.x);                   // scale.x shou
    }

    _grid_rows = input["grid_dim"][0];
    _grid_cols = input["grid_dim"][1];

    _layers.reserve(input["layers"].size());
    _layer_names.reserve(input["layers"].size());
    _layer_images.reserve(input["layers"].size());

    for (auto& layer : input["layers"]) {
        if (!ValidateLayerJson(layer))
            return false;

        Vec2 tile_size, sprite_size, sprite_offset = {0,0};
        std::string name = layer["name"];
        std::string image = layer["image_file"];
        auto grid = layer["grid"];
        
        _layer_names.push_back(name);
        _layer_images.push_back(image);
        _layers.emplace_back(_grid_rows, _grid_cols);
        int* i = &(_layers.back()(0,0));
        for (auto& ele : grid) {
            *(i++) = ele;
        }

    }


    return true;
}
void TileMapResource::Destroy() {

}

void TileMapResource::Draw(const std::string& atlas_name, int row, int col, const Vec2& pos) const {
    auto it = std::find(_atlas_names.begin(), _atlas_names.end(), atlas_name);
    if (it != _atlas_names.end()) {
        int index = (it - _atlas_names.begin());
        OGL::MatrixStack::PushModelView();
        OGL::MatrixStack::TranslateModelView(_atlas_offset[index].x, _atlas_offset[index].y, 0);

        _atlases[index].Draw(row, col, pos, _atlas_tile_scale[index]);

        OGL::MatrixStack::PopModelView();
    }
}
void TileMapResource::Draw(const std::string& atlas_name, int tile_index, const Vec2& pos) const {
    auto it = std::find(_atlas_names.begin(), _atlas_names.end(), atlas_name);
    if (it != _atlas_names.end()) {
        int index = (it - _atlas_names.begin());
        OGL::MatrixStack::PushModelView();
        OGL::MatrixStack::TranslateModelView(_atlas_offset[index].x, _atlas_offset[index].y, 0);
        
        _atlases[index].Draw(tile_index, pos, _atlas_tile_scale[index]);
        
        OGL::MatrixStack::PopModelView();
    }
}
void TileMapResource::DrawGrid(const Vec2& pos) const {
    OGL::MatrixStack::PushModelView();
    OGL::MatrixStack::TranslateModelView(pos.x, pos.y, 0);

    Vec2 screen = {0, 0};
    for (int i = 0; i < _grid_cols; i++) {
        for (int j = 0; j < _grid_rows; j++) {
            screen.x = (j - i) * _tile_half_size.x;
            screen.y = -(j + i) * _tile_half_size.y;

            OGL::Draw::Line(screen+Vec2{-_tile_half_size.x, 0}, screen+Vec2{0,-_tile_half_size.y}, OGL::Color::Black);
            OGL::Draw::Line(screen+Vec2{-_tile_half_size.x, 0}, screen+Vec2{0, _tile_half_size.y}, OGL::Color::Black);
            OGL::Draw::Line(screen+Vec2{0, _tile_half_size.y}, screen+Vec2{_tile_half_size.x, 0}, OGL::Color::Black);
            OGL::Draw::Line(screen+Vec2{0,-_tile_half_size.y}, screen+Vec2{_tile_half_size.x, 0}, OGL::Color::Black);

            // OGL::Draw::Line(screen+Vec2{0, -_tile_half_size.y}, screen+Vec2{0, _tile_half_size.y}, OGL::Color::Black);
            // OGL::Draw::Line(screen+Vec2{-_tile_half_size.x, 0}, screen+Vec2{_tile_half_size.x, 0}, OGL::Color::Black);

            OGL::Draw::SquareFilled(screen, {1., 1.}, OGL::Color::Yellow);
        }
    }

    OGL::MatrixStack::PopModelView();
}
void TileMapResource::DrawLayer(int layer_idx, const Vec2& pos) const {
    OGL::MatrixStack::PushModelView();
    OGL::MatrixStack::TranslateModelView(pos.x, pos.y, 0);

    Vec2 screen = {0, 0};
    auto& layer = _layers[layer_idx];
    for (int i = 0; i < _grid_cols; i++) {
        for (int j = 0; j < _grid_rows; j++) {
            screen.x = (j - i) * _tile_half_size.x;
            screen.y = -(j + i) * _tile_half_size.y;

            Draw(_layer_images[layer_idx], layer(i,j), screen);
        }
    }

    OGL::MatrixStack::PopModelView();
}



static bool ValidateImageJson(const nlohmann::json& image_file) {
    if (!image_file.contains("name") || !image_file["name"].is_string()) {
        LOG_CRITICAL("tilemap image file requires string \"name\".");
        return false;
    }
    if (!image_file.contains("image") || !image_file["image"].is_string()) {
        LOG_CRITICAL("tilemap image file requires string \"image\".");
        return false;
    }
    if (!image_file.contains("tile_size") || !image_file["tile_size"].is_array()) {
        LOG_CRITICAL("tilemap image file requires vector \"tile_size\".");
        return false;
    }
    if (!image_file.contains("sprite_size") || !image_file["sprite_size"].is_array()) {
        LOG_CRITICAL("tilemap image file requires vector \"sprite_size\".");
        return false;
    }
    if (image_file.contains("sprite_offset") && !image_file["sprite_offset"].is_array()) {
        LOG_CRITICAL("optional element \"sprite_offset\" must be a vector.");
        return false;
    }
    return true;
}

static bool ValidateLayerJson(const nlohmann::json& image_file) {
    if (!image_file.contains("name") || !image_file["name"].is_string()) {
        LOG_CRITICAL("tilemaplayer requires string \"name\".");
        return false;
    }
    if (!image_file.contains("image_file") || !image_file["image_file"].is_string()) {
        LOG_CRITICAL("tilemap layer requires string \"image_file\".");
        return false;
    }
    if (!image_file.contains("grid") || !image_file["grid"].is_array()) {
        LOG_CRITICAL("tilemap layer requires array \"grid\".");
        return false;
    }
    return true;
}