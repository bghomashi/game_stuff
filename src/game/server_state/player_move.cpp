#include "game/server_state/server_state.h"
#include "engine/engine.h"
#include "engine/components/network_component.h"
#include "game/network/game_messages.h"
#include "game/messages/messages.h"


void ServerState::PlayerMove(Net::message& move_msg, const Net::ClientID& uuid) {
    Vec2 destination;
    Net::ClientID client_id = Net::ClientID::INVALID;
    EntityID client_entity = EntityID::INVALID;

    move_msg >> destination.y >> destination.x;
    move_msg >> client_id;

    if (client_id != uuid)
        return;

    if (!NetworkComponent::ForEachTillTrue([=,&client_entity](const NetworkComponent& nc) {
        if (nc.client_id == uuid) {
            client_entity = nc.parent;
            return true;
        }
        return false;
    })) return;             // couldnt find that entity


    EntityMoveToCommand* msg = new EntityMoveToCommand();
    msg->entity = client_entity;
    msg->destination = destination;
    MessageQueue::Push(msg);

}

