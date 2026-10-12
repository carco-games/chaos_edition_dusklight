#include "hooks.hpp"
#include "color_hooks.hpp"
#include "d/actor/d_a_alink.h"
#include "d/d_camera.h"
#include "m_Do/m_Do_lib.h"
#include "d/actor/d_a_obj_item.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_bomb.h"
#include "d/actor/d_a_disappear.h"

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
DEFINE_HOOK(&dCamera_c::lockonCamera, LockonCamera);
DEFINE_HOOK(&dCamera_c::CalcTrimSize, CalcTrimSize);
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
static std::vector<s16> possibleSpawns = {
    fpcNm_NPC_COACH_e, fpcNm_NPC_THEB_e, fpcNm_NPC_KS_e, fpcNm_NPC_ZRA_e, fpcNm_NPC_TK_e, fpcNm_NPC_WORM_e, fpcNm_NPC_TKJ2_e, fpcNm_NPC_SQ_e, fpcNm_NPC_NE_e, fpcNm_NPC_TR_e, fpcNm_NPC_LF_e,
    fpcNm_NPC_FISH_e, fpcNm_NPC_DU_e, fpcNm_NPC_GND_e, fpcNm_NPC_GRA_e, fpcNm_NPC_GRC_e, fpcNm_NPC_GRD_e, fpcNm_NPC_GRM_e, fpcNm_NPC_GRMC_e, fpcNm_NPC_GRO_e, fpcNm_NPC_GRR_e, fpcNm_NPC_GRS_e,
    fpcNm_NPC_GRZ_e, fpcNm_NPC_YAMID_e, fpcNm_NPC_YAMIT_e, fpcNm_NPC_YAMIS_e, fpcNm_NPC_BLUENS_e, fpcNm_NPC_KAKASHI_e, fpcNm_NPC_ARU_e, fpcNm_NPC_BANS_e, fpcNm_NPC_BESU_e,
    fpcNm_NPC_BOU_e, fpcNm_NPC_BOU_S_e, fpcNm_NPC_CLERKA_e, fpcNm_NPC_CLERKB_e, fpcNm_NPC_CLERKT_e, fpcNm_NPC_DOC_e, fpcNm_NPC_GWOLF_e, fpcNm_NPC_LEN_e, fpcNm_NPC_LUD_e, fpcNm_NPC_FAIRY_SEIREI_e,
    fpcNm_NPC_FAIRY_e, fpcNm_NPC_HANJO_e, fpcNm_NPC_HENNA_e, fpcNm_NPC_HOZ_e, fpcNm_NPC_JAGAR_e, fpcNm_NPC_KKRI_e, fpcNm_NPC_KN_e,
    // finish list
};

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

    // Handle Random Spawns
    if (GET_CVAR_VALUE(g_hooks, RandoSpawns)) {

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
bool g_skipRouletteCheck = false;
HookAction onDamageMagnification(ModContext*, void* args, void* retval, void*) {
    if (GET_CVAR_VALUE(g_hooks, InstaKill) == false) {
        return HOOK_CONTINUE;
    }

    if (retval != nullptr) {
        if (g_skipRouletteCheck) {
            *static_cast<f32*>(retval) = 1.0f;
            g_skipRouletteCheck = false;
        } else {
            *static_cast<f32*>(retval) = 9999.0f;
            dBomb_c::createNormalBombExplode(&dComIfGp_getPlayer(0)->current.pos);
        }
    }

    return HOOK_SKIP_ORIGINAL;
}

// Damage Roulette
HookAction onSetDamagePoint(ModContext*, void* args, void* retval, void*) {
    if (g_skipRouletteCheck || !GET_CVAR_VALUE(g_hooks, DamageRoulette)) {
        return HOOK_CONTINUE;
    }

    static std::mt19937 rng{std::random_device{}()};
    if (std::uniform_int_distribution<>(1, 100)(rng) <= GET_CVAR_VALUE(g_hooks, DamageRouletteOdds)) {
        int i_dmgAmount = mods::arg<int>(args, 1);
        if (i_dmgAmount > 0) {
            dComIfGp_setItemLifeCount(i_dmgAmount * 2, 0);
        }
    }

    return HOOK_CONTINUE;
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
                g_skipRouletteCheck = true;
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
                g_skipRouletteCheck = true;
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

// Zoom Out / Top-Down
void onCameraRun(ModContext*, void* args, void*, void*) {
    if (!GET_CVAR_VALUE(g_hooks, ZoomOut) && !GET_CVAR_VALUE(g_hooks, TopDown)) {
        return;
    }

    dCamera_c* camera = mods::arg<dCamera_c*>(args, 0);
    f32 scale = 1.015f;
    if (GET_CVAR_VALUE(g_hooks, ZoomOut)) {
        scale *= ((f32)GET_CVAR_VALUE(g_hooks, ZoomOutPercent) / 100.0f);
        camera->mEye.z *= scale;
    } else if (GET_CVAR_VALUE(g_hooks, TopDown)) {
        camera->mEye.y = dComIfGp_getPlayer(0)->eyePos.y + 1500.0f * (GET_CVAR_VALUE(g_hooks, TopDownPercent) / 100.0f);
        camera->mCenter = dComIfGp_getPlayer(0)->current.pos;
    }
}

HookAction onLockonCamera(ModContext*, void* args, void*, void*) {
    if (GET_CVAR_VALUE(g_hooks, TopDown)) {
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
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
uint32_t g_enemyID = 0;
void onProcFrontRollCrashInit(ModContext*, void* args, void*, void*) {
    daAlink_c* link = mods::arg<daAlink_c*>(args, 0);
    if (g_hooks->getBonkShuffleReady()) {
        g_hooks->getConfigHandler()->setShuffle(true);
        g_hooks->setBonkShuffleReady(false);
    }

    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist(0, possibleSpawns.size() - 1);
    s16 randomSpawn = possibleSpawns[dist(rng)];
    mods::log::debug("Spawning actor: {}", randomSpawn);
    fopAcM_create(randomSpawn, 0, &link->current.pos, fopAcM_GetRoomNo(link), NULL, NULL, -1);
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
    PRE_HOOK(SetDamagePoint, onSetDamagePoint, "Failed to hook daAlink_c::setDamagePoint");
    POST_HOOK(SetMatrix, onSetMatrix, "Failed to hook daAlink_c::setMatrix");
    PRE_HOOK(SetIceSlipSpeed, onSetIceSlipSpeed, "Failed to hook daAlink_c::setIceSlipSpeed");
    POST_HOOK(Execute, onExecute, "Failed to hook daAlink_c::execute");
    POST_HOOK(CameraRun, onCameraRun, "Failed to hook dCamera_c::Run");
    PRE_HOOK(LockonCamera, onLockonCamera, "Failed to hook dCamera_c::lockonCamera");
    PRE_HOOK(CalcTrimSize, onLockonCamera, "Failed to hook dCamera_c::CalcTrimSize");
    POST_HOOK(ItemGet, onItemGet, "Failed to hook daItem_c::itemGet");
    PRE_HOOK(CreateDisappear, onCreateDisappear, "Failed to hook fopAcM_createDisappear");
    POST_HOOK(ProcFrontRollCrashInit, onProcFrontRollCrashInit, "Failed to hook daAlink_c::procFrontRollCrashInit");

    return result;
}