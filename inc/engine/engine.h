#pragma once

#include "engine/ecs/entity.h"
#include "engine/input/input.h"
#include "engine/messages/message_queue.h"

#include "engine/graphics/graphics.h"
#include "engine/graphics/objects/sprite.h"
#include "engine/graphics/objects/camera.h"
#include "engine/graphics/objects/tilemap.h"
#include "engine/resource_manager/resource_manager.h"


#include "engine/network/network.h"
#include "engine/application.h"
#include "engine/utility/logger.h"
#include "engine/utility/timer.h"
#include "engine/utility/circular_buffer.h"


class Engine {
protected:
    static CircularBuffer<float> s_fps_tracker;
    
    static float s_accum;
    static float s_time_step;

    static OGL::Window s_window;


    static ApplicationState::Ptr_t s_app_state;

protected:
    static bool Initialize();
    static void Destroy();
    static void Loop(float dt);

public:
    enum class DebugState {
        None,
        On
    };

    static Net::ClientID client_id;
    static Net::Client client;
    static Net::Server server;
    static Camera camera;
    static DebugState debug;

    static bool Go(ApplicationState* app);


    // api
    static Vec2 GetWindowSize();
    static Vec2 GetMouseLocation();
    static Vec2 GetMouseLocationWorldSpace();
    static bool GetKeyPressed(int key);
    static bool GetMousePressed(int button);

    static void SetState(ApplicationState::Ptr_t state);



    static const CircularBuffer<float>& FpsTracker();
};
