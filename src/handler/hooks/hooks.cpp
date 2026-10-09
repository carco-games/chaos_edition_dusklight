#include "hooks.hpp"
#include "color_hooks.hpp"
#include "d/actor/d_a_alink.h"
#include "d/d_camera.h"
#include "m_Do/m_Do_lib.h"
#include "d/actor/d_a_obj_item.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_bomb.h"
#include "d/actor/d_a_disappear.h"
#include "d/actor/d_a_e_oc.h"

IMPORT_SERVICE(HookService, svc_hook);

HooksHandler* g_hooks;

DEFINE_HOOK(&daAlink_c::draw, LinkDraw);
DEFINE_HOOK(&daAlink_c::procMove, LinkProcMove);
DEFINE_HOOK(&daAlink_c::procFrontRollInit, LinkFrontRollInit);
DEFINE_HOOK(&daAlink_c::damageMagnification, LinkDamageMagnification);
DEFINE_HOOK(&daAlink_c::setDamagePoint, SetDamagePoint);
DEFINE_HOOK(&daAlink_c::modelCallBack, SetMatrix);
DEFINE_HOOK(&daAlink_c::setIceSlipSpeed, SetIceSlipSpeed);
DEFINE_HOOK(&daAlink_c::execute, Execute);
DEFINE_HOOK(&dCamera_c::Run, CameraRun);
DEFINE_HOOK(&daItem_c::itemGet, ItemGet);
DEFINE_HOOK(fopAcM_createDisappear, CreateDisappear);
DEFINE_HOOK(&daAlink_c::procFrontRollCrashInit, ProcFrontRollCrashInit);

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

// Damage Roulette
void onSetDamagePoint(ModContext*, void* args, void* retval, void*) {
    if (GET_CVAR_VALUE(g_hooks, DamageRoulette) == false) {
        return;
    }

    static std::mt19937 rng{std::random_device{}()};
    if (std::uniform_int_distribution<>(1, 100)(rng) <= GET_CVAR_VALUE(g_hooks, DamageRouletteOdds)) {
        int i_dmgAmount = mods::arg<int>(args, 1);
        if (i_dmgAmount > 0) {
            dComIfGp_setItemLifeCount(i_dmgAmount * 2, 0);
        }
    }
}

// Tiny / Big Link
void onSetMatrix(ModContext*, void* args, void*, void*) {
    if (GET_CVAR_VALUE(g_hooks, TinyLink) == false && GET_CVAR_VALUE(g_hooks, BigLink) == false) {
        return;
    }

    daAlink_c* link = mods::arg<daAlink_c*>(args, 0);
    int i_jointNo = mods::arg<int>(args, 1);
    f32 scale;
    if (GET_CVAR_VALUE(g_hooks, TinyLink) == true) {
        scale = 0.7f / GET_CVAR_VALUE(g_hooks, TinyLinkScale);
    } else if (GET_CVAR_VALUE(g_hooks, BigLink) == true) {
        scale = 1.2f * GET_CVAR_VALUE(g_hooks, BigLinkScale);
    }

    MtxP mtx = link->mpLinkModel->getAnmMtx(i_jointNo);
    mDoMtx_stack_c::copy(mtx);
    mDoMtx_stack_c::scaleM(scale, scale, scale);
    cMtx_copy(mDoMtx_stack_c::get(), mtx);
}

// Arthritis / Health Drain
int64_t g_invulnerableTimer = 0;
int64_t g_healthDrainTimer = 0;
void onExecute(ModContext*, void* args, void*, void*) {
    if (GET_CVAR_VALUE(g_hooks, Arthritis) == false && GET_CVAR_VALUE(g_hooks, HealthDrain) == false) {
        return;
    }

    daAlink_c* link = mods::arg<daAlink_c*>(args, 0);
    if (GET_CVAR_VALUE(g_hooks, Arthritis)) {
        if (link->speed.x >= 15.0f || link->speed.x <= -15.0f || link->speed.z >= 15.0f || link->speed.z <= -15.0f) {
            if (g_invulnerableTimer == 0) {
                link->setDamagePoint(1, FALSE, FALSE, 0);
                g_invulnerableTimer = 60;
            }
        }

        if (g_invulnerableTimer != 0) {
            g_invulnerableTimer--;
        }
    }

    if (GET_CVAR_VALUE(g_hooks, HealthDrain)) {
        if (g_healthDrainTimer != 0) {
            g_healthDrainTimer--;
            if (g_healthDrainTimer == 0) {
                link->setDamagePoint(GET_CVAR_VALUE(g_hooks, HealthDrainAmount), FALSE, FALSE, 0);
                g_healthDrainTimer = GET_CVAR_VALUE(g_hooks, HealthDrainTimer);
            }
        }
    }
}

// Slippery
HookAction onSetIceSlipSpeed(ModContext*, void* args, void*, void*) {
    if (GET_CVAR_VALUE(g_hooks, Slippery) == false) {
        return HOOK_CONTINUE;
    }

    daAlink_c* link = mods::arg<daAlink_c*>(args, 0);
    f32 var_f31, var_f30, var_f29;
    if (link->checkWolf()) {
        var_f31 = 0.1f;
        var_f30 = 1.0f;
        var_f29 = 0.5f;
    } else {
        var_f31 = 0.03f;
        var_f30 = 0.5f;
        var_f29 = 0.75f;
    }

    cLib_addCalc(&link->field_0x35c4.x, 0.0f, var_f31, 100.0f, var_f30);
    cLib_addCalc(&link->field_0x35c4.z, 0.0f, var_f31, 100.0f, var_f30);

    cXyz sp58 = link->field_0x3528 - link->speed;
    f32 var_f28 = sp58.absXZ();
    if (var_f28 > 30.0f) {
        sp58 *= 30.0f / var_f28;
    }

    link->field_0x35c4 += sp58 * var_f29;
    link->speed += (sp58 * (1.0f - var_f29)) * GET_CVAR_VALUE(g_hooks, SlipperyScale);

    if (link->mProcVar4.field_0x3010 != 0 && (link->mProcID == daAlink_c::PROC_LARGE_DAMAGE_UP || link->mProcID == daAlink_c::PROC_WOLF_LARGE_DAMAGE_UP)) {
        sp58.normalizeZP();
        link->field_0x35c4 += sp58 * 20.0f;
    }

    return HOOK_SKIP_ORIGINAL;
}

// Zoom Out
void onCameraRun(ModContext*, void* args, void*, void*) {
    if (GET_CVAR_VALUE(g_hooks, ZoomOut) == false) {
        return;
    }

    dCamera_c* camera = mods::arg<dCamera_c*>(args, 0);
    f32 scale = 1.015f;
    scale *= ((f32)GET_CVAR_VALUE(g_hooks, ZoomOutPercent) / 100.0f);
    camera->mEye.z *= scale;
}

// Spin

// Drops Damage
void onItemGet(ModContext*, void* args, void*, void*) {
    if (!GET_CVAR_VALUE(g_hooks, DropsDamage)) {
        return;
    }

    daItem_c* item = mods::arg<daItem_c*>(args, 0);
    f32 dmgAmount = 4.0f;
    if (item->m_itemNo == dItemNo_HEART_e) {
        dmgAmount = 8.0f;
    }
    dComIfGp_setItemLifeCount(-4.0f, 0);
}

// Exploding Enemies
HookAction onCreateDisappear(ModContext*, void* args, void*, void*) {
    if (!GET_CVAR_VALUE(g_hooks, ExplodeEnemies)) {
        return HOOK_CONTINUE;
    }

    g_hooks->getConfigHandler()->addExplosion(dComIfGp_getPlayer(0)->current.pos);
    return HOOK_CONTINUE;
}

// Bonk Shuffle
void onProcFrontRollCrashInit(ModContext*, void* args, void*, void*) {
    if (g_hooks->getBonkShuffleReady()) {
        g_hooks->getConfigHandler()->setShuffle(true);
        g_hooks->setBonkShuffleReady(false);
    }
}

ModResult HooksHandler::initialize(ConfigHandler* config_handler, ModError* error) {
    configHandler = config_handler;
    g_hooks = this;
    colorHooks = new ColorHooks();
    colorHooks->initialize(config_handler, error);
    ModResult result;

    // Set Health Drain Timer
    g_healthDrainTimer = GET_CVAR_VALUE(g_hooks, HealthDrainTimer);

    PRE_HOOK(LinkDraw, onLinkDraw, "Failed to hook daAlink_c::draw");
    POST_HOOK(LinkProcMove, onProcMove, "Failed to hook daAlink_c::procMove");
    REPLACE_HOOK(LinkFrontRollInit, onProcFrontRollInit, "Failed to hook daAlink_c::procFrontRollInit");
    PRE_HOOK(LinkDamageMagnification, onDamageMagnification, "Failed to hook daAlink_c::damageMagnification");
    POST_HOOK(SetDamagePoint, onSetDamagePoint, "Failed to hook daAlink_c::setDamagePoint");
    POST_HOOK(SetMatrix, onSetMatrix, "Failed to hook daAlink_c::setMatrix");
    PRE_HOOK(SetIceSlipSpeed, onSetIceSlipSpeed, "Failed to hook daAlink_c::setIceSlipSpeed");
    POST_HOOK(Execute, onExecute, "Failed to hook daAlink_c::execute");
    POST_HOOK(CameraRun, onCameraRun, "Failed to hook dCamera_c::Run");
    POST_HOOK(ItemGet, onItemGet, "Failed to hook daItem_c::itemGet");
    PRE_HOOK(CreateDisappear, onCreateDisappear, "Failed to hook fopAcM_createDisappear");
    POST_HOOK(ProcFrontRollCrashInit, onProcFrontRollCrashInit, "Failed to hook daAlink_c::procFrontRollCrashInit");

    return result;
}