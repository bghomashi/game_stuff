#pragma once

#include <string>
#include "engine/application.h"

class StartClientState : public ApplicationState {
    int retries_left;
    float wait_timer;
    std::string status;
public:
    bool Start(); 
    void Update(float dt);
    void Draw(float alpha);

};