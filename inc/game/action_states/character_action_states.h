#pragma once

#include "engine/components/action_fsm_component.h"

struct WalkState : ActionState {
    const static std::string name;
    void Update(ActionFSMComponent& fsm, float dt);
};

struct AttackState : ActionState {
    const static std::string name;

    Vec2 dir;
    void Start(ActionFSMComponent& fsm, const std::string& attack, const Vec2& dir);
    void Update(ActionFSMComponent& fsm, float dt);
};

struct DamagedState : ActionState {
    const static std::string name;

    float timer;
    void Start(ActionFSMComponent& fsm);
    void Update(ActionFSMComponent& fsm, float dt);
};

struct DieState : ActionState {
    const static std::string name;

    float timer; 
    void Start(ActionFSMComponent& fsm);
    void Update(ActionFSMComponent& fsm, float dt);
};

struct IdleState : ActionState {
    const static std::string name;

    void Start(ActionFSMComponent& fsm);
    void Update(ActionFSMComponent& fsm, float dt);
};
