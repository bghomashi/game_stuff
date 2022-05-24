#include "game/my_application.h"
#include "engine/engine.h"

#include "engine/systems/sprite_renderer.h"

#include "game/combat/combat.h"

using color = OGL::Color;
using draw = OGL::Draw;
using device = OGL::Device;
using matrix_stack = OGL::MatrixStack;

void MyApplication::Draw(float alpha) {

    device::SetClearColor(OGL::Color::Blue);
    device::Clear(OGL::ClearBit::Color);
    matrix_stack::PushProjection();
    Engine::camera.SetViewport();
    matrix_stack::PushModelView();
    Engine::camera.Draw();



    // Draw world
    // tr.DrawLayer(0, {200,200});
    if (_debug) {
    //     tr.DrawGrid({200,200});
    }

    SpriteRender::Draw(alpha);

    if (_debug) {
         Combat::Combatant::DrawHurtBoxes();
         Combat::HitBox::DrawHitBoxes();
    }
    matrix_stack::PopModelView();



    // Draw UI
    auto font = ResourceManager<OGL::Font>::Get("DejaVuSans");
    Vec2 mouse = Engine::GetMouseLocation();
    Vec2 win_size = Engine::GetWindowSize();

    font.RenderText("Mouse=" + std::to_string(mouse.x) + " " + std::to_string(mouse.y), 
                win_size.x/2.f, win_size.y - 20.f, // position
                color::White);

    font.RenderText("FPS=" + std::to_string(Engine::FpsTracker().Avg()), 
                win_size.x/2.f, win_size.y - 40.f, // position
                color::White);



    matrix_stack::PopProjection();
}