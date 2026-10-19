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

#include <type_traits>
#include <memory>

#define ENEMY_DEATH_FUNCS(X)                                        \
    X(&e_ai_class::e_ai_damage, e_ai_class, ArmosDeath)             \
    X(&daE_DF_c::BombEatAction, daE_DF_c, DekuFlowerDeath)          \
    X(&daE_DK_c::executeDeath, daE_DK_c, BariDeath)                 \
    X(&daE_DT_c::executeDeath, daE_DT_c, DekuToadDeath)             \
    X(&daE_FB_c::dead_eff_set, daE_FB_c, FreezardDeath)             \
    X(&daE_GE_c::executeDown, daE_GE_c, GuayDeath)                  \
    X(&daE_GI_c::executeDamage, daE_GI_c, GibdoDeath)               \
    X(&daE_GM_c::core_delwait, daE_GM_c, BabyGohmaDeath)            \
    X(&daE_HM_c::W_DeathAction, daE_HM_c, TorchSlugW_Death)         \
    X(&daE_HM_c::DeathMotion, daE_HM_c, TorchSlugDeathMotion)       \
    X(&daE_HM_c::DeathCutDown, daE_HM_c, TorchSlugDeathCutDown)     \
    X(&daE_HP_c::executeDead, daE_HP_c, HugePoeDeath)               \
    X(&daE_HZ_c::checkFall, daE_HZ_c, TileWormFall)                 \
    X(&daE_HZ_c::executeDeath, daE_HZ_c, TileWormDeath)             \
    X(&daE_HZ_c::executeWaterDeath, daE_HZ_c, TileWormWaterDeath)   \
    X(&daE_KK_c::executeDead, daE_KK_c, ChilfosDeath)               \
    X(&daE_OC_c::executeDeath, daE_OC_c, BokoblinDeath)             \
    X(&daE_OC_c::executeWaterDeath, daE_OC_c, BokoblinWaterDeath)   \
    X(&daE_PZ_c::executeDead, daE_PZ_c, PhantomZantDeath)           \
    X(&daE_SB_c::Death_Motion, daE_SB_c, ShellBladeDeath)           \
    X(&daE_SM_c::E_SM_C_Death, daE_SM_c, ChuWormDeath)              \
    X(&daE_SW_c::executeDie, daE_SW_c, MoldormDeath)                \
    X(&daE_SW_c::d_die, daE_SW_c, MoldormDDeath)                    \
    X(&daE_TT_c::executeDeath, daE_TT_c, TektiteDeath)              \
    X(&daE_VA_c::executeOpaciDeath, daE_VA_c, DeathSwordDeath)      \
    X(&daE_WS_c::executeDown, daE_WS_c, WallSkulltulaDownDeath)     \
    X(&daE_WS_c::executeWindDown, daE_WS_c, WallSkulltulaWindDeath) \
    X(&daE_WW_c::executeDamage, daE_WW_c, WhiteWolfosDeath)         \
    X(&daE_YM_c::executeDown, daE_YM_c, TwInsectDeath)              \
    X(&daE_YMB_c::setDeathEffect, daE_YMB_c, TwInsectBossDeath)     \
    X(&daE_ZM_c::executeDead, daE_ZM_c, ZantMaskDeath)              \
    X(&daE_ZS_c::executeDamage, daE_ZS_c, StaltroopDeath)           \

#define ENEMY_STATIC_DEATH_FUNCS(X)                                                                                 \
    X("src/d/actor/d_a_e_ba.cpp#ba_disappear", fopAc_ac_c, KeeseDeath, void(fopAc_ac_c*), i_this)                  \
    X("src/d/actor/d_a_e_bu.cpp#bu_disappear", fopAc_ac_c, BubbleDeath, void(fopAc_ac_c*), i_this)                 \
    X("src/d/actor/d_a_e_db.cpp#e_db_e_dead", e_db_class, DekuBabaDeath, void(e_db_class*), i_this->enemy)          \
    X("src/d/actor/d_a_e_dd.cpp#daE_DD_Execute", e_dd_class, DodongoDeath, int(e_dd_class*), i_this->actor)         \
    X("src/d/actor/d_a_e_dn.cpp#dn_disappear", e_dn_class, LizalfosDeath, void(e_dn_class*), i_this->actor)         \
    X("src/d/actor/d_a_e_fs.cpp#e_fs_end", e_fs_class, WoodenPuppetDeath, void(e_fs_class*), i_this->mEnemy)        \
    X("src/d/actor/d_a_e_gb.cpp#e_gb_end", e_gb_class, GiantBabaDeath, void(e_gb_class*), i_this->actor)            \
    X("src/d/actor/d_a_e_gb.cpp#e_gf_end", e_gb_class, GiantBabaFlowerDeath, void(e_gb_class*), i_this->actor)      \
    X("src/d/actor/d_a_e_kg.cpp#e_kg_damage", e_kg_class, YoungGohmaDeath, void(e_kg_class*), i_this)              \
    X("src/d/actor/d_a_e_kr.cpp#e_kr_damage", e_kr_class, KargarokDeath, s8(e_kr_class*), i_this->enemy)            \
    X("src/d/actor/d_a_e_mf.cpp#mf_disappear", e_mf_class, DynalfosDeath, void(e_mf_class*), i_this->actor)         \
    X("src/d/actor/d_a_e_mk.cpp#demo_camera_end", e_mk_class, OokCrownDeath, void(e_mk_class*), i_this->actor)      \
    X("src/d/actor/d_a_e_mm.cpp#e_mm_damage", e_mm_class, HelmasaurDeath, void(e_mm_class*), i_this->enemy)         \
    X("src/d/actor/d_a_e_ms.cpp#ms_disappear", e_ms_class, RatDeath, void(e_ms_class*), i_this)                    \
    X("src/d/actor/d_a_e_nz.cpp#e_nz_damage", e_nz_class, GhoulRatDeath, void(e_nz_class*), i_this->enemy)          \
    X("src/d/actor/d_a_e_po.cpp#e_po_dead", e_po_class, PoeDeath, void(e_po_class*), i_this->enemy)                 \
    X("src/d/actor/d_a_e_rb.cpp#e_rb_damage", e_rb_class, LeeverDeath, void(e_rb_class*), i_this->enemy)            \
    X("src/d/actor/d_a_e_rd.cpp#rd_disappear", e_rd_class, BulblinDeath, void(e_rd_class*), i_this->enemy)          \
    X("src/d/actor/d_a_e_rdy.cpp#rd_disappear", e_rdy_class, ShadowBulblinDeath, void(e_rdy_class*), i_this->actor) \
    X("src/d/actor/d_a_e_s1.cpp#e_s1_fail", e_s1_class, ShadowBeastDeath, void(e_s1_class*), i_this)               \
    X("src/d/actor/d_a_e_sf.cpp#crash_eff", e_sf_class, StalfosDeath, void(e_sf_class*), i_this->actor)             \
    X("src/d/actor/d_a_e_sg.cpp#e_sg_damage", e_sg_class, SkullfishDeath, void(e_sg_class*), i_this)               \
    X("src/d/actor/d_a_e_sh.cpp#e_sh_dead", e_sh_class, StalhoundDeath, void(e_sh_class*), i_this->enemy)           \
    X("src/d/actor/d_a_e_st.cpp#e_st_damage", e_st_class, SkulltulaDeath, void(e_st_class*), i_this->actor)         \
    X("src/d/actor/d_a_e_st.cpp#e_st_water", e_st_class, SkulltulaWaterDeath, void(e_st_class*), i_this->actor)     \
    X("src/d/actor/d_a_e_st.cpp#e_st_g_end", e_st_class, SkulltulaGDeath, s8(e_st_class*), i_this->actor)           \
    X("src/d/actor/d_a_e_th.cpp#demo_camera", e_th_class, DarkhammerDeath, s8(e_th_class*), i_this)                \
    X("src/d/actor/d_a_e_yd.cpp#yd_disappear", e_yd_class, TwDekuBabaDeath, void(e_yd_class*), i_this->actor)       \
    X("src/d/actor/d_a_e_yg.cpp#ms_disappear", e_yg_class, TwVerminDeath, void(e_yg_class*), i_this->actor)         \
    X("src/d/actor/d_a_e_yh.cpp#yh_disappear", e_yh_class, TwHebiBabaDeath, void(e_yh_class*), i_this->enemy)       \
    X("src/d/actor/d_a_e_yk.cpp#yk_disappear", e_yk_class, TwKeeseDeath, void(e_yk_class*), i_this)                \
    X("src/d/actor/d_a_e_yr.cpp#yr_disappear", e_yr_class, TwKargorokDeath, void(e_yr_class*), i_this->mEnemy)      \

#define ENEMY_DEATH_HOOK(symbol, class, alias)   \
    DEFINE_HOOK(symbol, alias);

#define ENEMY_STATIC_DEATH_HOOK(symbol, class, alias, params, base_member)  \
    DEFINE_HOOK_SYMBOL(symbol, params, alias);

#define ENEMY_DEATH_HOOK_FUNC(symbol, class, alias)                     \
    HookAction on##alias(ModContext*, void* args, void*, void*) {       \
        class* i_this = mods::arg<class*>(args, 0);                     \
        onDeath(i_this);                                                \
        return HOOK_CONTINUE;                                           \
    }

template <typename T>
auto toPointer(T&& value) {
    if constexpr (std::is_pointer_v<std::remove_reference_t<T>>) {
        return value;
    } else {
        return std::addressof(value);
    }
}

#define ENEMY_STATIC_DEATH_HOOK_FUNC(symbol, class, alias, params, base_member) \
    HookAction on##alias(ModContext*, void* args, void*, void*) {               \
        class* i_this = mods::arg<class*>(args, 0);                             \
        onDeath(toPointer(base_member));                                        \
        return HOOK_CONTINUE;                                                   \
    }