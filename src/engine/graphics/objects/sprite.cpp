#include "graphics/objects/sprite.h"
#include "utility/logger.h"

Sprite::Sprite() : _resource(NULL) {
    _animation.anim = NULL;
    _animation.t = 0;
    _animation.frame = 0;
    _animation.frame_time = 0;
    _next_frame_time_scale = 1;
}
Sprite::Sprite(SpriteResource* resource) : _resource(resource) {
    _animation.anim = NULL;
    _animation.t = 0;
    _animation.frame = 0;
    _animation.frame_time = 0;
    _next_frame_time_scale = 1;

    auto start_anim = _resource->GetDefaultAnimation();
    Play(start_anim);
}

void Sprite::Update(float dt) {
    if (_animation.anim == NULL) return;

    _animation.t += dt;
    if (_animation.t >= _animation.frame_time) {
        _animation.t -= _animation.frame_time;
        if (_animation.frame + 1 >= _animation.anim->num_frames) {  // if we pass the end
            if (_next_animation.length() > 0) {                     // is there a next animation?
                Play(_next_animation, _next_frame_time_scale);
                _next_animation.clear();
                return;
            }
            if (_animation.anim->loop) {                            // loop back to zero
                _animation.frame = 0;
                return;
            }
            if (_animation.anim->fallback.length()) {               // is there a fallback?
                Play(_animation.anim->fallback);
                return;
            }
            // else stop animating.
            return;
        }
        _animation.frame++;
    }
}
float Sprite::GetPixelsPerFrame(const std::string& animation) const {
    auto anim = _resource->GetAnimation(animation);
    if (anim == NULL) return 0;
    return anim->pixels_per_frame;
}
float Sprite::GetFrameTime(const std::string& animation) const {
    auto anim = _resource->GetAnimation(animation);
    if (anim == NULL) return 0;
    return anim->frame_time;
}
int Sprite::GetNumFrames(const std::string& animation) const {
    auto anim = _resource->GetAnimation(animation);
    if (anim == NULL) return 0;
    return anim->num_frames;
}

void Sprite::Draw(const Vec2& pos) const {
    if (_animation.anim == NULL) return;
    const auto& image_name = _animation.anim->image_name;
    const int col = _animation.anim->start_frame.x;
    const int row = _animation.anim->start_frame.y;

    _resource->Draw(image_name, row, col + _animation.frame, pos);
}
void Sprite::Play(const std::string& animation, float frame_time_scale) {
    auto play_anim = _resource->GetAnimation(animation);
    if (play_anim == _animation.anim) return;
    
    _animation.anim = play_anim;
    if (_animation.anim == NULL) return;
    _animation.frame_time = frame_time_scale*_animation.anim->frame_time;
    _animation.frame = 0;
    _animation.t = 0;
}
void Sprite::PlayNext(const std::string& animation, float frame_time_scale) {
    _next_animation = animation;
    _next_frame_time_scale = frame_time_scale;
}