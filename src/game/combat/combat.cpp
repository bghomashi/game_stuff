#include "game/combat/combat.h"
#include "engine/graphics/graphics.h"
#include "engine/components/transform_component.h"
#include "game/messages/messages.h"

bool Combat::Combatant::TestHitBox(const HitBox& hit_box) {
    auto transform = EntityManager::Get<TransformComponent>(parent);

    for (auto hurt : hurt_boxes) {
        hurt.x += transform->position.x - 0.5f*hurt.width;
        hurt.y += transform->position.y - 0.5f*hurt.height;
        if (intersectRect<float>(hit_box, hurt))
            return true;
    }
    return false;
}


void Combat::Combatant::DrawHurtBoxes() {
    ForEach([] (const Combatant& combat) {
        auto transform = EntityManager::Get<TransformComponent>(combat.parent);
        for (auto& hb : combat.hurt_boxes) {
            OGL::Draw::SquareFilled(transform->position + 
                Vec2{hb.x, hb.y},
                {hb.width, hb.height},
                { 0.0f, 1.0f, 1.0f, 0.5f });
        }
    });
}

Combat::HitBox* Combat::HitBox::Create(EntityID id) {
    s_hitboxes.emplace_back();
    s_hitboxes.back().parent = id;
    return &s_hitboxes.back();
}
void Combat::HitBox::DrawHitBoxes() {
    for (auto& hb : s_hitboxes) {
        OGL::Draw::SquareFilled(
            Vec2{hb.x, hb.y}+0.5f*Vec2{hb.width, hb.height},
            {hb.width, hb.height},
            { 1.0f, 0.0f, 0.5f, 0.5f });
    }
}
void Combat::HitBox::Free(HitBox* in) {
    for (auto it = s_hitboxes.begin(); it != s_hitboxes.end(); it++) {
        auto& hb = *it;
        if (&hb == in) {
            s_hitboxes.erase(it);
            return;
        }
    }
}

#include "game/components/stats_component.h"
#include "engine/components/ability_component.h"

void Combat::ResolveHit(Combatant* offender, Combatant* defender) {
    int DMG = 80;
    int RFL = 0;
    
    // Final damage here 
    if (DMG > 0) {
        auto overkill = defender->health.Sub(DMG);
        if (defender->health == 0) {
            EntityDeath* msg = new EntityDeath();
            msg->entity = defender->parent;
            MessageQueue::Push(msg);
        } else {
            EntityDamaged* msg = new EntityDamaged();
            msg->entity = defender->parent;
            MessageQueue::Push(msg);
        }
    }

    if (RFL > 0) {
        auto overkill = offender->health.Sub(RFL);
        if (offender->health == 0) {
            EntityDeath* msg = new EntityDeath();
            msg->entity = offender->parent;
            MessageQueue::Push(msg);
        } else {
            EntityDamaged* msg = new EntityDamaged();
            msg->entity = offender->parent;
            MessageQueue::Push(msg);
        }
    }
}






std::list<Combat::HitBox> Combat::HitBox::s_hitboxes;