#pragma once

#include <string>
#include "engine/components/ability_component.h"


class SwordSwipe : public Ability {
public:
    static const std::vector<std::string> suffix_array;

    SwordSwipe();
    void Start();
    void Stop();
    bool Update(float dt);
};