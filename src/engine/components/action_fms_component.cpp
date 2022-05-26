#include "engine/components/action_fsm_component.h"


void ActionFSMComponent::Update(float dt) {
    currentState->Update(*this, dt);
}
void ActionFSMComponent::HandleKeyDown(int key, int mods) {
    currentState->HandleKeyDown(*this, key, mods);
}
void ActionFSMComponent::HandleKeyUp(int key, int mods) {
    currentState->HandleKeyUp(*this, key, mods);
}
const std::string& ActionFSMComponent::NameOfCurrentState() const {
    return currentState->Name();
}