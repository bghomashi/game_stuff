#include "game/server_state/server_state.h"
#include "engine/engine.h"
#include "engine/components.h"
#include "game/network/game_messages.h"

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
    SpawnPlayer(uuid);          // probabilty do this with a message

    Vec2 position = {0, 0};
    Vec2 destination = {0, 0};

    Net::message spawn_msg = {GameMessage::CHARACTER_SPAWN};
    spawn_msg << (unsigned)uuid; 
    spawn_msg << position.x << position.y;
    spawn_msg << destination.x << destination.y;
    Engine::server.Send(spawn_msg, uuid);

    // notify previous network clients
    NetworkComponent::ForEach([&](const NetworkComponent& nc){
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
            

            Net::message spawn_msg = {GameMessage::CHARACTER_SPAWN};
            spawn_msg << (unsigned)nc.client_id; 
            spawn_msg << position.x << position.y;
            spawn_msg << destination.x << destination.y;
            Engine::server.Send(spawn_msg, uuid);
        }
    });
}

