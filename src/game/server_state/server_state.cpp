#include <sstream>
#include <iomanip>
#include "game/server_state/server_state.h"
#include "engine/graphics/graphics.h"
#include "engine/engine.h"
#include "engine/network/network.h"
#include "engine/systems/sprite_renderer.h"
#include "engine/components.h"

#include "game/action_states/character_action_states.h"
#include "game/network/game_messages.h"
#include "game/messages/messages.h"

using color = OGL::Color;
using draw = OGL::Draw;
using device = OGL::Device;
using matrix_stack = OGL::MatrixStack;

#define SERVER_PORT 60000
#define SNAPSHOTS_PER_SECOND 10

bool ServerState::Start() {
    // Load resources
    if (!ResourceManager<SpriteResource>::Load("goblin_mage", "res/sprites/goblin_mage.json") ||
        !ResourceManager<SpriteResource>::Load("knight", "res/sprites/knight.json")) {
        LOG_CRITICAL("failed to load sprite.");
        return false;
    }
    // just load font
#ifdef __linux__
    if (!ResourceManager<OGL::Font>::Load("Arial", g_font_directory + "Arial.ttf", 24)) {
#elif WIN32
    if (!ResourceManager<OGL::Font>::Load("Arial", g_font_directory + "Arial.ttf", 24)) {
#endif
        LOG_CRITICAL("Failed to load font");
        return false;
    }
    
    // setup hooks
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

    snapshot_accum = 0;
    bps_recv = CircularBuffer<float>(10);
    bps_sent = CircularBuffer<float>(10);

    if (!Engine::server.Start(SERVER_PORT))
        return false;
    Engine::server.client_timeout_callback = [=](const Net::ClientID& uuid) {
        ClientTimeoutCallback(uuid);
    };

    status.push_back("successfully started server.");
    status.push_back("");

    return true;
}
std::string convertToString(const std::vector<std::uint8_t> a)
{
    int i;
    std::string s = "";
    for (i = 0; i < a.size(); i++) {
        s = s + (char)a[i];
    }
    return s;
}
void ServerState::Update(float dt) {
    snapshot_accum += dt;

    Net::ClientID client_id;
    std::vector<Net::message> messages;
    if (Engine::server.Recv(messages, client_id)) {
        for (int i = 0; i < messages.size(); i++) {
            auto& msg = messages[i];
            switch (msg.header.id) {
                case GameMessage::CLIENT_REQUEST_CONNECTION: {
                    ClientRequestConnect(msg, client_id);
                    break;
                }
                case GameMessage::PLAYER_MOVE: {
                    PlayerMove(msg, client_id);
                    break;
                }
                case GameMessage::PLAYER_ATTACK: {
                    PlayerAttack(msg, client_id);
                    break;
                }
                case GameMessage::PLAYER_QUERY: {
                    PlayerQuery(msg, client_id);
                }
            }
        }
    }

    if (snapshot_accum >= 1.f/SNAPSHOTS_PER_SECOND) {
        snapshot_accum -= 1.f/SNAPSHOTS_PER_SECOND;
        bps_sent.push(Engine::server.bytes_sent);
        bps_recv.push(Engine::server.bytes_recv);
        Engine::server.bytes_sent = 0;
        Engine::server.bytes_recv = 0;
        std::stringstream ss;
        ss << std::setprecision(2) << std::fixed
           << "bps_recv = " << bps_recv.Avg()
           << " bps_sent = " << bps_sent.Avg();
        status[1] = ss.str();

        // for each component that has a client
        Net::message snapshot_msg;
        snapshot_msg.header.id = GameMessage::CHARACTER_SNAPSHOT;
        NetworkComponent::ForEach([&snapshot_msg](const NetworkComponent& nc){
            EntityID entity = nc.parent;
            Net::ClientID client = nc.client_id;

            Vec2 position = EntityManager::Get<TransformComponent>(entity)->position;
            Vec2 destination = position;

            // not sure how to handle this part.
            auto path = EntityManager::Get<PathComponent>(entity);
            if (path && !path->points.empty())
                destination = path->points.front();

            snapshot_msg << (unsigned)client;
            snapshot_msg << position.x << position.y;
            snapshot_msg << destination.x << destination.y;
            
        });
        NetworkComponent::ForEach([&snapshot_msg](const NetworkComponent& nc){
            Engine::server.Send(snapshot_msg, nc.client_id);
        });
    }

    Engine::server.Update(dt);
    // update world stuff
    ActionFSMComponent::ForEach([dt](ActionFSMComponent& fsm) {
        fsm.Update(dt);
    });
    
    AbilityComponent::ForEach([dt](AbilityComponent& ac) {
        ac.Update(dt);
    });

    SpriteRender::Update(dt);
}
void ServerState::Stop() {
    Engine::server.Stop();
}
void ServerState::Draw(float alpha) {
    device::SetClearColor(OGL::Color::Blue);
    device::Clear(OGL::ClearBit::Color);
    matrix_stack::PushProjection();
    Engine::camera.SetViewport();
    matrix_stack::PushModelView();
    Engine::camera.Draw();

    SpriteRender::Draw(alpha);

    if (Engine::debug == Engine::DebugState::On) {
        //  Combat::Combatant::DrawHurtBoxes();
        //  Combat::HitBox::DrawHitBoxes();
    }
    
    matrix_stack::PopModelView();

    Vec2 win_size = Engine::GetWindowSize();
    auto font = ResourceManager<OGL::Font>::Get("Arial");

    for (int i = 0; i < status.size(); i++) {
        auto& str = status[i];
        font.RenderText(str, 
                    10.f, win_size.y - 20.f*(i+1.f), // position
                    color::White);
    }

    matrix_stack::PopProjection();
}


#include "engine/components.h"
#include "game/components/stats_component.h"
#include "game/combat/combat.h"
#include "game/action_states/character_action_states.h"
#include "game/abilities/sword_swipe.h"

void ServerState::SpawnPlayer(const Net::ClientID& uuid) {
    SpriteResource& knight_sr = ResourceManager<SpriteResource>::Get("knight");

    ComponentID transform_handle;
    ComponentID ability_handle;
    ComponentID combat_handle;
    ComponentID fsm_handle;
    ComponentID network_handle;

    EntityID entity = EntityManager::Create();
    // add transform component (last know position)
    transform_handle = EntityManager::Add<TransformComponent>(entity, Vec2{0.f,0.f}, 0.f, Vec2{0.f,0.f}, 0.f);
    // sprite
    EntityManager::Add<SpriteComponent>(entity, Sprite(&knight_sr), transform_handle);
    // stats
    EntityManager::Add<StatsComponent>(entity, 90.f);
    // pathing
    EntityManager::Add<PathComponent>(entity);
    // state machine
    fsm_handle = EntityManager::Add<ActionFSMComponent>(entity, false, 2, 
        // actions
        std::unordered_map<std::string, ActionState::Ptr_t>
        {
            {"walk", ActionState::Ptr_t(new WalkState())},
            {"idle", ActionState::Ptr_t(new IdleState())},
            {"attack", ActionState::Ptr_t(new AttackState())},
            {"damaged", ActionState::Ptr_t(new DamagedState())},
            {"die", ActionState::Ptr_t(new DieState())}
        }, (ActionState*)NULL);
    auto fsm = ActionFSMComponent::Get(fsm_handle);

    fsm->SetState<IdleState>();      // set starting state
    // abilities
    ability_handle = EntityManager::Add<AbilityComponent>(entity);
    auto ability_component = AbilityComponent::Get(ability_handle);
    ability_component->abilities["sword_swipe"] = std::shared_ptr<Ability>(new SwordSwipe());
    ability_component->abilities["sword_swipe"]->parent = entity;
    // combatant
    combat_handle = EntityManager::Add<Combat::Combatant>(entity, RangedValue<int>{0,100,100}, RangedValue<int>{0,100,100});
    auto combatant = Combat::Combatant::Get(combat_handle);
    combatant->hurt_boxes.push_back({0,10,20,60});
    // networked
    network_handle = EntityManager::Add<NetworkComponent>(entity, uuid);
}
