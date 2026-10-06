#pragma once

#include "mods/service.hpp"
#include "mods/svc/log.hpp"
#include <variant>

#define INITIALIZE(handler)                                         \
    result = handler.initialize(error);                             \
    if (result != MOD_OK) {                                         \
        std::string error_msg = "Error in ";                        \
        error_msg += #handler;                                      \
        return mods::set_error(error, result, error_msg.c_str());   \
    }


// UI Macros ---------------------------------------------------------------------
#define ADD_INIT_CONTROL(control_kind, label_str, callback, error_msg)  \
    control = UI_CONTROL_DESC_INIT;                                     \
    control.kind = control_kind;                                        \
    control.label = label_str;                                          \
    control.on_pressed = &callback;                                     \
    result = add_control(panel, control);                               \
    if (result != MOD_OK) {                                             \
        return mods::set_error(error, result, error_msg);               \
    }

#define ADD_NUMBER_CONTROL(label_str, help, cvar, minimum, maximum, error_msg)  \
    control = UI_CONTROL_DESC_INIT;                                             \
    control.kind = UI_CONTROL_NUMBER;                                           \
    control.label = label_str;                                                  \
    control.help_rml = help;                                                    \
    control.binding = UI_BINDING_CONFIG_VAR;                                    \
    control.config_var = cvar;                                                  \
    control.min = minimum;                                                      \
    control.max = maximum;                                                      \
    result = add_control(left, control);                                        \
    if (result != MOD_OK) {                                                     \
        return mods::set_error(error, result, error_msg);                       \
    }

#define ADD_TOGGLE(cvarName, label_str, help)                                   \
    result = add_toggle(left, label_str, GET_CVAR(g_ui, cvarName), help);       \
    if (result != MOD_OK) {                                                     \
        return mods::set_error(error, result, "Failed to add effect toggle");   \
    }

#define ADD_SHUFFLE_TOGGLE(cvarName, label_str, help)                                   \
    result = add_toggle(left, label_str, GET_CVAR(g_ui, cvarName##Shuffle), help);      \
    if (result != MOD_OK) {                                                             \
        return mods::set_error(error, result, "Failed to add effect shuffle toggle");   \
    }

#define ADD_TABS(tabs_no, close_callback, window_handle, error_msg)         \
    UiWindowDesc desc = UI_WINDOW_DESC_INIT;                                \
    desc.tabs = tabs;                                                       \
    desc.tab_count = tabs_no;                                               \
    desc.on_closed = close_callback;                                        \
    if (svc_ui->window_push(mod_ctx, &desc, &window_handle) != MOD_OK) {    \
        mods::log::debug(error_msg);                                        \
    }
// -------------------------------------------------------------------------------


// Config Macros -----------------------------------------------------------------
#define REGISTER_CVARS(name, type, default)                                         \
    result = register_cvar<##type>("g_cvar" #name, default, g_cvar##name, error);   \
    if (result != MOD_OK) {                                                         \
        return mods::set_error(error, result, "Failed to register cvar");           \
    }

#define REGISTER_SHUFFLE_CVARS(name, type, default)                                             \
    result = register_cvar<bool>("g_cvar" #name "Shuffle", true, g_cvar##name##Shuffle, error); \
    if (result != MOD_OK) {                                                                     \
        return mods::set_error(error, result, "Failed to register cvar");                       \
    }

#define EFFECT_GETTERS_SETTERS(name, type, default)                                     \
    ConfigVarHandle getCvar##name() { return *name; }                                   \
    type get##name() { return getOption<type>(*name, default); }                        \
    void setCvar##name(type value) { setOption<type>(*name, value); }                   \
    ConfigVarHandle& getCvar##name##Shuffle() { return *name##Shuffle; }                \
    type get##name##Shuffle() { return getOption<type>(*name##Shuffle, default); }      \
    void setCvar##name##Shuffle(type value) { setOption<type>(*name##Shuffle, value); }

#define GETTERS_SETTERS(name, type, default)                            \
    ConfigVarHandle getCvar##name() { return *name; }                   \
    type get##name() { return getOption<type>(*name, default); }        \
    void setCvar##name(type value) { setOption<type>(*name, value); }

#define EFFECT_MEMBER_CVARS(name, type, default)    \
    ConfigVarHandle* name;                          \
    ConfigVarHandle* name##Shuffle;

#define MEMBER_CVARS(name, type, default)   \
    ConfigVarHandle* name;

#define GET_CVAR(handler, name)                     \
    handler->getConfigHandler()->getCvar##name()

#define GET_CVAR_VALUE(handler, name)           \
    handler->getConfigHandler()->get##name()

#define SET_CVAR_VALUE(handler, name, value)            \
    handler->getConfigHandler()->setCvar##name(value);

#define CHAOS_EFFECTS(X)            \
    X(FastLink, bool, false)        \
    X(Invisibility, bool, false)    \
    X(InstaKill, bool, false)       \
    X(RandoColors, bool, false)     \
    X(DamageRoulette, bool, false)  \
    X(Arthritis, bool, false)       \
    X(TinyLink, bool, false)        \
    X(BigLink, bool, false)         \
    X(Slippery, bool, false)        \
    X(ZoomOut, bool, false)         \
    X(HealthDrain, bool, false)     \
    X(Spin, bool, false)            \
    X(VertFlipCamera, bool, false)  \
    X(DropsDamage, bool, false)     \
    X(ExplodeEnemies, bool, false)

#define EFFECT_SETTINGS(X)              \
    X(FastLinkSpeed, int64_t, 3)        \
    X(DamageRouletteOdds, int64_t, 50)  \
    X(TinyLinkScale, int64_t, 3)        \
    X(BigLinkScale, int64_t, 3)         \
    X(SlipperyScale, int64_t, 1)        \
    X(ZoomOutPercent, int64_t, 75)      \
    X(HealthDrainTimer, int64_t, 60)    \
    X(SpinSpeed, int64_t, 1)            \
    X(DropsDamageAmount, int64_t, 4)    \
    X(ExplodeEnemiesTimer, int64_t, 2)

#define EFFECT_TOGGLES(X)                                                                           \
    X(FastLink, "Fast Link", "Makes Link Faster")                                                   \
    X(Invisibility, "Invisibility", "Makes Link Invisible")                                         \
    X(InstaKill, "Instant Kills", "Damage will instantly kill Link")                                \
    X(RandoColors, "Random Link Colors", "Randomizes Link's colors")                                \
    X(DamageRoulette, "Damage Roulette", "Things that would normally damage Link might heal him")   \
    X(Arthritis, "Arthritis", "Link can't move too quickly without taking damage")                  \
    X(TinyLink, "Tiny Link", "Makes Link tiny")                                                     \
    X(BigLink, "Big Link", "Makes Link big")                                                        \
    X(Slippery, "Slippery", "Makes Link slippery")                                                  \
    X(ZoomOut, "Zoom Out", "Makes the camera zoomed out")                                           \
    X(HealthDrain, "Health Drain", "Link loses health over time")                                   \
    X(Spin, "Spin", "Spins the camera around Link")                                                 \
    X(VertFlipCamera, "Upside-Down Camera", "Flips the camera upside-down")                         \
    X(DropsDamage, "Drops Damage", "Item drops damage Link")                                        \
    X(ExplodeEnemies, "Exploding Enemies", "Enemies explode after being defeated")

#define SHUFFLE_SETTINGS(X)                 \
    X(ShuffleEnabled, bool, false)          \
    X(ShuffleTimer, int64_t, 120)           \
    X(EnableRandTimer, bool, false)         \
    X(RandTimerUpperLimit, int64_t, 180)    \
    X(ShuffleToasts, bool, false)           \
    X(BonkShuffleEnabled, bool, false)      \
    X(BonkShuffleCooldown, int64_t, 300)    \
    X(BonkShuffleToasts, bool, false)
// -------------------------------------------------------------------------------

enum ChaosEffects {
    /* 0x0 */ CE_SUPER_SPEED,
    /* 0x1 */ CE_INVISIBILITY,
    /* 0x2 */ CE_INSTA_KILLS,
    /* 0x3 */ CE_RANDO_LINK_COLORS,
    /* 0x4 */ CE_DAMAGE_ROULETTE,
    /* 0x5 */ CE_ARTHRITIS,
    /* 0x6 */ CE_TINY_LINK,
    /* 0x7 */ CE_BIG_LINK,
    /* 0x8 */ CE_SLIPPERY,
    /* 0x9 */ CE_ZOOM_OUT,
    /* 0xA */ CE_HEALTH_DRAIN,
    /* 0xB */ CE_SPIN,
    /* 0xC */ CE_VERT_FLIP_CAMERA,
    /* 0xD */ CE_DAMAGE_DROPS,
    /* 0xE */ CE_EXPLODE_ENEMIES,
    EFFECT_COUNT
};
