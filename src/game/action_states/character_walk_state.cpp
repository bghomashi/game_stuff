#include "game/action_states/character_action_states.h"
#include "engine/ecs/entity.h"
#include "engine/components.h"
#include "engine/engine.h"
#include "game/components/stats_component.h"
#include "engine/messages/message_queue.h"
#include "game/messages/messages.h"

static const std::vector<std::string> suffix_array = { "w", "sw", "s", "se", "e", "ne", "n", "nw", "w" };

void WalkState::Update(ActionFSMComponent& fsm, float dt) {
    // to walk we need to get pathing info.
    // this can/will change at any time
    
    auto path = EntityManager::Get<PathComponent>(fsm.parent);
    auto sprite = EntityManager::Get<SpriteComponent>(fsm.parent);
    auto transform = EntityManager::Get<TransformComponent>(fsm.parent);
    auto stats = EntityManager::Get<StatsComponent>(fsm.parent);

    Vec2 destination;
    Vec2 rel_motion;
    Vec2 dir;
    float ms = stats->move_speed;
    float distance = ms*dt;                     // max distance we can move


    while (distance > 0) {
        // not going anywhere
        if (path->points.empty()) {
            // done walking 
            fsm.SetState("idle");
            return;
        }

        destination = path->points.front();
        rel_motion = destination - transform->position;
        dir = rel_motion.norm();
        ms = stats->move_speed;


        fsm.angle = int(0.5*(atan2(dir.y, dir.x)*8./M_PI + 9.));
        if (rel_motion.lengthSqr() <= distance*distance) {           // we made it
            transform->SetPosition(path->points.front());
            path->points.pop();
            distance -= rel_motion.length(); 
        } else {
            transform->SetPosition(dir*distance + transform->position);
            distance = 0;
            float ppf = sprite->sprite.GetPixelsPerFrame("walk_" + suffix_array[fsm.angle]);
            sprite->sprite.Play("walk_" + suffix_array[fsm.angle], ppf/ms); 
        }
    }
}