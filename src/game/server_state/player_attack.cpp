#include "game/server_state/server_state.h"
#include "engine/engine.h"
#include "engine/components/network_component.h"
#include "game/network/game_messages.h"
#include "game/messages/messages.h"


void ServerState::PlayerAttack(Net::message& attack_msg, const Net::ClientID& uuid) {
    Vec2 dir;
    std::string attack_name;
    Net::ClientID client_id = Net::ClientID::INVALID;
    EntityID client_entity;

    attack_msg >> attack_name;
    attack_msg >> dir.y >> dir.x;
    attack_msg >> client_id;

    if (client_id != uuid)
        return;

    // which entity
    NetworkComponent::ForEach([=,&client_entity](const NetworkComponent& nc) {
        if (nc.client_id == uuid)
            client_entity = nc.parent;
    });

    // process in game
    EntityAttackCommand* msg = new EntityAttackCommand();
    msg->entity = client_entity;
    msg->direction = dir;
    msg->attack = attack_name;
    MessageQueue::Push(msg);

    // send to everyone else
    attack_msg << client_id;
    attack_msg << dir.x << dir.y;
    attack_msg << attack_name;

    NetworkComponent::ForEach([&](NetworkComponent& nc){
        Engine::server.Send(attack_msg, nc.client_id);
    });
}

