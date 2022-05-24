#pragma once

#include <vector>
#include <list>
#include "engine/ecs/entity.h"
#include "engine/utility/rectangle.h"
#include "engine/utility/slot_map.h"
#include "engine/utility/ranged_value.h"

#include <iostream>

namespace Combat {
    typedef Rectangle HurtBox;
    struct HitBox : Rectangle {
        EntityID parent;

        static std::list<HitBox> s_hitboxes;
        static HitBox* Create(EntityID id);
        static void Free(HitBox* hb);
        static void DrawHitBoxes();
    };

    // Any object participating in combat must have
    struct Combatant : Component<Combatant> {
        RangedValue<int> health;
        RangedValue<int> mana;

        std::vector<HurtBox> hurt_boxes;

        
        bool TestHitBox(const HitBox& hit_box);

        static void DrawHurtBoxes();
    };

    void ResolveHit(Combatant* offender, Combatant* defender);

};