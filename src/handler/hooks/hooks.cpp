#include "hooks.hpp"
#include "d/actor/d_a_alink.h"

IMPORT_SERVICE(HookService, svc_hook);

HooksHandler* g_hooks;

DEFINE_HOOK(&daAlink_c::draw, LinkDraw);
DEFINE_HOOK(&daAlink_c::setNormalSpeedF, SetDoubleAnime);
DEFINE_HOOK(&daAlink_c::procMove, LinkProcMove);
DEFINE_HOOK(&daAlink_c::procFrontRoll, LinkFrontRoll);

// Fast Effect
bool moving = false;
void onExecute(ModContext*, void* args, void*, void*) {
    // if (GET_CVAR_VALUE(g_hooks, FastLink) == false) {
    //     return;
    // }

    daAlink_c* link = mods::arg<daAlink_c*>(args, 0);
    link->mNormalSpeed *= 1.1f;
    // if (link->mProcID == daAlink_c::PROC_MOVE) {
    //     // link->mMaxSpeed *= 15;
    //     link->speed.x = 100;
    //     // link->mNormalSpeed *= 1.1;
    //     f32& anmSpeed = mods::arg_ref<f32>(args, 3);
    //     anmSpeed = 2.0f * (200.0f / 100.0f);
    //     link->mNormalSpeed = 35.0f * (200.0f / 100.0f);
    // }
    // return HOOK_SKIP_ORIGINAL;
}
void onProcMove(ModContext*, void* args, void* retval, void*) {
    daAlink_c* link = mods::arg<daAlink_c*>(args, 0);
    link->mMaxSpeed *= 5.7f;
    link->speedF *= 5.7f;
}
// HookAction on_proc_front_roll(ModContext*, void* args, void* retval, void*) {
//     if (!get_bool_option(effects[FAST].cVar, false)) {
//         return HOOK_CONTINUE;
//     }

//     daAlink_c* link = mods::arg<daAlink_c*>(args, 0);
//     link->mNormalSpeed *= 1.2f;
//     return HOOK_CONTINUE;
// }

HookAction onLinkDraw(ModContext*, void* args, void* retval, void*) {
    if (GET_CVAR_VALUE(g_hooks, Invisibility) == false) {
        return HOOK_CONTINUE;
    }

    return HOOK_SKIP_ORIGINAL;
}

ModResult HooksHandler::initialize(ConfigHandler* config_handler, ModError* error) {
    configHandler = config_handler;
    g_hooks = this;
    ModResult result;

    PRE_HOOK(LinkDraw, onLinkDraw, "Failed to hook daAlink_c::draw");
    POST_HOOK(SetDoubleAnime, onExecute, "Failed to hook daAlink_c::execute");

    return result;
}