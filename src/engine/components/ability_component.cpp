#include "engine/components/ability_component.h"

void Ability::Start() {}
void Ability::Stop() {}
bool Ability::Update(float dt) {
    cooldown -= dt;
    if (cooldown < 0) {
        Stop();
        return false;
    }
    return true;
}

bool AbilityComponent::Has(const std::string& ability) const {
    return abilities.find(ability) != abilities.end();
}
void AbilityComponent::Activate(const std::string& ability) {
    active_ability = abilities[ability];
    active_ability->Start();
}
bool AbilityComponent::Finished() const {
    return (active_ability == nullptr);
}
void AbilityComponent::Start() { }
void AbilityComponent::Stop() { }
void AbilityComponent::Update(float dt) {
    if (active_ability) {
        if (!active_ability->Update(dt))
            active_ability = nullptr;
    }
}