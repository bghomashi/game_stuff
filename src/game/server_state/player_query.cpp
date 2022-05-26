#include "game/server_state/server_state.h"
#include "engine/engine.h" 
#include "engine/components.h"
#include "game/network/game_messages.h"
#include "game/messages/messages.h"
#include "game/action_states/character_action_states.h"
#include "game/combat/combat.h"

void ServerState::PlayerQuery(Net::message& query_msg, const Net::ClientID& uuid) {
    Net::ClientID client_id = Net::ClientID::INVALID;
    EntityID client_entity;
    
    if (uuid == client_id) return;
    // player[uuid] is asking about...
    query_msg >> client_id;
    // get that entity
    NetworkComponent::ForEachTillTrue([=,&client_entity](const NetworkComponent& nc) {
        if (nc.client_id == client_id) {
            client_entity = nc.parent;
            return true;
        }
        return false;
    });

    Vec2 position = EntityManager::Get<TransformComponent>(client_entity)->position;
    Vec2 destination = position;
    std::string ability_name;
    int angle = EntityManager::Get<ActionFSMComponent>(client_entity)->angle;
    std::string state = EntityManager::Get<ActionFSMComponent>(client_entity)->NameOfCurrentState();
    if (state == AttackState::name)
        ability_name = EntityManager::Get<AbilityComponent>(client_entity)->active_ability->Name();

    if (!EntityManager::Get<PathComponent>(client_entity)->points.empty())
        destination = EntityManager::Get<PathComponent>(client_entity)->points.front();

    auto combatant = EntityManager::Get<Combat::Combatant>(client_entity);

    Net::message spawn_msg = {GameMessage::CHARACTER_SPAWN};
    spawn_msg << (unsigned)client_id; 
    spawn_msg << position.x << position.y;
    spawn_msg << destination.x << destination.y;
    spawn_msg << combatant->health;
    spawn_msg << combatant->mana;
    if (state == AttackState::name)
        spawn_msg << ability_name;
    spawn_msg << angle << state;
    Engine::server.Send(spawn_msg, uuid);

}
