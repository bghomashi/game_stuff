#pragma once

#include "engine/ecs/entity.h"
#include "engine/messages/message_queue.h"
#include "engine/utility/vector2.h"

struct EntitySetPosition : public MessageType<EntitySetPosition> {
    EntityID entity;
    Vec2 destination;
};

struct EntitySetAngle : public MessageType<EntitySetAngle> {
    EntityID entity;
    Vec2 dir;
};



struct EntityMoveToCommand : public MessageType<EntityMoveToCommand> {
    EntityID entity;
    Vec2 destination;
};
struct EntityAttackCommand : public MessageType<EntityAttackCommand> {
    EntityID entity;
    Vec2 direction;
    std::string attack;
};



struct EntityDamaged : public MessageType<EntityDamaged> {
    EntityID entity;
};

struct EntityDeath : public MessageType<EntityDeath> {
    EntityID entity;
};

struct EntityDestroy : public MessageType<EntityDestroy> {
    EntityID entity;
};

struct CombatHit : public MessageType<CombatHit> {
    EntityID offender, defender;
};

