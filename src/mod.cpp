#include "context.hpp"
#include "handler/handler.hpp"
#include "handler/hooks/color_hooks.hpp"

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
    // Check Shuffle Mode
    if (GET_CVAR_VALUE(g_handlerP, ShuffleEnabled) == true) {
        g_shuffleMode = SHUFFLE_REGULAR;
    } else if (GET_CVAR_VALUE(g_handlerP, BonkShuffleEnabled) == true) {
        g_shuffleMode = SHUFFLE_BONK;
    } else {
        g_shuffleMode = SHUFFLE_OFF;
    }

    // Reset timers if needed
    if (g_shuffleTimer == 0 || g_bonkShuffleCooldown == 0) {
        switch (g_shuffleMode) {
            case SHUFFLE_REGULAR:
                if (GET_CVAR_VALUE(g_handlerP, EnableRandTimer) == true) {
                    std::mt19937 rng{std::random_device{}()};
                    std::uniform_int_distribution<int64_t> dist(1, GET_CVAR_VALUE(g_handlerP, RandTimerUpperLimit));
                    g_shuffleTimer = dist(rng);
                } else {
                    g_shuffleTimer = GET_CVAR_VALUE(g_handlerP, ShuffleTimer);
                }
                break;

            case SHUFFLE_BONK:
                g_bonkShuffleCooldown = GET_CVAR_VALUE(g_handlerP, BonkShuffleCooldown);
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
            g_handler.setBonkShuffleReady(true);
        }
    }

    // Shuffle effects
    if (g_handler.getConfigHandler()->getShuffle()) {
        g_handler.getConfigHandler()->shuffleEffects();
    }

    g_handler.getHooksHandler()->getColorHooks()->update();
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    return MOD_OK;
}
}
