
#include <iostream>
#include <fstream>
#include "engine/graphics/objects/sprite.h"
#include "engine/utility/json.hpp"
#include "engine/utility/logger.h"

static std::string ToLower(std::string data) {
    std::transform(data.begin(), data.end(), data.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return data;
}

static bool ValidateImageJson(const nlohmann::json& image_file);
static bool ValidateAnimationJson(const nlohmann::json& animation);

bool SpriteResource::Load(const std::string& filename) {
    std::ifstream i(filename);
    if (!i.is_open()) return false;

    nlohmann::json input;
    i >> input;

    // validate
    if (!input.contains("resource_type") || !input["resource_type"].is_string()) {
        LOG_CRITICAL("\"resource_type\" is not a string.");
        return false;
    }
    if (ToLower(input["resource_type"]) != "sprite") return false;
    
    if (!input.contains("default_animation") || !input["default_animation"].is_string()) {
        LOG_CRITICAL("sprite resource does not contain \"default_animation\" string.");
        return false;
    }
    _default_animation = input["default_animation"];

    _scale = 1.;
    if (input.contains("scale") && input["scale"].is_number()) 
        _scale = input["scale"];
    
    if (!input.contains("image_files") || !input["image_files"].is_array()) {
        LOG_CRITICAL("sprite resource does not contain \"image_files\" array.");
        return false;
    }
    _atlases.reserve(input["image_files"].size());
    _atlas_names.reserve(input["image_files"].size());
    for (auto& image_file : input["image_files"]) {
        if (!ValidateImageJson(image_file))
            return false;


        Vec2 tile_size;
        std::string name = image_file["name"];
        std::string file = image_file["image"];
        tile_size.x = image_file["tile_size"][0];
        tile_size.y = image_file["tile_size"][1];

        _atlases.emplace_back(file, tile_size);
        _atlas_names.push_back(name);
    }

    _animations.reserve(input["animations"].size());
    _animation_names.reserve(input["animations"].size());
    for (auto& animation : input["animations"]) {
        if (!ValidateAnimationJson(animation))
            return false;

        iVec2 start_frame;
        bool loop = false;
        std::string fallback;
        float pixels_per_frame = 0;
        int num_frames = animation["num_frames"];
        float frame_time = animation["frame_time"];
        std::string name = animation["name"];
        std::string image_name = animation["image_name"];
        start_frame.y = animation["start_frame"][0];            // row
        start_frame.x = animation["start_frame"][1];            // col
        if (animation.contains("fallback"))
            fallback = animation["fallback"];
        if (animation.contains("loop"))
            loop = animation["loop"];
        if (animation.contains("pixels_per_frame"))
            pixels_per_frame = _scale*animation["pixels_per_frame"].get<float>();


        _animations.push_back({image_name, frame_time, num_frames, start_frame, loop, fallback, pixels_per_frame});
        _animation_names.push_back(name);
    }

    return true;
}
void SpriteResource::Destroy() {

}

void SpriteResource::Draw(const std::string& atlas_name, int row, int col, const Vec2& pos) const {
    auto it = std::find(_atlas_names.begin(), _atlas_names.end(), atlas_name);
    if (it != _atlas_names.end()) {
        int index = (it - _atlas_names.begin());
        _atlases[index].Draw(row, col, pos, _scale);
    }
}
void SpriteResource::Draw(const std::string& atlas_name, int tile_index, const Vec2& pos) const {
    auto it = std::find(_atlas_names.begin(), _atlas_names.end(), atlas_name);
    if (it != _atlas_names.end()) {
        int index = (it - _atlas_names.begin());
        _atlases[index].Draw(tile_index, pos, _scale);
    }
}
const std::string& SpriteResource::GetDefaultAnimation() const {
    return _default_animation;
}
const SpriteResource::Animation* SpriteResource::GetAnimation(const std::string& name) const {
    auto it = std::find(_animation_names.begin(), _animation_names.end(), name);
    if (it != _animation_names.end()) {
        int index = (it - _animation_names.begin());
        return &_animations[index];
    } 
    return NULL;
}





static bool ValidateImageJson(const nlohmann::json& image_file) {
    if (!image_file.contains("name") || !image_file["name"].is_string()) {
        LOG_CRITICAL("sprite image file requires string \"name\".");
        return false;
    }
    if (!image_file.contains("image") || !image_file["image"].is_string()) {
        LOG_CRITICAL("sprite image file requires string \"image\".");
        return false;
    }
    if (!image_file.contains("tile_size") || !image_file["tile_size"].is_array()) {
        LOG_CRITICAL("sprite image file requires vector \"tile_size\".");
        return false;
    }
    return true;
}

static bool ValidateAnimationJson(const nlohmann::json& animation) {
    if (!animation.contains("name") || !animation["name"].is_string()) {
        LOG_CRITICAL("sprite animation requires string \"name\".");
        return false;
    }
    if (!animation.contains("image_name") || !animation["image_name"].is_string()) {
        LOG_CRITICAL("sprite animation requires string \"image_name\".");
        return false;
    }
    if (!animation.contains("start_frame") || !animation["start_frame"].is_array()) {
        LOG_CRITICAL("sprite animation requires vector \"start_frame\".");
        return false;
    }
    if (!animation.contains("num_frames") || !animation["num_frames"].is_number()) {
        LOG_CRITICAL("sprite animation requires vector \"num_frames\".");
        return false;
    }
    if (!animation.contains("frame_time") || !animation["frame_time"].is_number()) {
        LOG_CRITICAL("sprite animation requires vector \"frame_time\".");
        return false;
    }
    
    return true;
}