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

    void Start(ActionFSMComponent& ac) {}
    virtual void Stop(ActionFSMComponent& ac) {}
    virtual void Update(ActionFSMComponent& ac, float dt) {}
    virtual void HandleKeyDown(ActionFSMComponent& fsm, int key, int mods) {}
    virtual void HandleKeyUp(ActionFSMComponent& fsm, int key, int mods) {}
    virtual const std::string& Name() const = 0;
};

struct ActionFSMComponent : Component<ActionFSMComponent> { 
    bool lock_state;
    int angle;
    std::unordered_map<std::string, ActionState::Ptr_t> availableState;
    
    ActionState* currentState;                              // this should never be empty; we are always in some state

    void HandleKeyDown(int key, int mods);
    void HandleKeyUp(int key, int mods);

    void Update(float dt);

    const std::string& NameOfCurrentState() const;
    template <typename State, typename... Args>
    void SetState(Args... args) {
        if (lock_state) return;
        auto it = availableState.find(State::name);
        if (it == availableState.end())
            return;

        if (currentState != NULL)
            currentState->Stop(*this);
        currentState = it->second.get();
        ((State*)currentState)->Start(*this, args...);
    }
};

