#include "context.hpp"
#include "handler/handler.hpp"
#include "handler/hooks/color_hooks.hpp"
#include "d/d_bomb.h"

DEFINE_MOD();

IMPORT_SERVICE(LogService, svc_log);

Handler g_handler;
Handler* g_handlerP;
uint8_t g_shuffleMode = SHUFFLE_OFF;
int64_t g_shuffleTimer = 0;
int64_t g_bonkShuffleCooldown = 0;

extern "C" {
MOD_EXPORT ModResult mod_initialize(ModError* error) {
    ModResult result;
    INITIALIZE(g_handler);
    g_handlerP = &g_handler; // Pointer is useful for "GET_CVAR_VALUE" macro

    if (GET_CVAR_VALUE(g_handlerP, BonkShuffleEnabled) == true) {
        g_handler.setBonkShuffleReady(true);
    } else {
        g_handler.setBonkShuffleReady(false);
    }

    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    // Check Shuffle Mode and ensure both modes are not on
    if (GET_CVAR_VALUE(g_handlerP, ShuffleEnabled)) {
        g_shuffleMode = SHUFFLE_REGULAR;
    } else if (GET_CVAR_VALUE(g_handlerP, BonkShuffleEnabled)) {
        g_shuffleMode = SHUFFLE_BONK;
    } else {
        g_shuffleMode = SHUFFLE_OFF;
    }

    // Reset timers if needed
    if ((g_shuffleTimer == 0 && g_shuffleMode == SHUFFLE_REGULAR) || (g_bonkShuffleCooldown == 0 && !g_handler.getBonkShuffleReady() && g_shuffleMode == SHUFFLE_BONK)) {
        switch (g_shuffleMode) {
            case SHUFFLE_REGULAR:
                if (GET_CVAR_VALUE(g_handlerP, EnableRandTimer) == true) {
                    std::mt19937 rng{std::random_device{}()};
                    std::uniform_int_distribution<int64_t> dist(GET_CVAR_VALUE(g_handlerP, RandTimerLowerLimit), GET_CVAR_VALUE(g_handlerP, RandTimerUpperLimit));
                    g_shuffleTimer = dist(rng) * 30;
                } else {
                    mods::log::debug("Setting shuffle timer");
                    g_shuffleTimer = GET_CVAR_VALUE(g_handlerP, ShuffleTimer) * 30;
                }
                break;

            case SHUFFLE_BONK:
                mods::log::debug("Setting bonk shuffle cooldown");
                g_bonkShuffleCooldown = GET_CVAR_VALUE(g_handlerP, BonkShuffleCooldown) * 30;
                break;
        }
    }

    // Decrease timers (if corresponding shuffle mode is on)
    if (g_shuffleTimer != 0 && g_shuffleMode == SHUFFLE_REGULAR) {
        g_shuffleTimer--;
        if (g_shuffleTimer == 0) {
            g_handler.getConfigHandler()->setShuffle(true);
        }
    }

    if (g_bonkShuffleCooldown != 0 && g_shuffleMode == SHUFFLE_BONK && !g_handler.getBonkShuffleReady()) {
        g_bonkShuffleCooldown--;
        if (g_bonkShuffleCooldown == 0) {
            mods::log::debug("bonk shuffle ready");
            g_handler.setBonkShuffleReady(true);
        }
    }

    // Shuffle effects
    if (g_handler.getConfigHandler()->getShuffle()) {
        g_handler.getConfigHandler()->shuffleEffects();
    }

    // Check randomized colors
    g_handler.getHooksHandler()->getColorHooks()->update();

    // Check exploding enemies
    auto& explosions = g_handler.getConfigHandler()->getActiveExplosions();
    for (auto iterator = explosions.begin(); iterator != explosions.end();) {
        if (iterator->explosionTimer != 0) {
            iterator->explosionTimer--;
        }

        if (iterator->explosionTimer == 0) {
            dBomb_c::createNormalBombExplode(&iterator->explosionPos);
            mods::log::debug("Erasing active explosion");
            iterator = explosions.erase(iterator);
        } else {
            ++iterator;
        }
    }
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    return MOD_OK;
}
}
