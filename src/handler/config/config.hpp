#pragma once

#include "../../context.hpp"
#include <mods/svc/config.h>
#include "f_op/f_op_actor.h"

class ConfigHandler {
public:
    ModResult initialize(ModError* error);

    CHAOS_EFFECTS(EFFECT_GETTERS_SETTERS);
    EFFECT_SETTINGS(GETTERS_SETTERS);
    SHUFFLE_SETTINGS(GETTERS_SETTERS);

    template <typename T>
    ModResult register_cvar(const char* name, T defaultValue, ConfigVarHandle& handle, ModError* error) {
        ConfigVarDesc cvarDesc = CONFIG_VAR_DESC_INIT;
        cvarDesc.name = name;
        if constexpr (std::is_same_v<T, bool>) {
            cvarDesc.type = CONFIG_VAR_BOOL;
            cvarDesc.default_bool = defaultValue;
        } else if constexpr (std::is_same_v<T, int64_t>) {
            cvarDesc.type = CONFIG_VAR_INT;
            cvarDesc.default_int = defaultValue;
        } else if constexpr (std::is_same_v<T, std::string>) {
            cvarDesc.type = CONFIG_VAR_STRING;
            cvarDesc.default_string = defaultValue.c_str();
        } else if constexpr (std::is_same_v<T, float>) {
            cvarDesc.type = CONFIG_VAR_FLOAT;
            cvarDesc.default_float = defaultValue;
        }

        if (svc_config->register_var(mod_ctx, &cvarDesc, &handle) != MOD_OK) {
            return mods::set_error(error, MOD_ERROR, "failed to register cvar option");
        }
        return MOD_OK;
    }

    template <typename T>
    T getOption(ConfigVarHandle handle, T fallback) {
        T value = fallback;
        auto result = MOD_OK;
        if constexpr (std::is_same_v<T, bool>) {
            result = svc_config->get_bool(mod_ctx, handle, &value);
        } else if constexpr (std::is_same_v<T, int64_t>) {
            result = svc_config->get_int(mod_ctx, handle, &value);
        } else if constexpr (std::is_same_v<T, double>) {
            result = svc_config->get_float(mod_ctx, handle, &value);
        }
        if (handle == 0 || result != MOD_OK) {
            return fallback;
        }
        return value;
    }

    template <typename T>
    void setOption(ConfigVarHandle handle, T value) {
        if constexpr (std::is_same_v<T, bool>) {
            svc_config->set_bool(mod_ctx, handle, value);
        } else {
            svc_config->set_int(mod_ctx, handle, value);
        }
    }

    struct EnemyExplosion {
        cXyz explosionPos;
        int64_t explosionTimer;
    };

    bool getShuffle() { return shuffle; }
    void setShuffle(bool value) { shuffle = value; }
    void shuffleEffects();
    void addExplosion(cXyz pos) {
        mods::log::debug("Inside addExplosion");

        mods::log::debug("Got explosion timer");

        activeExplosions.emplace_back(EnemyExplosion{pos, getExplodeEnemiesTimer() + 30});

        mods::log::debug("Explosion added to vector");
    }
    std::vector<EnemyExplosion>& getActiveExplosions() { return activeExplosions; }
private:
    CHAOS_EFFECTS(EFFECT_MEMBER_CVARS);
    EFFECT_SETTINGS(MEMBER_CVARS);
    SHUFFLE_SETTINGS(MEMBER_CVARS);
    bool shuffle;
    std::vector<EnemyExplosion> activeExplosions;
};