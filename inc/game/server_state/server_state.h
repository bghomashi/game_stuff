#pragma once


#include "engine/application.h"
#include "engine/network/network.h"
#include "engine/utility/circular_buffer.h"
#include <vector>
#include <string>

class ServerState : public ApplicationState {
    std::vector<std::string> status;
    float snapshot_accum;
    CircularBuffer<float> bps_sent, bps_recv;

    void ClientRequestConnect(Net::message& msg, const Net::ClientID& uuid);
    void PlayerMove(Net::message& msg, const Net::ClientID& uuid);
    void PlayerAttack(Net::message& msg, const Net::ClientID& uuid);
    void PlayerQuery(Net::message& msg, const Net::ClientID& uuid);
    
    void ClientTimeoutCallback(const Net::ClientID& uuid);

    void SpawnPlayer(const Net::ClientID& uuid);
public:
    bool Start(); 
    void Stop(); 
    void Update(float dt);
    void Draw(float alpha);

};