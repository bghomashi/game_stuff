#pragma once


#include "engine/graphics/objects/atlas.h"
#include <memory>


class SpriteResource {
public:
    struct Animation {
        std::string image_name;
        float frame_time;
        int num_frames;
        iVec2 start_frame;
        bool loop;
        std::string fallback;
        float pixels_per_frame;
    };
protected:
    // atlases
    std::vector<std::string> _atlas_names;
    std::vector<Atlas> _atlases;
    float _scale;

    // animations
    std::string _default_animation;
    std::vector<std::string> _animation_names;
    std::vector<Animation> _animations;
public:
    typedef std::shared_ptr<SpriteResource> Ptr_t;
    
    bool Load(const std::string& filename);
    void Destroy();

    const std::string& GetDefaultAnimation() const;
    const Animation* GetAnimation(const std::string& name) const;

    void Draw(const std::string& atlas_name, int row, int col, const Vec2& pos) const;
    void Draw(const std::string& atlas_name, int tile_index, const Vec2& pos) const;
};

class Sprite {
    struct AnimationInstance {
        const SpriteResource::Animation* anim;

        int frame;
        float t;
        float frame_time;
    };

    SpriteResource* _resource;
    AnimationInstance _animation;

    std::string _next_animation;
    float _next_frame_time_scale;
public:
    typedef std::shared_ptr<Sprite> Ptr_t;

    Sprite();
    Sprite(SpriteResource* resource);
    
    void Update(float dt);
    void Draw(const Vec2& pos) const;
    void Play(const std::string& animation, float frame_time_scale = 1);
    void PlayNext(const std::string& animation, float frame_time_scale = 1);

    float GetPixelsPerFrame(const std::string& animation) const;
    float GetFrameTime(const std::string& animation) const;
    int GetNumFrames(const std::string& animation) const;
};
