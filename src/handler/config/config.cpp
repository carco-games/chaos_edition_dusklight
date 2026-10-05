#include "config.hpp"

IMPORT_SERVICE(ConfigService, svc_config);

#define EFFECT_CVAR(name, type, default)    \
    ConfigVarHandle g_cvar##name;           \
    ConfigVarHandle g_cvar##name##Shuffle;

#define CVAR(name, type, default)   \
    ConfigVarHandle g_cvar##name;

CHAOS_EFFECTS(EFFECT_CVAR);
EFFECT_SETTINGS(CVAR);
SHUFFLE_SETTINGS(CVAR);

#define MAP_EFFECT_CVAR(name)               \
    name = &g_cvar##name;                   \
    name##Shuffle = &g_cvar##name##Shuffle;

#define MAP_CVAR(name)      \
    name = &g_cvar##name;

ModResult ConfigHandler::initialize(ModError* error) {
    ModResult result;
    CHAOS_EFFECTS(REGISTER_CVARS);
    CHAOS_EFFECTS(REGISTER_SHUFFLE_CVARS);
    EFFECT_SETTINGS(REGISTER_CVARS);
    SHUFFLE_SETTINGS(REGISTER_CVARS);

    CHAOS_EFFECTS(MAP_EFFECT_CVAR)
    EFFECT_SETTINGS(MAP_CVAR);
    SHUFFLE_SETTINGS(MAP_CVAR);
    return MOD_OK;
}