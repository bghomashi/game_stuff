#include "game/abilities/sword_swipe.h"
#include "game/combat/combat.h"

#include "engine/components.h"
#include "game/messages/messages.h"

#include <algorithm>

 const std::vector<std::string> SwordSwipe::suffix_array = { "w", "sw", "s", "se", "e", "ne", "n", "nw", "w" };

Combat::HitBox* hb;
std::vector<EntityID> already_hit;

SwordSwipe::SwordSwipe() {
}
void SwordSwipe::Start() {
    auto transform = EntityManager::Get<TransformComponent>(parent);
    // auto controller = EntityManager::Get<CharacterController>(parent);
    auto sprite = EntityManager::Get<SpriteComponent>(parent);
    auto fsm = EntityManager::Get<ActionFSMComponent>(parent);

    //int angle = controller->angle;
    // controller->lock = true;
    int angle = fsm->angle;
    float spf = sprite->sprite.GetFrameTime("attack_" + suffix_array[angle]);
    int len = sprite->sprite.GetNumFrames("attack_" + suffix_array[angle]);
    sprite->sprite.Play("attack_" + suffix_array[angle]);
    cooldown = spf*len;
    already_hit.clear();

    // hitbox
    hb = Combat::HitBox::Create(parent);
    hb->width = 50;
    hb->height = 70;

    switch (angle) {
        case 0:
            hb->x = -20;
            hb->y = 10;
            break;
        case 1:
            hb->x = -10;
            hb->y = 0;
            break;
        case 2:
            hb->x =  0;
            hb->y = -20;
            hb->height = 50;
            break;
        case 3:
            hb->x = 10;
            hb->y = 0;
            break;
        case 4:
            hb->x = 20;
            hb->y = 10;
            break;
        case 5:
            hb->x = 10;
            hb->y = 20;
            break;
        case 6:
            hb->x = 0;
            hb->y = 30;
            hb->height = 60;
            break;
        case 7:
            hb->x = -10;
            hb->y = 20;
            break;
        case 8:
            hb->x = -20;
            hb->y = 10;
            break;
    }
    hb->x += transform->position.x - 0.5f*hb->width;
    hb->y += transform->position.y - 0.5f*hb->height;
}
void SwordSwipe::Stop() {
    // auto controller = EntityManager::Get<CharacterController>(parent);
    // controller->lock = false;

    Combat::HitBox::Free(hb);
}
bool SwordSwipe::Update(float dt) {
    bool result = Ability::Update(dt);
    
    std::vector<EntityID> hits;
    hits.reserve(10);
    Combat::Combatant::ForEach([&](Combat::Combatant& cb){
        if (cb.parent != parent && cb.TestHitBox(*hb)) {
            hits.push_back(cb.parent);
        }
    });
    for (auto& hit : hits) {
        // if we already hit this entity
        if (std::find(already_hit.begin(), already_hit.end(), hit) != already_hit.end())
            continue;       // skip

        CombatHit* msg = new CombatHit();
        msg->offender = parent;
        msg->defender = hit;
        MessageQueue::Push(msg);

        already_hit.push_back(hit);
    }

    return result;
}