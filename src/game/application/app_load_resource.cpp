#include "game/my_application.h"
#include "engine/engine.h"

#include "game/messages/messages.h"

#include "engine/components.h"
#include "game/combat/combat.h"
#include "game/action_states/character_action_states.h"
#include "game/components/stats_component.h"
#include "game/abilities/sword_swipe.h"

bool MyApplication::LoadResources() {
    // Load resources
    if (!ResourceManager<SpriteResource>::Load("goblin_mage", "res/sprites/goblin_mage.json") ||
        !ResourceManager<SpriteResource>::Load("knight", "res/sprites/knight.json")) {
        LOG_CRITICAL("failed to load sprite.");
        return false;
    }

    if (!ResourceManager<TileMapResource>::Load("tilemap", "res/tilemaps/tilemap.json")) {
        LOG_CRITICAL("failed to load tilemap.");
        return false;
    }

#ifdef __linux__
    if (!ResourceManager<OGL::Font>::Load("DejaVuSans", g_font_directory + "DejaVuSans.ttf", 24)) {
#elif WIN32
    if (!ResourceManager<OGL::Font>::Load("DejaVuSans", g_font_directory + "arial.ttf", 24)) {
#endif
        LOG_CRITICAL("Failed to load font");
        return false;
    }

    return true;
}