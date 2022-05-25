#pragma once

#include <string>
#include "engine/ecs/entity.h"
#include "engine/application.h"
#include "engine/network/network.h"

class MainClientState : public ApplicationState {
    EntityID _client_entity;

    bool LoadResources();
    bool SetupHooks();

    void SpawnPlayer(Net::message& uuid);
    void SpawnGoblin();
public:
    bool Start(); 
    void Update(float dt);
    void Draw(float alpha);

};