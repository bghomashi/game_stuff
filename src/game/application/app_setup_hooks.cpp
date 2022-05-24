#include "game/my_application.h"
#include "engine/engine.h"

#include "game/messages/messages.h"

#include "engine/components.h"
#include "game/combat/combat.h"
#include "game/action_states/character_action_states.h"
#include "game/components/stats_component.h"
#include "game/abilities/sword_swipe.h"

bool MyApplication::SetupHooks() {

    Input::SetKeyUp(Input::F1, [=]() {
        _debug = !_debug;
    });
    Input::SetKeyUp(Input::One, [=]() {
        SpawnPlayer();
    });

    Input::SetKeyUp(Input::Space,[=](){
        EntityAttackCommand* msg = new EntityAttackCommand();
        msg->entity = entityOne;
        msg->attack = "sword_swing";
        MessageQueue::Push(msg);
    });
    CombatHit::RegisterListener([](CombatHit* msg) {
        auto offender = EntityManager::Get<Combat::Combatant>(msg->offender);
        auto defender = EntityManager::Get<Combat::Combatant>(msg->defender);

        Combat::ResolveHit(offender, defender);
    });
    EntityDestroy::RegisterListener([](EntityDestroy* msg) {
        EntityManager::Destroy(msg->entity);
    });



    EntityMoveToCommand::RegisterListener([] (EntityMoveToCommand* msg) {
        auto fsm = EntityManager::Get<ActionFSMComponent>(msg->entity);
        auto path = EntityManager::Get<PathComponent>(msg->entity);
        if (path) {
            while (!path->points.empty())
                path->points.pop();
            path->points.push(msg->destination);
        }
        if (fsm) {
            fsm->SetState("walk");
        }
    });

    EntityAttackCommand::RegisterListener([] (EntityAttackCommand* msg) {
        auto fsm = EntityManager::Get<ActionFSMComponent>(msg->entity);
        if (fsm) {
            fsm->SetState("attack");// not ideal, maybe include option parameter
        }
    });
    EntityDeath::RegisterListener([] (EntityDeath* msg) {
        auto fsm = EntityManager::Get<ActionFSMComponent>(msg->entity);
        if (fsm) {
            fsm->SetState("die");
        }
    });
    EntityDamaged::RegisterListener([] (EntityDamaged* msg) {
        auto fsm = EntityManager::Get<ActionFSMComponent>(msg->entity);
        if (fsm) {
            fsm->SetState("damaged");
        }
    });


    return true;
}