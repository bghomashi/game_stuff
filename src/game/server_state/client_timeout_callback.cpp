#include "game/server_state/server_state.h"
#include "engine/engine.h" 
#include "engine/components.h"
#include "game/network/game_messages.h"
#include "game/messages/messages.h"


void ServerState::ClientTimeoutCallback(const Net::ClientID& client_id) {
    EntityID entity = EntityID::INVALID;
    
    // who is being disconnected
    NetworkComponent::ForEachTillTrue([&](const NetworkComponent& nc) {
        if (nc.client_id == client_id) {
            entity = nc.parent;
            return true;
        }
        return false;
    });
    if (entity == EntityID::INVALID)            // no entity associated with this client
        return;

    EntityDestroy* msg = new EntityDestroy();
    msg->entity = entity;
    MessageQueue::Push(msg);


    Net::message destroy_msg = {GameMessage::CHARACTER_DESTROY};
    destroy_msg << (unsigned)client_id; 

    // broadcast to everyone this player is gone
    NetworkComponent::ForEach([&](const NetworkComponent& nc){
        if (nc.client_id != client_id)
            Engine::server.Send(destroy_msg, nc.client_id);
    });
}
