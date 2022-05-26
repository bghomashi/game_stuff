#include "game/server_state/server_state.h"
#include "engine/engine.h"
#include "engine/components.h"
#include "game/network/game_messages.h"
#include "game/action_states/character_action_states.h"

void ServerState::ClientRequestConnect(Net::message& msg, const Net::ClientID& uuid) {
    std::string username, password;
    msg >> password >> username;

    // check DB for username and password

    // check that user is not already online
    if (NetworkComponent::ForEachTillTrue([=](const NetworkComponent& nc){
        return uuid == nc.client_id;
    })) return;


    Net::message accept_msg = {GameMessage::SERVER_ACCEPT_CONNECTION};
    accept_msg << "bejan" << "ghomashi" << (unsigned)uuid; 
    Engine::server.Send(accept_msg, uuid);
    Engine::server.Flush(uuid);

    // spawn player
    EntityID new_entity = SpawnPlayer(uuid);          // probabilty do this with a message

    Vec2 position = {0, 0};
    Vec2 destination = {0, 0};
    std::string ability_name;
    int angle = EntityManager::Get<ActionFSMComponent>(new_entity)->angle;
    std::string state = EntityManager::Get<ActionFSMComponent>(new_entity)->NameOfCurrentState();
    if (state == AttackState::name) {
        auto ac = EntityManager::Get<AbilityComponent>(new_entity);
        if (ac->active_ability)
            ability_name = ac->active_ability->Name();
    }



    Net::message spawn_msg = {GameMessage::CHARACTER_SPAWN};
    spawn_msg << (unsigned)uuid; 
    spawn_msg << position.x << position.y;
    spawn_msg << destination.x << destination.y;
    if (state == AttackState::name)
        spawn_msg << ability_name;
    spawn_msg << angle << state;
    Engine::server.Send(spawn_msg, uuid);

    // notify previous network clients
    NetworkComponent::ForEach([&](const NetworkComponent& nc) {
        // also send the same spawn message to each other client
        if (nc.client_id != uuid)
            Engine::server.Send(spawn_msg, nc.client_id);
    });

    // notify new client of old entities
    NetworkComponent::ForEach([&](const NetworkComponent& nc){
        // also send the same spawn message to each other client
        if (nc.client_id != uuid) {
            auto transform = EntityManager::Get<TransformComponent>(nc.parent);
            position = transform->position;
            destination = transform->position;

            if (!EntityManager::Get<PathComponent>(nc.parent)->points.empty())
                destination = EntityManager::Get<PathComponent>(nc.parent)->points.front();
            
            angle = EntityManager::Get<ActionFSMComponent>(nc.parent)->angle;
            state = EntityManager::Get<ActionFSMComponent>(nc.parent)->NameOfCurrentState();
            if (state == AttackState::name)
                ability_name = EntityManager::Get<AbilityComponent>(new_entity)->active_ability->Name();


            Net::message spawn_msg = {GameMessage::CHARACTER_SPAWN};
            spawn_msg << (unsigned)nc.client_id; 
            spawn_msg << position.x << position.y;
            spawn_msg << destination.x << destination.y;
            if (state == AttackState::name)
                spawn_msg << ability_name;
            spawn_msg << angle << state;

            Engine::server.Send(spawn_msg, uuid);
        }
    });
}

