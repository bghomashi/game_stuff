#pragma once

#include "engine/application.h"
#include "engine/ecs/entity.h"
#include "engine/network/network.h"
#include "game/network/game_messages.h"

enum class AppMode {
    SERVER,
    CLIENT,
    BOTH
};

class MyApplication  {

    // app state
    bool _debug;
    AppMode mode;

    EntityID entityOne, entityTwo;

    bool LoadResources();
    bool SetupHooks();

    bool Startup();
    void Shutdown();
    void Update(float dt);
    void Draw(float alpha);

    void SpawnPlayer();
    void SpawnGoblin();

public:
    MyApplication(AppMode mode) : mode(mode) {

    }
};
