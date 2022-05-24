#include "game/server_state/server_state.h"
#include "engine/engine.h"
#include "engine/components/network_component.h"
#include "game/network/game_messages.h"

void ServerState::ClientRequestConnect(Net::message& msg, const Net::ClientID& uuid) {
    std::string username, password;
    msg >> password >> username;

    // check DB for username and password

    // check that user is not already online
    if (NetworkComponent::ForEachTillTrue([=](const NetworkComponent& nc){
        return uuid == nc.client_id;
    })) return;

    // this is just to test
    status.push_back(std::string(username) + " " + std::string(password) + " " + std::to_string((unsigned)uuid));

    Net::message accept_msg = {GameMessage::SERVER_ACCEPT_CONNECTION};
    accept_msg << "bejan" << "ghomashi" << (unsigned)uuid; 
    accept_msg.header.size = accept_msg.body.size();
    Engine::server.Send(accept_msg, uuid);
    Engine::server.Update();

    // spawn player
    SpawnPlayer(uuid);          // probabilty do this with a message

    Net::message spawn_msg = {GameMessage::CHARACTER_SPAWN};
    spawn_msg << (unsigned)uuid; 
    spawn_msg.header.size = spawn_msg.body.size();
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
            Net::message spawn_msg = {GameMessage::CHARACTER_SPAWN};
            spawn_msg << (unsigned)nc.client_id; 
            spawn_msg.header.size = spawn_msg.body.size();
            Engine::server.Send(spawn_msg, uuid);
        }
    });
}

