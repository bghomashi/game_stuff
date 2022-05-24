#include "engine/components/action_fsm_component.h"

void ActionFSMComponent::SetState(const std::string& state) {
    if (lock_state) return;
    auto it = availableState.find(state);
    if (it == availableState.end())
        return;

    if (currentState != NULL)
        currentState->Stop(*this);
    currentState = it->second.get();
    currentState->Start(*this);
}

void ActionFSMComponent::Update(float dt) {
    currentState->Update(*this, dt);
}
void ActionFSMComponent::HandleKeyDown(int key, int mods) {
    currentState->HandleKeyDown(*this, key, mods);
}
void ActionFSMComponent::HandleKeyUp(int key, int mods) {
    currentState->HandleKeyUp(*this, key, mods);
}