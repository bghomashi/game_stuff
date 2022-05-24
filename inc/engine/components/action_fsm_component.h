#pragma once

#include "engine/ecs/entity.h"
#include "engine/utility/vector2.h"
#include <unordered_map>
#include <stack>
#include <memory>
#include <string>

struct ActionFSMComponent;
struct ActionState {
    typedef std::shared_ptr<ActionState> Ptr_t;

    virtual void Start(ActionFSMComponent& ac) {}
    virtual void Stop(ActionFSMComponent& ac) {}
    virtual void Update(ActionFSMComponent& ac, float dt) {}
    virtual void HandleKeyDown(ActionFSMComponent& fsm, int key, int mods) {}
    virtual void HandleKeyUp(ActionFSMComponent& fsm, int key, int mods) {}
};

struct ActionFSMComponent : Component<ActionFSMComponent> { 
    bool lock_state;
    int angle;
    std::unordered_map<std::string, ActionState::Ptr_t> availableState;
    
    ActionState* currentState;                              // this should never be empty; we are always in some state

    void SetState(const std::string& state);
    void HandleKeyDown(int key, int mods);
    void HandleKeyUp(int key, int mods);

    void Update(float dt);
};