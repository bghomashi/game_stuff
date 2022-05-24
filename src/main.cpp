#include "game/client_states/client_states.h"
#include "game/server_state/server_state.h"

#include "engine/engine.h"


int main(int argc, char* argv[]) {
    ApplicationState* app_state;
    if (argc >= 2 && strcmp("server", argv[1]) == 0) {
        app_state = new ServerState();
    } else {
        app_state = new StartClientState();
    }
        
    if (!Engine::Go(app_state))
        return -1;

    return 0;
}