#include "game/my_application.h"
#include "engine/engine.h"

#include "game/messages/messages.h"

#include "engine/components.h"
#include "game/combat/combat.h"
#include "game/action_states/character_action_states.h"
#include "game/components/stats_component.h"
#include "game/abilities/sword_swipe.h"

using color = OGL::Color;
using draw = OGL::Draw;
using device = OGL::Device;
using matrix_stack = OGL::MatrixStack;

#define SERVER_ADDR 127,0,0,1
#define SERVER_PORT 80

void MyApplication::SpawnPlayer() {
    SpriteResource& knight_sr = ResourceManager<SpriteResource>::Get("knight");

    ComponentID transform_handle;
    ComponentID ability_handle;
    ComponentID combat_handle;
    ComponentID fsm_handle;

    entityOne = EntityManager::Create();
    // add transform component
    transform_handle = EntityManager::Add<TransformComponent>(entityOne, Vec2{0.f,0.f}, 0.f, Vec2{0.f,0.f}, 0.f);
    // sprite
    EntityManager::Add<SpriteComponent>(entityOne, Sprite(&knight_sr), transform_handle);
    // stats
    EntityManager::Add<StatsComponent>(entityOne, 90.f);
    // pathing
    EntityManager::Add<PathComponent>(entityOne);
    // state machine
    fsm_handle = EntityManager::Add<ActionFSMComponent>(entityOne, false, 2, 
        // actions
        std::unordered_map<std::string, ActionState::Ptr_t>
        {
            {"walk", std::shared_ptr<WalkState>(new WalkState())},
            {"idle", std::shared_ptr<IdleState>(new IdleState())},
            {"attack", std::shared_ptr<AttackState>(new AttackState())},
            {"damaged", std::shared_ptr<DamagedState>(new DamagedState())},
            {"die", std::shared_ptr<DieState>(new DieState())}
        }, (ActionState*)NULL);
    auto fsm = ActionFSMComponent::Get(fsm_handle);
    fsm->SetState("idle");      // set starting state
    // abilities
    ability_handle = EntityManager::Add<AbilityComponent>(entityOne);
    auto ability_component = AbilityComponent::Get(ability_handle);
    ability_component->abilities["sword_swipe"] = std::shared_ptr<Ability>(new SwordSwipe());
    ability_component->abilities["sword_swipe"]->parent = entityOne;
    // combatant
    combat_handle = EntityManager::Add<Combat::Combatant>(entityOne, RangedValue<int>{0,100,100}, RangedValue<int>{0,100,100});
    auto combatant = Combat::Combatant::Get(combat_handle);
    combatant->hurt_boxes.push_back({0,10,20,60});
}

void MyApplication::SpawnGoblin() {
    SpriteResource& sr = ResourceManager<SpriteResource>::Get("goblin_mage");

    ComponentID transform_handle;
    ComponentID fsm_handle;
    ComponentID ability_handle;
    ComponentID combat_handle;

    entityTwo = EntityManager::Create();
    transform_handle = EntityManager::Add<TransformComponent>(entityTwo, Vec2{300.f,000.f}, 0.f, Vec2{300.f,000.f}, 0.f);
    EntityManager::Add<SpriteComponent>(entityTwo, Sprite(&sr), transform_handle);
    EntityManager::Add<StatsComponent>(entityTwo, 60.f);

    // pathing
    EntityManager::Add<PathComponent>(entityTwo);
    // state machine
    fsm_handle = EntityManager::Add<ActionFSMComponent>(entityTwo, false, 2, 
        // actions
        std::unordered_map<std::string, ActionState::Ptr_t>
        {
            {"walk", std::shared_ptr<WalkState>(new WalkState())},
            {"idle", std::shared_ptr<IdleState>(new IdleState())},
            {"attack", std::shared_ptr<AttackState>(new AttackState())},
            {"damaged", std::shared_ptr<DamagedState>(new DamagedState())},
            {"die", std::shared_ptr<DieState>(new DieState())}
        }, (ActionState*)NULL);
    auto fsm = ActionFSMComponent::Get(fsm_handle);
    fsm->SetState("idle");      // set starting state

    combat_handle = EntityManager::Add<Combat::Combatant>(entityTwo, RangedValue<int>{0,100,100}, RangedValue<int>{0,100,100});
    auto combatant = Combat::Combatant::Get(combat_handle);
    combatant->hurt_boxes.push_back({0,0,20,60});

    // EntityManager::Add<GoblinAI>(entityTwo, entityOne);
}

bool MyApplication::Startup() {
    if (!LoadResources())
        return false;

    if (!SetupHooks())
        return false;

    if (mode == AppMode::CLIENT) {
        Net::message request_msg = {GameMessage::CLIENT_REQUEST_CONNECTION};
        // request_msg << username << password; ?
        Engine::client.Start(Net::MakeAddress(SERVER_ADDR, SERVER_PORT));
        Engine::client.Send(request_msg);
    }


    if (mode == AppMode::SERVER)
        Engine::server.Start(SERVER_PORT);


    _debug = false;

    // SpawnPlayer();

    // SpawnGoblin();

    device::SetViewport(0, 0, 800, 600);
    device::SetClearColor(color::Blue);
    device::SetDepthTest(false);
    device::SetBlending(true);
    device::SetBlendFunc(OGL::BlendFunc::Src_Alpha, OGL::BlendFunc::One_Minus_Src_Alpha);
    device::SetDepthMask(false);

    return true;
}