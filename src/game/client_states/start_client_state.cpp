#include "game/client_states/client_states.h"
#include "engine/graphics/graphics.h"
#include "engine/engine.h"
#include "engine/network/network.h"
#include "game/network/game_messages.h"

using color = OGL::Color;
using draw = OGL::Draw;
using device = OGL::Device;
using matrix_stack = OGL::MatrixStack;

// #define SERVER_ADDR 192,168,0,2
#define SERVER_ADDR "bghomashi.asuscomm.com"
#define SERVER_PORT 60000
#define MAX_WAIT_TIME 10.f
#define MAX_RETRIES 3

bool StartClientState::Start() {
    retries_left = MAX_RETRIES;
    wait_timer = 0;
    
    // just load font
#ifdef __linux__
    if (!ResourceManager<OGL::Font>::Load("Arial", g_font_directory + "Arial.ttf", 24)) {
#elif WIN32
    if (!ResourceManager<OGL::Font>::Load("Arial", g_font_directory + "Arial.ttf", 24)) {
#endif
        LOG_CRITICAL("Failed to load font");
        return false;
    }

    if (!Engine::client.Start(SERVER_ADDR, SERVER_PORT)) {
        LOG_CRITICAL("Failed start client");
        return false;
    }

    return true;
}
void StartClientState::Update(float dt) {
    Engine::client.Update(dt);                    // send all waiting messages

    while (Engine::client.messages_waiting()) {
        auto msg = Engine::client.pop_incoming_message();

        switch (msg.header.id) {
        case GameMessage::SERVER_ACCEPT_CONNECTION:
            // new game state;

            msg >> Engine::client_id;
                
            MainClientState* s = new MainClientState();
            Engine::SetState(std::shared_ptr<ApplicationState>(s));

            return;
        }
    }
    

    wait_timer -= dt;

    if (wait_timer <= 0 && retries_left > 0) {
        retries_left--;
        wait_timer = MAX_WAIT_TIME;
        status = "Connecting to server. " + std::to_string(retries_left) + " retries left...";
        Net::message request_msg = {GameMessage::CLIENT_REQUEST_CONNECTION};
        request_msg << "bejan" << "ghomashi"; 
        Engine::client.Send(request_msg);
    } else {
        if (wait_timer <= 0 && retries_left <= 0)
            status = "Failed to connect to server. Check internet connection.";
    }
}
void StartClientState::Draw(float alpha) {
    device::SetClearColor(OGL::Color::Blue);
    device::Clear(OGL::ClearBit::Color);
    matrix_stack::PushProjection();
    Engine::camera.SetViewport();
    // matrix_stack::PushModelView();
    // Engine::camera.Draw();
    // matrix_stack::PopModelView();

    Vec2 win_size = Engine::GetWindowSize();
    auto font = ResourceManager<OGL::Font>::Get("Arial");

    font.RenderText(status, 
                10.f, win_size.y - 20.f, // position
                color::White);

    matrix_stack::PopProjection();
}