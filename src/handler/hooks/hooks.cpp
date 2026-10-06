#include "hooks.hpp"
#include "color_hooks.hpp"
#include "d/actor/d_a_alink.h"

IMPORT_SERVICE(HookService, svc_hook);

HooksHandler* g_hooks;

DEFINE_HOOK(&daAlink_c::draw, LinkDraw);
DEFINE_HOOK(&daAlink_c::procMove, LinkProcMove);
DEFINE_HOOK(&daAlink_c::procFrontRollInit, LinkFrontRollInit);
DEFINE_HOOK(&daAlink_c::damageMagnification, LinkDamageMagnification);

// Fast Link
void onProcMove(ModContext*, void* args, void*, void*) {
    daAlink_c* link = mods::arg<daAlink_c*>(args, 0);
    if (GET_CVAR_VALUE(g_hooks, FastLink) == true) {
        link->mNormalSpeed += 2 * GET_CVAR_VALUE(g_hooks, FastLinkSpeed);
    }
}

// Replace function for front roll init. Replace is used to direclty modify roll speed inside the function.
void onProcFrontRollInit(ModContext*, void* args, void* retval, void*) {
    daAlink_c* link = mods::arg<daAlink_c*>(args, 0);
    BOOL is_guard_anime = link->checkUpperGuardAnime();
    f32 rollSpeedMultiplier;
    if (GET_CVAR_VALUE(g_hooks, FastLink) == true) {
        rollSpeedMultiplier = 3 * GET_CVAR_VALUE(g_hooks, FastLinkSpeed);
    } else {
        rollSpeedMultiplier = 1.0f;
    }

    if (link->mProcID == daAlink_c::PROC_FRONT_ROLL && link->mDemo.getDemoMode() == daPy_demo_c::DEMO_FRONT_ROLL_e) {
        if (retval != nullptr) {
            *static_cast<int*>(retval) = 0;
        }
    }

    BOOL is_dive_jump = link->mProcID == daAlink_c::PROC_DIVE_JUMP;
    link->commonProcInit(daAlink_c::PROC_FRONT_ROLL);

    f32 roll_anm_speed;
    if (is_dive_jump) {
        roll_anm_speed = 6.0f;
    } else {
        roll_anm_speed = link->mpHIO->mFrontRoll.m.mRollAnm.mStartFrame;
    }

    link->setSingleAnime(daAlink_c::ANM_FRONT_ROLL, link->mpHIO->mFrontRoll.m.mRollAnm.mSpeed * rollSpeedMultiplier, roll_anm_speed,
                         link->mpHIO->mFrontRoll.m.mRollAnm.mEndFrame, link->mpHIO->mFrontRoll.m.mRollAnm.mInterpolation);

    link->mNormalSpeed = link->speedF * link->mpHIO->mFrontRoll.m.mSpeedRate + link->mpHIO->mFrontRoll.m.mInitSpeed;

    f32 max_speed = link->mpHIO->mFrontRoll.m.mInitSpeed + link->mpHIO->mMove.m.mMaxSpeed * link->mpHIO->mFrontRoll.m.mSpeedRate;
    if (link->mNormalSpeed > max_speed) {
        link->mNormalSpeed = max_speed;
    }

    if (link->mNormalSpeed < link->mpHIO->mFrontRoll.m.mMinSpeed) {
        link->mNormalSpeed = link->mpHIO->mFrontRoll.m.mMinSpeed;
    }

    if (link->checkNoResetFlg0(daAlink_c::FLG0_WATER_IN_MOVE)) {
        link->mNormalSpeed *= link->mpHIO->mItem.mIronBoots.m.mWaterVelocityX;
    } else if (link->checkHeavyStateOn(TRUE, TRUE)) {
        link->mNormalSpeed *= link->mHeavySpeedMultiplier;
    }

    link->mNormalSpeed *= rollSpeedMultiplier;

    link->current.angle.y = link->shape_angle.y;
    link->voiceStart(Z2SE_AL_V_BACKTEN);
    link->mProcVar2.field_0x300c = 0;
    link->setFootEffectProcType(0);

    if (is_guard_anime) {
        link->onNoResetFlg0(daAlink_c::FLG0_UNK_2);
    }

    if (link->checkEquipHeavyBoots()) {
        link->mAtSph.SetAtType(AT_TYPE_HEAVY_BOOTS);
        link->mAtSph.SetAtHitMark(1);
        link->mAtSph.SetAtSe(dCcD_SE_METAL);
        link->mAtSph.SetAtAtp(2);
        link->mAtSph.SetAtMtrl(dCcD_MTRL_NONE);
        link->mAtSph.ResetAtHit();
        link->mAtSph.SetR(link->mpHIO->mFrontRoll.m.mBootsAttackRadius);
        link->mAtSph.OnAtSetBit();
    }

        if (retval != nullptr) {
            *static_cast<int*>(retval) = 1;
        }
}

// Invisibility
HookAction onLinkDraw(ModContext*, void* args, void*, void*) {
    if (GET_CVAR_VALUE(g_hooks, Invisibility) == false) {
        return HOOK_CONTINUE;
    }

    return HOOK_SKIP_ORIGINAL;
}

// Insta Kills
HookAction onDamageMagnification(ModContext*, void* args, void* retval, void*) {
    if (GET_CVAR_VALUE(g_hooks, InstaKill) == false) {
        return HOOK_CONTINUE;
    }

    if (retval != nullptr) {
        *static_cast<f32*>(retval) = 9999.0f;
    }

    return HOOK_SKIP_ORIGINAL;
}



ModResult HooksHandler::initialize(ConfigHandler* config_handler, ModError* error) {
    configHandler = config_handler;
    g_hooks = this;
    colorHooks = new ColorHooks();
    colorHooks->initialize(config_handler, error);
    ModResult result;

    PRE_HOOK(LinkDraw, onLinkDraw, "Failed to hook daAlink_c::draw");
    POST_HOOK(LinkProcMove, onProcMove, "Failed to hook daAlink_c::procMove");
    REPLACE_HOOK(LinkFrontRollInit, onProcFrontRollInit, "Failed to hook daAlink_c::procFrontRollInit");
    PRE_HOOK(LinkDamageMagnification, onDamageMagnification, "Failed to hook daAlink_c::damageMagnification");

    return result;
}