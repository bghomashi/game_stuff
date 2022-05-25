#include "game/my_application.h"
#include "engine/engine.h"

#include "engine/systems/sprite_renderer.h"
#include "engine/components.h"
#include "game/messages/messages.h"

#include <iostream>

void MyApplication::Update(float dt) {

    // user input
    if (Engine::GetKeyPressed(Input::W))
        Engine::camera.position += Vec2{ 0.f, 1.f};
    if (Engine::GetKeyPressed(Input::A))
        Engine::camera.position += Vec2{-1.f, 0.f};
    if (Engine::GetKeyPressed(Input::S))
        Engine::camera.position += Vec2{ 0.f,-1.f};
    if (Engine::GetKeyPressed(Input::D))
        Engine::camera.position += Vec2{ 1.f, 0.f};
    
    if (Engine::GetKeyPressed(Input::Two)) {
        if (EntityManager::Exists(entityOne))
            EntityManager::Destroy(entityOne);
    }

    if (Input::MousePressed(Input::MouseLeft)) {
        EntityMoveToCommand* msg = new EntityMoveToCommand();
        msg->entity = entityOne;
        msg->destination = Engine::GetMouseLocationWorldSpace();
        MessageQueue::Push(msg);
    }


    if (mode == AppMode::CLIENT || mode == AppMode::BOTH)
        Engine::client.Update(dt);
    if (mode == AppMode::SERVER || mode == AppMode::BOTH)
        Engine::server.Update(dt);


    ActionFSMComponent::ForEach([dt](ActionFSMComponent& fsm) {
        fsm.Update(dt);
    });
    
    AbilityComponent::ForEach([dt](AbilityComponent& ac) {
        ac.Update(dt);
    });

    SpriteRender::Update(dt);
}