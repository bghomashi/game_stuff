#pragma once

#include "engine/ecs/entity.h"
#include <memory>
#include <unordered_map>
#include <string>

struct Ability {
    typedef std::shared_ptr<Ability> Ptr_t;
    
    float cooldown;
    EntityID parent;

    virtual void Start();
    virtual void Stop();
    virtual bool Update(float dt);

    virtual const std::string& Name() const = 0;
};

struct AbilityComponent : Component<AbilityComponent> {
    std::unordered_map<std::string, Ability::Ptr_t> abilities;
    Ability::Ptr_t active_ability;

    bool Has(const std::string& ability) const;
    void Activate(const std::string& ability);
    bool Finished() const;
    void Start();
    void Stop();
    void Update(float dt);
};