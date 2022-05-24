#pragma once


#include "engine/application.h"
#include "engine/network/network.h"
#include <vector>
#include <string>

class ServerState : public ApplicationState {
    std::vector<std::string> status;

    void ClientRequestConnect(Net::message& msg, const Net::ClientID& uuid);
    void PlayerMove(Net::message& msg, const Net::ClientID& uuid);

    void SpawnPlayer(const Net::ClientID& uuid);
public:
    bool Start(); 
    void Stop(); 
    void Update(float dt);
    void Draw(float alpha);

};