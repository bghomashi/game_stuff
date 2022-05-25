#include "game/action_states/character_action_states.h"
#include "engine/ecs/entity.h"
#include "engine/components.h"
#include "engine/engine.h"
#include "game/components/stats_component.h"
#include "engine/messages/message_queue.h"
#include "game/messages/messages.h"

static const std::vector<std::string> suffix_array = { "w", "sw", "s", "se", "e", "ne", "n", "nw", "w" };

void DieState::Start(ActionFSMComponent& fsm) {
    timer = 2;
    auto sprite = EntityManager::Get<SpriteComponent>(fsm.parent);
    sprite->sprite.Play("die_" + suffix_array[fsm.angle]);
}
void DieState::Update(ActionFSMComponent& fsm, float dt) {
    auto transform = EntityManager::Get<TransformComponent>(fsm.parent);
    transform->SetPosition(transform->position);
    timer -= dt;
    if (timer <= 0) {
        timer = 0;
        EntityDestroy* msg = new EntityDestroy();
        msg->entity = fsm.parent;
        MessageQueue::Push(msg);
    }
}

const std::string DieState::name = "die";