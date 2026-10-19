#include "../../context.hpp"
#include "../config/config.hpp"
#include "mods/svc/hook.hpp"

#define POST_HOOK(entry, function, error_msg)               \
    result = mods::hook::add_post<entry>(function);         \
    if (result != MOD_OK) {                                 \
        return mods::set_error(error, result, error_msg);   \
    }

#define PRE_HOOK(entry, function, error_msg)                \
    result = mods::hook::add_pre<entry>(function);          \
    if (result != MOD_OK) {                                 \
        return mods::set_error(error, result, error_msg);   \
    }

#define REPLACE_HOOK(entry, function, error_msg)            \
    result = mods::hook::replace<entry>(function);          \
    if (result != MOD_OK) {                                 \
        return mods::set_error(error, result, error_msg);   \
    }

#define ADD_ENEMY_DEATH_HOOK(symbol, class, alias)          \
    PRE_HOOK(alias, on##alias, "Failed to hook " #symbol)

#define ADD_ENEMY_DEATH_HOOKS                       \
    ENEMY_DEATH_FUNCS(ADD_ENEMY_DEATH_HOOK);        \
    ENEMY_STATIC_DEATH_FUNCS(ADD_ENEMY_DEATH_HOOK);

class ColorHooks;

class HooksHandler {
public:
    ModResult initialize(ConfigHandler* config_handler, ModError* error);
    ConfigHandler* getConfigHandler() { return configHandler; }
    ColorHooks* getColorHooks() { return colorHooks; }
    bool getBonkShuffleReady() { return bonkShuffleReady; }
    void setBonkShuffleReady(bool value) { bonkShuffleReady = value; }
private:
    ConfigHandler* configHandler;
    ColorHooks* colorHooks;
    bool bonkShuffleReady;
};