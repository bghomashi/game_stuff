#include "game/action_states/character_action_states.h"
#include "engine/ecs/entity.h"
#include "engine/components.h"
#include "engine/engine.h"
#include "game/components/stats_component.h"
#include "engine/messages/message_queue.h"
#include "game/messages/messages.h"

static const std::vector<std::string> suffix_array = { "w", "sw", "s", "se", "e", "ne", "n", "nw", "w" };


void DamagedState::Start(ActionFSMComponent& fsm) {
    auto sprite = EntityManager::Get<SpriteComponent>(fsm.parent);
    sprite->sprite.Play("damaged_" + suffix_array[fsm.angle]);
    timer = sprite->sprite.GetTotalAnimationLength("damaged_" + suffix_array[fsm.angle]);
}
void DamagedState::Update(ActionFSMComponent& fsm, float dt) {
    auto transform = EntityManager::Get<TransformComponent>(fsm.parent);
    transform->SetPosition(transform->position);
    timer -= dt;
    if (timer <= 0) {
        timer = 0;
        fsm.SetState<IdleState>();
    }
}

const std::string DamagedState::name = "damaged";