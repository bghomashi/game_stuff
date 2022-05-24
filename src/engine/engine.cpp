#include "engine/engine.h"


#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

using color = OGL::Color;
using draw = OGL::Draw;
using device = OGL::Device;
using matrix_stack = OGL::MatrixStack;

bool Engine::Go(ApplicationState* app) {

    Timer fps_timer;
    float dt;

    if (!Initialize())
        return false;

    SetState(std::shared_ptr<ApplicationState>(app));

    fps_timer.Reset();

    while(!s_window.ShouldQuit()) {
        dt = fps_timer.Elapsed();
        s_fps_tracker.push(1.f/dt);
        s_accum += dt;
        
        while (s_accum >= s_time_step) {
            s_accum -= s_time_step;
            s_window.HandleEvents();
            
            MessageQueue::Flush();

            s_app_state->Update(s_time_step);
        }

        s_window.SwapBuffers();
        s_app_state->Draw(s_accum / s_time_step);
    }

    s_app_state->Stop();

    Destroy();

    return true;
}

bool Engine::Initialize() {
    s_fps_tracker = CircularBuffer<float>(50);

    // initialize window
    if (!s_window.Create(WINDOW_WIDTH, WINDOW_HEIGHT, "Window title")) {
        LOG_CRITICAL("Failed to initialize window.");
        return false;
    }

    // initialize opengl
    if (!draw::Initialize()) {
        LOG_CRITICAL("Failed to initialize device.");
        return false;
    }

    s_window.resize_callback = [=] (int width, int height) {
        camera.viewport = {0,0,width, height};
    };
    s_window.mouse_btn_callback = [=] (int button, int action, int mods) {
        Input::OnMouseButton(button, action, mods);
    };
    s_window.key_callback = [=] (int key, int action, int mods) {
        Input::OnKey(key, action, mods);
    };
    s_window.mouse_pos_callback = [=] (double x, double y) {
        Input::OnMouseMove(x, y);
    };
    s_window.character_callback = [=] (char character) {
    };


    camera = {{0,0}, {0,0,WINDOW_WIDTH, WINDOW_HEIGHT}};
    s_accum = 0;
    s_time_step = 0.001f;

    return true;
}

void Engine::Destroy() {
    // draw::Destroy();
    s_window.Close();
}

Vec2 Engine::GetWindowSize() {
    return s_window.Size().ToFloats();
}
Vec2 Engine::GetMouseLocation() {
    return s_window.MousePos();
}
Vec2 Engine::GetMouseLocationWorldSpace() {
    return camera.position + s_window.MousePos();
}
bool Engine::GetKeyPressed(int key) {
    return s_window.KeyPressed(key);
}
bool Engine::GetMousePressed(int button) {
    return s_window.MousePressed(button);
}
const CircularBuffer<float>& Engine::FpsTracker() {
    return s_fps_tracker;
}
void Engine::SetState(ApplicationState::Ptr_t state) {
    if (s_app_state != nullptr)
        s_app_state->Stop();
    s_app_state = state;
    s_app_state->Start();
}












// initialize static variables

CircularBuffer<float> Engine::s_fps_tracker;

float Engine::s_accum;
float Engine::s_time_step;

Engine::DebugState Engine::debug = Engine::DebugState::None;

OGL::Window Engine::s_window;
Camera Engine::camera;

Net::ClientID Engine::client_id = Net::ClientID::INVALID;
Net::Client Engine::client;
Net::Server Engine::server;
ApplicationState::Ptr_t Engine::s_app_state;