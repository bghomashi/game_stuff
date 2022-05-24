#include "game/action_states/character_action_states.h"
#include "engine/ecs/entity.h"
#include "engine/components.h"
#include "engine/engine.h"
#include "game/components/stats_component.h"
#include "engine/messages/message_queue.h"
#include "game/messages/messages.h"

static const std::vector<std::string> suffix_array = { "w", "sw", "s", "se", "e", "ne", "n", "nw", "w" };

void AttackState::Start(ActionFSMComponent& fsm, const std::string& attack, const Vec2& dir) {
    auto transform = EntityManager::Get<TransformComponent>(fsm.parent);
    auto sprite = EntityManager::Get<SpriteComponent>(fsm.parent);
    auto ability = EntityManager::Get<AbilityComponent>(fsm.parent);
    auto path = EntityManager::Get<PathComponent>(fsm.parent);

    
    while (!path->points.empty())
        path->points.pop();
        
    // Vec2 mouse_pos = Engine::GetMouseLocationWorldSpace();
    // Vec2 dir = mouse_pos - transform->position;

    fsm.angle = int(0.5*(atan2(dir.y, dir.x)*8./M_PI + 9.));

    if (ability && ability->Has(attack)) {
        fsm.lock_state = true;
        ability->Activate(attack);
        sprite->sprite.Play("attack_" + suffix_array[fsm.angle]);
    }
}

void AttackState::Update(ActionFSMComponent& fsm, float dt) {
    auto ability = EntityManager::Get<AbilityComponent>(fsm.parent);

    if (ability->Finished()) {
        fsm.lock_state = false;
        fsm.SetState<IdleState>();
    }
}

const std::string AttackState::name = "attack"; 