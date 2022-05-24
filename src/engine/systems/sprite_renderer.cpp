#include "engine/systems/sprite_renderer.h"

void SpriteRender::Update(float dt) {
    // update the animation
    SpriteComponent::ForEach([dt](SpriteComponent& sc) {
        sc.sprite.Update(dt);
    });
}
void SpriteRender::Draw(float alpha) {
    SpriteComponent::ForEach([=](const SpriteComponent& sc) {
        Vec2 curr_pos = TransformComponent::Get(sc.transform_handle)->position;  // current frame animation
        Vec2 prev_pos = TransformComponent::Get(sc.transform_handle)->prev_position;  // current frame animation
        Vec2 pos = curr_pos*alpha + prev_pos*(1.f - alpha);
        sc.sprite.Draw(pos);
    }); 
}