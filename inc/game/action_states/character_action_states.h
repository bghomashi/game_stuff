#pragma once

#include "engine/components/action_fsm_component.h"

struct WalkState : ActionState {
    void Update(ActionFSMComponent& fsm, float dt);
};

struct AttackState : ActionState {
    void Start(ActionFSMComponent& fsm);
    void Update(ActionFSMComponent& fsm, float dt);
};

struct DamagedState : ActionState {
    float timer;
    void Start(ActionFSMComponent& fsm);
    void Update(ActionFSMComponent& fsm, float dt);
};

struct DieState : ActionState {
    float timer; 
    void Start(ActionFSMComponent& fsm);
    void Update(ActionFSMComponent& fsm, float dt);
};

struct IdleState : ActionState {
    void Start(ActionFSMComponent& fsm);
};
