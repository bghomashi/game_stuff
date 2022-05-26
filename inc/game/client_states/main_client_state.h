#pragma once

#include <string>
#include "engine/ecs/entity.h"
#include "engine/application.h"
#include "engine/network/network.h"
#include "engine/utility/circular_buffer.h"

class MainClientState : public ApplicationState {
    EntityID _client_entity;
    CircularBuffer<float> bps_sent, bps_recv;
    std::string bps;

    bool LoadResources();
    bool SetupHooks();

    void SpawnPlayer(Net::message& uuid);
    void SpawnGoblin();
public:
    bool Start(); 
    void Update(float dt);
    void Draw(float alpha);

};