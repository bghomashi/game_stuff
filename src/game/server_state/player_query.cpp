#include "game/server_state/server_state.h"
#include "engine/engine.h" 
#include "engine/components.h"
#include "game/network/game_messages.h"
#include "game/messages/messages.h"


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
    if (!EntityManager::Get<PathComponent>(client_entity)->points.empty())
        destination = EntityManager::Get<PathComponent>(client_entity)->points.front();

    Net::message spawn_msg = {GameMessage::CHARACTER_SPAWN};
    spawn_msg << (unsigned)client_id; 
    spawn_msg << position.x << position.y;
    spawn_msg << destination.x << destination.y;
    Engine::server.Send(spawn_msg, uuid);

}
