#pragma once

// Enemy Headers
#include "d/actor/d_a_e_ai.h"
#include "d/actor/d_a_e_ba.h"
#include "d/actor/d_a_e_bs.h"
#include "d/actor/d_a_e_bu.h"
#include "d/actor/d_a_e_db.h"
#include "d/actor/d_a_e_dd.h"
#include "d/actor/d_a_e_df.h"
#include "d/actor/d_a_e_dk.h"
#include "d/actor/d_a_e_dn.h"
#include "d/actor/d_a_e_dt.h"
#include "d/actor/d_a_e_fb.h"
#include "d/actor/d_a_e_fs.h"
#include "d/actor/d_a_e_gb.h"
#include "d/actor/d_a_e_ge.h"
#include "d/actor/d_a_e_gi.h"
#include "d/actor/d_a_e_gm.h"
#include "d/actor/d_a_e_hm.h"
#include "d/actor/d_a_e_hp.h"
#include "d/actor/d_a_e_hz.h"
#include "d/actor/d_a_e_kg.h"
#include "d/actor/d_a_e_kk.h"
#include "d/actor/d_a_e_kr.h"
#include "d/actor/d_a_e_mf.h"
#include "d/actor/d_a_e_mk.h"
#include "d/actor/d_a_e_mm.h"
#include "d/actor/d_a_e_ms.h"
#include "d/actor/d_a_e_nz.h"
#include "d/actor/d_a_e_oc.h"
#include "d/actor/d_a_e_po.h"
#include "d/actor/d_a_e_pz.h"
#include "d/actor/d_a_e_rb.h"
#include "d/actor/d_a_e_rd.h"
#include "d/actor/d_a_e_rdy.h"
#include "d/actor/d_a_e_s1.h"
#include "d/actor/d_a_e_sb.h"
#include "d/actor/d_a_e_sf.h"
#include "d/actor/d_a_e_sg.h"
#include "d/actor/d_a_e_sh.h"
#include "d/actor/d_a_e_sm.h"
#include "d/actor/d_a_e_st.h"
#include "d/actor/d_a_e_sw.h"
#include "d/actor/d_a_e_th.h"
#include "d/actor/d_a_e_tt.h"
#include "d/actor/d_a_e_vt.h"
#include "d/actor/d_a_e_ws.h"
#include "d/actor/d_a_e_ww.h"
#include "d/actor/d_a_e_yd.h"
#include "d/actor/d_a_e_yg.h"
#include "d/actor/d_a_e_yh.h"
#define ACT_ATTACK ACT_ATTACK_YK
#include "d/actor/d_a_e_yk.h"
#undef ACT_ATTACK
#include "d/actor/d_a_e_ym.h"
#include "d/actor/d_a_e_ymb.h"
#include "d/actor/d_a_e_yr.h"
#include "d/actor/d_a_e_zm.h"
#include "d/actor/d_a_e_zs.h"

// Helper for tile worm fall logic
// Checks same conditions that causes fopAcM_createDisappear to be called when tile worm is falling
bool checkTileWormFall(daE_HZ_c* i_this) {
    dBgS_GndChk gnd_chk;
    cXyz modified_pos(i_this->current.pos);
    modified_pos.y += 300.0f;
    gnd_chk.SetPos(&modified_pos);

    f32 ground_cross = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (i_this->speed.y < 0.0f && ground_cross != -G_CM3D_F_INF) {
        s32 gnd_chk_ground_code = dComIfG_Bgsp().GetGroundCode(gnd_chk);
        if ((gnd_chk_ground_code == 4 || gnd_chk_ground_code == 10 || gnd_chk_ground_code == 5) &&
            ground_cross + 500.0f > i_this->current.pos.y) {
            return true;
        }
    }

    return false;
}

#define ENEMY_DEATH_FUNCS(X)                                        \
    X(&e_ai_class::e_ai_damage, e_ai_class, ArmosDeath,             \
        i_this->field_0x692 == 0 && i_this->m_mode == 1 &&          \
        i_this->m_timers[2] == 1)                                   \
    X(&daE_DF_c::BombEatAction, daE_DF_c, DekuFlowerDeath,          \
        i_this->mEatStep == 3 && i_this->mpMorfSO->isStop() &&      \
        i_this->mAnim == daE_DF_c::ANIM_DIE)                        \
    X(&daE_DK_c::executeDeath, daE_DK_c, BariDeath,                 \
        i_this->mMoveMode == 3 && i_this->field_0x69c == 1)         \
    X(&daE_DT_c::executeDeath, daE_DT_c, DekuToadDeath,             \
        i_this->mMode == 4 && i_this->mTimer == 0)                  \
    X(&daE_FB_c::dead_eff_set, daE_FB_c, FreezardDeath, true)       \
    X(&daE_GE_c::executeDown, daE_GE_c, GuayDeath,                  \
        (i_this->mMode == 1 && i_this->field_0xb8e[0] == 0) ||      \
        (i_this->mMode == 4 && i_this->field_0xb8e[0] == 0))        \
    X(&daE_GI_c::executeDamage, daE_GI_c, GibdoDeath,               \
        i_this->mMoveMode == 11 && i_this->mpModelMorf->isStop())   \
    X(&daE_GM_c::core_delwait, daE_GM_c, BabyGohmaDeath,            \
        i_this->mMode == 0)                                         \
    X(&daE_HM_c::W_DeathAction, daE_HM_c, TorchSlugW_Death,         \
        i_this->field_0x5da > 1 && i_this->mAnm_p->isStop())        \
    X(&daE_HM_c::DeathMotion, daE_HM_c, TorchSlugDeathMotion,       \
        i_this->field_0x5da > 1 && i_this->mAnm_p->isStop())        \
    X(&daE_HM_c::DeathCutDown, daE_HM_c, TorchSlugDeathCutDown,     \
        i_this->mAnm_p->isStop())                                   \
    X(&daE_HP_c::executeDead, daE_HP_c, HugePoeDeath,               \
        i_this->movemode == 1 && i_this->field_0x788 != 0           \
        || i_this->mItemCheckHandled)                               \
    X(&daE_HZ_c::checkFall, daE_HZ_c, TileWormFall,                 \
        checkTileWormFall(i_this))                                  \
    X(&daE_HZ_c::executeDeath, daE_HZ_c, TileWormDeath,             \
        i_this->mMode == 2 && i_this->mpMorfSO->isStop())           \
    X(&daE_HZ_c::executeWaterDeath, daE_HZ_c, TileWormWaterDeath,   \
        i_this->mMode == 2 && i_this->mpMorfSO->isStop())           \
    X(&daE_KK_c::executeDead, daE_KK_c, ChilfosDeath,               \
        i_this->mMoveMode == 1 && i_this->mpMorfSO->isStop())       \
    X(&daE_OC_c::executeDeath, daE_OC_c, BokoblinDeath,             \
        i_this->mOcState == 2 && i_this->field_0x6c0 == 0)          \
    X(&daE_OC_c::executeWaterDeath, daE_OC_c, BokoblinWaterDeath,   \
        i_this->mOcState == 2 && i_this->mpMorf->isStop())          \
    X(&daE_PZ_c::executeDead, daE_PZ_c, PhantomZantDeath,           \
        i_this->mMoveMode == 3 && i_this->field_0x7d0 == 0)         \
    X(&daE_SB_c::Death_Motion, daE_SB_c, ShellBladeDeath, true)     \
    X(&daE_SM_c::E_SM_C_Death, daE_SM_c, ChuWormDeath,              \
        i_this->field_0x6b4 == 2 && i_this->mpModelMorf->isStop() &&\
        i_this->field_0x684)                                        \
    X(&daE_SW_c::executeDie, daE_SW_c, MoldormDeath,                \
        i_this->mMoveMode == 2 && i_this->mpModelMorf->isStop() &&  \
        i_this->field_0x6ea == 0)                                   \
    X(&daE_SW_c::d_die, daE_SW_c, MoldormDDeath,                    \
        i_this->mMoveMode == 2 && i_this->mpModelMorf->isStop() &&  \
        i_this->field_0x6ea == 0)                                   \
    X(&daE_TT_c::executeDeath, daE_TT_c, TektiteDeath,              \
        i_this->mMode == 6 && i_this->mGenericTimer == 0)           \
    X(&daE_VA_c::executeOpaciDeath, daE_VA_c, DeathSwordDeath,      \
        i_this->mMode == 2 && i_this->mpMorf->isStop())             \
    X(&daE_WS_c::executeDown, daE_WS_c, WallSkulltulaDownDeath,     \
        i_this->mMode == 5 && i_this->mWaitTimer == 0)              \
    X(&daE_WS_c::executeWindDown, daE_WS_c, WallSkulltulaWindDeath, \
        i_this->mMode == 5 && i_this->mWaitTimer == 0)              \
    X(&daE_WW_c::executeDamage, daE_WW_c, WhiteWolfosDeath,         \
        i_this->mActionMode == 6 && i_this->mpModelMorf->isStop())  \
    X(&daE_YM_c::executeDown, daE_YM_c, TwInsectDeath,              \
        i_this->mMode == 3 && i_this->mpMorf->isStop())             \
    X(&daE_YMB_c::setDeathEffect, daE_YMB_c, TwInsectBossDeath,     \
        true)                                                       \
    X(&daE_ZM_c::executeDead, daE_ZM_c, ZantMaskDeath,              \
        i_this->mMode == 3 && i_this->mpModelMorf->isStop())        \
    X(&daE_ZS_c::executeDamage, daE_ZS_c, StaltroopDeath,           \
        i_this->mMode == 3 && i_this->mpMorf->isStop())             \

#define ENEMY_STATIC_DEATH_FUNCS(X)                                                 \
    X("src/d/actor/d_a_e_ba.cpp#ba_disappear", fopAc_ac_c,                          \
        KeeseDeath, void(fopAc_ac_c*), true, i_this->current.pos)                   \
    X("src/d/actor/d_a_e_ba.cpp#action", e_ba_class, KeeseActionDeath,              \
        void(e_ba_class*), i_this->mIsDying && (i_this->mKnockbackSpeed <= 1.0f ||  \
        i_this->mAcch.ChkWallHit()) && i_this->mHomeType == e_ba_class::HOME_APPEAR,\
        i_this->mEnemy.current.pos)                                                 \
    X("src/d/actor/d_a_e_bu.cpp#bu_disappear", fopAc_ac_c,                          \
        BubbleDeath, void(fopAc_ac_c*), true, i_this->current.pos)                  \
    X("src/d/actor/d_a_e_db.cpp#e_db_e_dead", e_db_class,                           \
        DekuBabaDeath, void(e_db_class*), i_this->mode == 4 &&                      \
        i_this->timers[0] == 0, i_this->enemy.current.pos)                          \
    X("src/d/actor/d_a_e_dd.cpp#daE_DD_Execute", e_dd_class,                        \
        DodongoDeath, int(e_dd_class*), i_this->field_0x5b6 != 0,                   \
        i_this->actor.current.pos)                                                  \
    X("src/d/actor/d_a_e_dn.cpp#dn_disappear", e_dn_class,                          \
        LizalfosDeath, void(e_dn_class*), true, i_this->actor.current.pos)          \
    X("src/d/actor/d_a_e_fs.cpp#e_fs_end", e_fs_class,                              \
        WoodenPuppetDeath, void(e_fs_class*), i_this->mMode == 1 &&                 \
        i_this->mpMorf->isStop(), i_this->mEnemy.current.pos)                       \
    X("src/d/actor/d_a_e_gb.cpp#e_gb_end", e_gb_class,                              \
        GiantBabaDeath, void(e_gb_class*), i_this->mode == 2 &&                     \
        (i_this->frameCounter & 1) == 0 && i_this->stemModelIdx == 17,              \
        i_this->actor.current.pos)                                                  \
    X("src/d/actor/d_a_e_gb.cpp#e_gf_end", e_gb_class,                              \
        GiantBabaFlowerDeath, void(e_gb_class*), i_this->flowerMode == 3 &&         \
        i_this->timer[0] == 0, i_this->actor.current.pos)                           \
    X("src/d/actor/d_a_e_kg.cpp#e_kg_damage", e_kg_class,                           \
        YoungGohmaDeath, void(e_kg_class*), i_this->field_0x678 == 3 &&             \
        i_this->field_0x694[0] == 0, i_this->current.pos)                           \
    X("src/d/actor/d_a_e_kr.cpp#e_kr_damage", e_kr_class,                           \
        KargarokDeath, s8(e_kr_class*), i_this->enemy.health <= 0 &&                \
        (i_this->field_0x69c[0] == 0x14 || i_this->field_0x69c[1] == 0),            \
        i_this->enemy.current.pos)                                                  \
    X("src/d/actor/d_a_e_mf.cpp#mf_disappear", e_mf_class,                          \
        DynalfosDeath, void(e_mf_class*), true, i_this->actor.current.pos)          \
    X("src/d/actor/d_a_e_mk.cpp#demo_camera_end", e_mk_class,                       \
        OokCrownDeath, void(e_mk_class*), i_this->demoSubMode == 6 &&               \
        i_this->demoCamCounter == 180, i_this->actor.current.pos)                   \
    X("src/d/actor/d_a_e_mm.cpp#e_mm_damage", e_mm_class,                           \
        HelmasaurDeath, void(e_mm_class*), i_this->mode == 2 &&                     \
        i_this->timers[0] == 0, i_this->enemy.current.pos)                          \
    X("src/d/actor/d_a_e_ms.cpp#ms_disappear", e_ms_class,                          \
        RatDeath, void(e_ms_class*), true, i_this->current.pos)                     \
    X("src/d/actor/d_a_e_nz.cpp#e_nz_damage", e_nz_class,                           \
        GhoulRatDeath, void(e_nz_class*), i_this->mSubAction == 1 &&                \
        i_this->field_0x6a2[0] == 0, i_this->enemy.current.pos)                     \
    X("src/d/actor/d_a_e_po.cpp#e_po_dead", e_po_class,                             \
        PoeDeath, void(e_po_class*), i_this->mType == 50, i_this->enemy.current.pos)\
    X("src/d/actor/d_a_e_rb.cpp#e_rb_damage", e_rb_class,                           \
        LeeverDeath, void(e_rb_class*), i_this->mode == 2 &&                        \
        i_this->timers[0] == 0, i_this->enemy.current.pos)                          \
    X("src/d/actor/d_a_e_rd.cpp#rd_disappear", e_rd_class,                          \
        BulblinDeath, void(e_rd_class*), true, i_this->enemy.current.pos)           \
    X("src/d/actor/d_a_e_rdy.cpp#rd_disappear", e_rdy_class,                        \
        ShadowBulblinDeath, void(e_rdy_class*), true, i_this->actor.current.pos)    \
    X("src/d/actor/d_a_e_s1.cpp#e_s1_fail", e_s1_class,                             \
        ShadowBeastDeath, void(e_s1_class*), i_this->mMode == 11 &&                 \
        i_this->mTimers[0] == 0 && i_this->mDemoMode == 0, i_this->current.pos)     \
    X("src/d/actor/d_a_e_sf.cpp#crash_eff", e_sf_class,                             \
        StalfosDeath, void(e_sf_class*), i_this->mAction == 30 ||                   \
        i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB),                        \
        i_this->actor.current.pos)                                                  \
    X("src/d/actor/d_a_e_sg.cpp#e_sg_damage", e_sg_class,                           \
        SkullfishDeath, void(e_sg_class*), i_this->speedF == 0.0f,                  \
        i_this->current.pos)                                                        \
    X("src/d/actor/d_a_e_sh.cpp#e_sh_dead", e_sh_class,                             \
        StalhoundDeath, void(e_sh_class*), i_this->field_0x678 == 2 &&              \
        i_this->field_0x698[0] == 0, i_this->enemy.current.pos)                     \
    X("src/d/actor/d_a_e_st.cpp#e_st_damage", e_st_class,                           \
        SkulltulaDeath, void(e_st_class*), i_this->mActionPhase == 5 &&             \
        i_this->mBgc.ChkGroundHit() && i_this->mAnm == 13 &&                        \
        i_this->mTimers[0] == 0, i_this->actor.current.pos)                         \
    X("src/d/actor/d_a_e_st.cpp#e_st_water", e_st_class,                            \
        SkulltulaWaterDeath, void(e_st_class*), i_this->mActionPhase == 1 &&        \
        i_this->mTimers[0] == 0, i_this->actor.current.pos)                         \
    X("src/d/actor/d_a_e_st.cpp#e_st_g_end", e_st_class,                            \
        SkulltulaGDeath, s8(e_st_class*), (i_this->mActionPhase == 2 ||             \
        i_this->mActionPhase == 11) && i_this->mpModelMorf->isStop(),               \
        i_this->actor.current.pos)                                                  \
    X("src/d/actor/d_a_e_th.cpp#demo_camera", e_th_class,                           \
        DarkhammerDeath, s8(e_th_class*), i_this->mDemoCamMode == 12 &&             \
        i_this->mDemoCamTimer == 1, i_this->current.pos)                            \
    X("src/d/actor/d_a_e_yd.cpp#yd_disappear", e_yd_class,                          \
        TwDekuBabaDeath, void(e_yd_class*), true, i_this->actor.current.pos)        \
    X("src/d/actor/d_a_e_yg.cpp#ms_disappear", e_yg_class,                          \
        TwVerminDeath, void(e_yg_class*), true, i_this->actor.current.pos)          \
    X("src/d/actor/d_a_e_yh.cpp#yh_disappear", e_yh_class,                          \
        TwHebiBabaDeath, void(e_yh_class*), true, i_this->enemy.current.pos)        \
    X("src/d/actor/d_a_e_yk.cpp#yk_disappear", e_yk_class,                          \
        TwKeeseDeath, void(e_yk_class*), true, i_this->current.pos)                 \
    X("src/d/actor/d_a_e_yr.cpp#yr_disappear", e_yr_class,                          \
        TwKargorokDeath, void(e_yr_class*), true, i_this->mEnemy.current.pos)       \

#define ENEMY_DEATH_HOOK(symbol, class, alias, condition)   \
    DEFINE_HOOK(symbol, alias);

#define ENEMY_STATIC_DEATH_HOOK(symbol, class, alias, params, condition, pos)   \
    DEFINE_HOOK_SYMBOL(symbol, params, alias);

#define ENEMY_DEATH_HOOK_FUNC(symbol, class, alias, condition)  \
    void on##alias(ModContext*, void* args, void*, void*) {     \
        onDeath<class>(args, 0,                                 \
            [](class* i_this) { return (condition); },          \
            [](class* i_this) { return i_this->current.pos; }); \
    }

#define ENEMY_STATIC_DEATH_HOOK_FUNC(symbol, class, alias, params, condition, pos)  \
    void on##alias(ModContext*, void* args, void*, void*) {                         \
        onDeath<class>(args, 1,                                                     \
            [](class* i_this) { return (condition); },                              \
            [](class* i_this) { return (pos); });                                   \
    }