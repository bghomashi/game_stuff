#pragma once

#include "engine/components/ability_component.h"
#include "engine/ecs/entity.h"
#include "game/combat/combat.h"
#include <vector>
#include <string>


class SwordSwipe : public Ability {
public:
    Combat::HitBox* hb;
    std::vector<EntityID> already_hit;
    const static std::string name;
    static const std::vector<std::string> suffix_array;

    SwordSwipe();
    void Start();
    void Stop();
    bool Update(float dt);

    inline const std::string& Name() const {
        return name;
    }
};