#include "game/client_states/client_states.h"

#include "engine/systems/sprite_renderer.h"
#include "engine/engine.h"
#include "engine/components.h"

#include "game/messages/messages.h"
#include "game/combat/combat.h"
#include "game/action_states/character_action_states.h"
#include "game/components/stats_component.h"
#include "game/abilities/sword_swipe.h"
#include "game/network/game_messages.h"


using color = OGL::Color;
using draw = OGL::Draw;
using device = OGL::Device;
using matrix_stack = OGL::MatrixStack;

bool MainClientState::SetupHooks() {

    Input::SetKeyUp(Input::F1, [=]() {
        if (Engine::debug == Engine::DebugState::On)
            Engine::debug = Engine::DebugState::None;
        else
            Engine::debug = Engine::DebugState::On;
    });

    Input::SetKeyUp(Input::Space,[=](){
        auto position = EntityManager::Get<TransformComponent>(_client_entity)->position;
        auto point = Engine::GetMouseLocationWorldSpace();
        auto dir = (point - position).norm();

        auto fsm = EntityManager::Get<ActionFSMComponent>(_client_entity);
        if (fsm->NameOfCurrentState() == AttackState::name) // (fsm->lock_state)
            return;     // only do this if we are not already attacking (or locked?)

        // send message to server
        Net::message attack_msg = { GameMessage::PLAYER_ATTACK };
        attack_msg << Engine::client_id;
        attack_msg << dir.x << dir.y;
        attack_msg << SwordSwipe::name;
        Engine::client.Send(attack_msg);

        // post message
        EntityAttackCommand* msg = new EntityAttackCommand();
        msg->entity = _client_entity;
        msg->direction = dir;
        msg->attack = SwordSwipe::name;
        MessageQueue::Push(msg);
    });



    CombatHit::RegisterListener([](CombatHit* msg) {
        auto offender = EntityManager::Get<Combat::Combatant>(msg->offender);
        auto defender = EntityManager::Get<Combat::Combatant>(msg->defender);

        Combat::ResolveHit(offender, defender);
    });


    EntityMoveToCommand::RegisterListener([] (EntityMoveToCommand* msg) {
        auto fsm = EntityManager::Get<ActionFSMComponent>(msg->entity);
        auto path = EntityManager::Get<PathComponent>(msg->entity);
        if (path) {
            while (!path->points.empty())
                path->points.pop();
            path->points.push(msg->destination);
        }
        if (fsm) {
            fsm->SetState<WalkState>();
        }
    });

    EntityAttackCommand::RegisterListener([] (EntityAttackCommand* msg) {
        std::cout << "attack dir.x=" << msg->direction.x << " dir.y=" << msg->direction.y << std::endl;
        auto fsm = EntityManager::Get<ActionFSMComponent>(msg->entity);
        if (fsm) {
            fsm->SetState<AttackState>(msg->attack, msg->direction);// not ideal, maybe include option parameter
        }
    });

    EntityDestroy::RegisterListener([](EntityDestroy* msg) {
        EntityManager::Destroy(msg->entity);
    });

    EntityDeath::RegisterListener([] (EntityDeath* msg) {
        auto fsm = EntityManager::Get<ActionFSMComponent>(msg->entity);
        if (fsm) {
            fsm->SetState<DieState>();
        }
    });
    EntityDamaged::RegisterListener([] (EntityDamaged* msg) {
        auto fsm = EntityManager::Get<ActionFSMComponent>(msg->entity);
        if (fsm) {
            fsm->SetState<DamagedState>();
        }
    });

    // Net::message msg;
    // msg.header.id = GameMessage::CHARACTER_SPAWN;
    
    // msg << (unsigned)1;
    // msg << 0.f << 0.f;
    // msg << 0.f << 0.f;


    // if (!ResourceManager<SpriteResource>::Load("goblin_mage", "res/sprites/goblin_mage.json") ||
    //     !ResourceManager<SpriteResource>::Load("knight", "res/sprites/knight.json")) {
    //     LOG_CRITICAL("failed to load sprite.");
    //     return false;
    // }
    // SpawnPlayer(msg);
    
    return true;
}

bool MainClientState::LoadResources() {
    // Load resources

    //if (!ResourceManager<TileMapResource>::Load("tilemap", "res/tilemaps/tilemap.json")) {
    //    LOG_CRITICAL("failed to load tilemap.");
    //    return false;
    //}

#ifdef __linux__
    if (!ResourceManager<OGL::Font>::Load("Arial", g_font_directory + "Arial.ttf", 24)) {
#elif WIN32
    if (!ResourceManager<OGL::Font>::Load("Arial", g_font_directory + "Arial.ttf", 24)) {
#endif
        LOG_CRITICAL("Failed to load font");
        return false;
    }

    return true;
}

bool MainClientState::Start() {

    if (!LoadResources())
        return false;

    if (!SetupHooks())
        return false;
    
    Engine::debug = Engine::DebugState::None;

    device::SetViewport(0, 0, 800, 600);
    device::SetClearColor(color::Blue);
    device::SetDepthTest(false);
    device::SetBlending(true);
    device::SetBlendFunc(OGL::BlendFunc::Src_Alpha, OGL::BlendFunc::One_Minus_Src_Alpha);
    device::SetDepthMask(false);

    return true;
}
void MainClientState::Update(float dt) {
    // user input -- this info stays on client
    if (Engine::GetKeyPressed(Input::W))
        Engine::camera.position += Vec2{ 0.f, 10.f}*dt;
    if (Engine::GetKeyPressed(Input::A))
        Engine::camera.position += Vec2{-10.f, 0.f}*dt;
    if (Engine::GetKeyPressed(Input::S))
        Engine::camera.position += Vec2{ 0.f,-10.f}*dt;
    if (Engine::GetKeyPressed(Input::D))
        Engine::camera.position += Vec2{ 10.f, 0.f}*dt;
    // -----------------------------------------

    if (Input::MousePressed(Input::MouseLeft)) {
        // move "me"
        Vec2 destination = Engine::GetMouseLocationWorldSpace();

        // send message to server
        Net::message move_msg = {GameMessage::PLAYER_MOVE};
        move_msg << Engine::client_id;
        move_msg << destination.x << destination.y;
        Engine::client.Send(move_msg);

        // post message
        EntityMoveToCommand* msg = new EntityMoveToCommand();
        msg->entity = _client_entity;
        msg->destination = destination;
        MessageQueue::Push(msg);
    }

    Engine::client.Update(dt);
    while (Engine::client.messages_waiting()) {
        auto msg = Engine::client.pop_incoming_message();

        switch (msg.header.id) {
        case GameMessage::SERVER_ACCEPT_CONNECTION:
            break;
        case GameMessage::CHARACTER_SPAWN: {
            SpawnPlayer(msg);
            break;
        }
        case GameMessage::CHARACTER_SNAPSHOT: {
            // update character positions
            while (!msg.body.empty()) {
                Net::ClientID client;
                Vec2 position, destination, dir;
                std::string state, ability_name;
                int angle;
                RangedValue<int> health, mana;

                msg >> state >> angle;
                if (state == AttackState::name)
                    msg >> ability_name;
                msg >> mana >> health;
                msg >> destination.y >> destination.x;
                msg >> position.y >> position.x;
                msg >> client;

                // skip ourselves
                if (client == Engine::client_id) continue;

                if (!NetworkComponent::ForEachTillTrue([&](const NetworkComponent& nc) {
                    if (client == nc.client_id) {
                        // post message
                        auto transform = EntityManager::Get<TransformComponent>(nc.parent);
                        auto fsm = EntityManager::Get<ActionFSMComponent>(nc.parent);
                        auto combatant = EntityManager::Get<Combat::Combatant>(nc.parent);
                        
                        if (client != Engine::client_id) {
                            // dont mess with the transform
                            // unless??
                            transform->SetPosition(position);
                            float radians = M_PI*(2.*angle - 9.)/8.;
                            dir.x = cos(radians);
                            dir.y = sin(radians);
                            fsm->angle = angle;
                        }
                        
                        if (state == AttackState::name)
                            fsm->SetState<AttackState>(ability_name, dir);
                        else if (state == DamagedState::name) {
                            fsm->SetState<DamagedState>();
                        }
                        else if (state == DieState::name)
                            fsm->SetState<DieState>();
                        
                        if (destination != position) {
                            EntityMoveToCommand* msg = new EntityMoveToCommand();
                            msg->entity = nc.parent;
                            msg->destination = destination;
                            MessageQueue::Push(msg);
                        }

                        combatant->health = health;
                        combatant->mana = mana;
                        return true; // found
                    }
                    return false;   // not yet
                })) {               // we didnt find this entity
                    Net::message query_msg;
                    query_msg.header.id = GameMessage::PLAYER_QUERY;
                    query_msg << (unsigned)client;
                    Engine::client.Send(query_msg);
                }
            }
            break;
        }
        case GameMessage::CHARACTER_DESTROY: {
            Net::ClientID client_id;
            EntityID entity = EntityID::INVALID;

            msg >> client_id;
            // who is being disconnected
            NetworkComponent::ForEachTillTrue([=,&entity](const NetworkComponent& nc) {
                if (nc.client_id == client_id) {
                    entity = nc.parent;
                    return true;
                }
                return false;
            });


            if (entity != EntityID::INVALID) {
                EntityDestroy* destroy_msg = new EntityDestroy();
                destroy_msg->entity = entity;
                MessageQueue::Push(destroy_msg);
            }
            break;
        }
        
        case GameMessage::PLAYER_ATTACK: {
            Vec2 dir;
            std::string attack_name;
            Net::ClientID client_id = Net::ClientID::INVALID;
            EntityID client_entity = EntityID::INVALID;

            msg >> attack_name;
            msg >> dir.y >> dir.x;
            msg >> client_id;

            // if its me I already know
            if (client_id != Engine::client_id)
                break;

            // which entity
            NetworkComponent::ForEach([=,&client_entity](const NetworkComponent& nc) {
                if (nc.client_id == client_id)
                    client_entity = nc.parent;
            });

            if (client_entity == EntityID::INVALID)
                break;

            // process in game
            EntityAttackCommand* msg = new EntityAttackCommand();
            msg->entity = client_entity;
            msg->direction = dir;
            msg->attack = attack_name;
            MessageQueue::Push(msg);

            break;
        }
        default:
            LOG_CRITICAL("Unrecognized network message.");
            break;
        }
    }

    ActionFSMComponent::ForEach([dt](ActionFSMComponent& fsm) {
        fsm.Update(dt);
    });
    
    AbilityComponent::ForEach([dt](AbilityComponent& ac) {
        ac.Update(dt);
    });

    SpriteRender::Update(dt);
}
void MainClientState::Draw(float alpha) {
    device::SetClearColor(OGL::Color::Blue);
    device::Clear(OGL::ClearBit::Color);
    matrix_stack::PushProjection();
    Engine::camera.SetViewport();
    matrix_stack::PushModelView();
    Engine::camera.Draw();



    

    // // Draw world
    // // tr.DrawLayer(0, {200,200});
    // if (Engine::debug == Engine::DebugState::On) {
    // //     tr.DrawGrid({200,200});
    // }

    SpriteRender::Draw(alpha);

    if (Engine::debug == Engine::DebugState::On) {
         Combat::Combatant::DrawHurtBoxes();
         Combat::HitBox::DrawHitBoxes();
    }

    Combat::Combatant::ForEach([](const Combat::Combatant& c){
        float width = 50;       // width of hp bar
        auto transform = EntityManager::Get<TransformComponent>(c.parent);
        Vec2 hp_pos = transform->position;
        Vec2 hp_size = {width,4};
        Vec2 ma_pos = transform->position;
        Vec2 ma_size = {width,4};
        ma_pos.y += 50.;           // just an arbitrary vertical shift
        hp_pos.y += 57.;           // just an arbitrary vertical shift

        float hp_per = float(c.health.cur - c.health.min)/float(c.health.max-c.health.min);
        float ma_per = float(c.mana.cur - c.mana.min)/float(c.mana.max-c.mana.min);

        draw::SquareFilled(
            hp_pos - Vec2{1,0}, 
            hp_size + Vec2{2,2},
            color::Black
        );
        draw::SquareFilled(
            ma_pos - Vec2{1,0}, 
            ma_size + Vec2{2,2},
            color::Black
        );


        hp_size.x *= hp_per;
        hp_pos.x -= 0.5f*(width-hp_size.x);
        ma_size.x *= ma_per;
        ma_pos.x -= 0.5f*(width-ma_size.x);
        draw::SquareFilled(
            hp_pos, 
            hp_size,
            color::Red
        );
        draw::SquareFilled(
            ma_pos - Vec2{1,0}, 
            ma_size,
            color::Blue
        );
    });


    matrix_stack::PopModelView();



    // // Draw UI
    auto font = ResourceManager<OGL::Font>::Get("Arial");
    // Vec2 mouse = Engine::GetMouseLocation();
    Vec2 win_size = Engine::GetWindowSize();

    // font.RenderText("Mouse=" + std::to_string(mouse.x) + " " + std::to_string(mouse.y), 
    //             win_size.x/2.f, win_size.y - 20.f, // position
    //             color::White);

    font.RenderText("FPS=" + std::to_string(Engine::FpsTracker().Avg()), 
                win_size.x/2.f, win_size.y - 40.f, // position
                color::White);



    matrix_stack::PopProjection();
}




void MainClientState::SpawnPlayer(Net::message& spawn_msg) {
    SpriteResource& knight_sr = ResourceManager<SpriteResource>::Get("knight");

    ComponentID transform_handle;
    ComponentID ability_handle;
    ComponentID combat_handle;
    ComponentID fsm_handle;
    ComponentID network_handle;
    Vec2 position, destination, dir;
    std::string state, ability_name;
    int angle;
    RangedValue<int> health, mana;

    Net::ClientID uuid;

    // animation....
    spawn_msg >> state >> angle;
    if (state == AttackState::name)
        spawn_msg >> ability_name;
    spawn_msg >> mana >> health;
    spawn_msg >> destination.y >> destination.x;
    spawn_msg >> position.y >> position.x;
    spawn_msg >> uuid;

    EntityID entity = EntityManager::Create();
    if (Engine::client_id == uuid)
        _client_entity = entity;

    // add transform component
    transform_handle = EntityManager::Add<TransformComponent>(entity, position, 0.f, position, 0.f);
    // sprite
    EntityManager::Add<SpriteComponent>(entity, Sprite(&knight_sr), transform_handle);
    // stats
    EntityManager::Add<StatsComponent>(entity, 90.f);
    // pathing
    EntityManager::Add<PathComponent>(entity);
    if (destination != position) {
        EntityMoveToCommand* msg = new EntityMoveToCommand();
        msg->entity = entity;
        msg->destination = destination;
        MessageQueue::Push(msg);
    }
    // abilities
    ability_handle = EntityManager::Add<AbilityComponent>(entity);
    auto ability_component = AbilityComponent::Get(ability_handle);
    ability_component->abilities[SwordSwipe::name] = std::shared_ptr<Ability>(new SwordSwipe());
    ability_component->abilities[SwordSwipe::name]->parent = entity;
    // state machine
    fsm_handle = EntityManager::Add<ActionFSMComponent>(entity, false, 2, 
        // actions
        std::unordered_map<std::string, ActionState::Ptr_t>
        {
            {WalkState::name, ActionState::Ptr_t(new WalkState())},
            {IdleState::name, ActionState::Ptr_t(new IdleState())},
            {AttackState::name, ActionState::Ptr_t(new AttackState())},
            {DamagedState::name, ActionState::Ptr_t(new DamagedState())},
            {DieState::name, ActionState::Ptr_t(new DieState())}
        }, (ActionState*)NULL);

    float radians = M_PI*(2.*angle - 9.)/8.;
    dir.x = cos(radians);
    dir.y = sin(radians);

    auto fsm = ActionFSMComponent::Get(fsm_handle);
    fsm->angle = angle;
    if (state == AttackState::name)
        fsm->SetState<AttackState>(ability_name, dir);
    else
        fsm->SetState<IdleState>();      // set starting state

    // combatant
    combat_handle = EntityManager::Add<Combat::Combatant>(entity, health, mana);
    auto combatant = Combat::Combatant::Get(combat_handle);
    combatant->hurt_boxes.push_back({0,10,20,60});
    // network
    network_handle = EntityManager::Add<NetworkComponent>(entity, uuid);

}

void MainClientState::SpawnGoblin() {
    SpriteResource& sr = ResourceManager<SpriteResource>::Get("goblin_mage");

    ComponentID transform_handle;
    ComponentID fsm_handle;
    ComponentID ability_handle;
    ComponentID combat_handle;

    EntityID entityTwo = EntityManager::Create();
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
    fsm->SetState<IdleState>();      // set starting state

    combat_handle = EntityManager::Add<Combat::Combatant>(entityTwo, RangedValue<int>{0,100,100}, RangedValue<int>{0,100,100});
    auto combatant = Combat::Combatant::Get(combat_handle);
    combatant->hurt_boxes.push_back({0,0,20,60});

    // EntityManager::Add<GoblinAI>(entityTwo, entityOne);
}
