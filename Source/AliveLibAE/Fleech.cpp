#include "stdafx.h"
#include "Fleech.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "SnoozeParticle.hpp"
#include "Dove.hpp"
#include "Blood.hpp"
#include "Shadow.hpp"
#include "Particle.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"
#include "Gibs.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "PsxDisplay.hpp"
#include "Sfx.hpp"
#include "SlamDoor.hpp"
#include "Sound/Midi.hpp"
#include "Sys_common.hpp"
#include "Grid.hpp"

ALIVE_VAR(1, 0x5BC20C, u8, sFleechRandomIdx_5BC20C, 0);
ALIVE_VAR(1, 0x5BC20E, s16, sFleechCount_5BC20E, 0);

const TFleechMotionFn sFleech_motion_table_551798[19] = {
    &Fleech::M_Sleeping_0_42F0B0,
    &Fleech::M_WakingUp_1_42F270,
    &Fleech::M_Unknown_2_42F2F0,
    &Fleech::M_Idle_3_42E850,
    &Fleech::M_Crawl_4_42E960,
    &Fleech::M_PatrolCry_5_42E810,
    &Fleech::M_Knockback_6_42EAF0,
    &Fleech::M_StopCrawling_7_42EBB0,      //Stop due to being aligned vertically with target
    &Fleech::M_StopMidCrawlCycle_8_42EB20,
    &Fleech::M_Fall_9_42ECD0,
    &Fleech::M_Land_10_42F330,
    &Fleech::M_RaiseHead_11_42F590,
    &Fleech::M_Climb_12_42F7F0,
    &Fleech::M_SettleOnGround_13_42FB00,
    &Fleech::M_ExtendTongueFromEnemy_14_42FBD0,
    &Fleech::M_RetractTongueFromEnemey_15_42FC40,
    &Fleech::M_DeathByFalling_16_42FCE0,
    &Fleech::M_SleepingWithTongue_17_42F370,
    &Fleech::M_Consume_18_42FDF0};

enum eFleechMotions
{
    M_Sleeping_0_42F0B0,
    M_WakingUp_1_42F270,
    M_Unknown_2_42F2F0,
    M_Idle_3_42E850,
    M_Crawl_4_42E960,
    M_PatrolCry_5_42E810,
    M_Knockback_6_42EAF0,
    M_StopCrawling_7_42EBB0,
    M_StopMidCrawlCycle_8_42EB20,
    M_Fall_9_42ECD0,
    M_Land_10_42F330,
    M_RaiseHead_11_42F590,
    M_Climb_12_42F7F0,
    M_SettleOnGround_13_42FB00,
    M_ExtendTongueFromEnemy_14_42FBD0,
    M_RetractTongueFromEnemey_15_42FC40,
    M_DeathByFalling_16_42FCE0,
    M_SleepingWithTongue_17_42F370,
    M_Consume_18_42FDF0
};

const SfxDefinition stru_5518E0[20] = {
    {0u, 6u, 50u, 30u, -127, 127},
    {0u, 6u, 49u, 127u, 0, 0},
    {0u, 6u, 54u, 127u, 0, 0},
    {0u, 1u, 36u, 35u, 0, 0},
    {0u, 1u, 37u, 35u, 0, 0},
    {0u, 6u, 52u, 100u, 0, 0},
    {0u, 6u, 50u, 30u, -127, 0},
    {0u, 6u, 50u, 40u, 500, 600},
    {0u, 6u, 50u, 30u, 300, 400},
    {0u, 3u, 59u, 100u, 256, 384},
    {0u, 4u, 65u, 80u, 0, 0},
    {0u, 4u, 65u, 55u, 0, 0},
    {0u, 4u, 65u, 85u, 0, 0},
    {0u, 4u, 65u, 100u, 127, 127},
    {0u, 6u, 61u, 70u, -127, 127},
    {0u, 6u, 62u, 70u, -127, 127},
    {0u, 6u, 63u, 70u, -127, 127},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};


const TFleechBrainFn sFleechAiTable[4] = {
    &Fleech::Brain_0_Patrol_430BA0,
    &Fleech::Brain_1_ChasingAbe_428760,
    &Fleech::Brain_2_Scared_42D310,
    &Fleech::Brain_3_Death_42D1E0};

enum eFleechBrains
{
    eBrain_0_Patrol_430BA0 = 0,
    eBrain_1_ChasingAbe_428760 = 1,
    eBrain_2_Scared_42D310 = 2,
    eBrain_3_Death_42D1E0 = 3
};

static u8 Fleech_NextRandom()
{
    return sRandomBytes_546744[sFleechRandomIdx_5BC20C++];
}

Fleech::Fleech(Path_Fleech* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(2)
{
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    mBaseGameObjectTlvInfo = tlvInfo;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
    }

    Init_42A170();

    field_118_tlvInfo = tlvInfo;
    field_124_brain_state = eFleechBrains::eBrain_0_Patrol_430BA0;
    field_11C_obj_id = -1;
    field_170_danger_obj = -1;

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_direction == XDirection_short::eLeft_0);

    field_174_flags.Set(Flags_174::eBit5_asleep, pTlv->field_14_asleep == Choice_short::eYes_1);
    field_174_flags.Set(Flags_174::eBit6_goes_to_sleep, pTlv->field_24_goes_to_sleep == Choice_short::eYes_1);
    field_174_flags.Set(Flags_174::eBit7_persistant, pTlv->field_2C_persistant == Choice_short::eYes_1);

    field_140_max_anger = 2;
    field_158_chase_delay = 10;

    field_142_attack_anger_increaser = pTlv->field_1A_attack_anger_increaser + 2;
    field_144_wake_up_switch_id = pTlv->field_1E_wake_up_switch_id;
    field_146_wake_up_switch_anger_value = pTlv->field_28_wake_up_switch_anger_value;
    field_148_wake_up_switch_value = SwitchStates_Get(pTlv->field_1E_wake_up_switch_id) & 0xFFFF;
    field_14A_can_wake_up_switch_id = pTlv->field_2A_can_wake_up_switch_id;
    field_150_patrol_range = FP_GetExponent(FP_FromInteger(pTlv->field_26_patrol_range_in_grids) * ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale));
    field_15C_lost_target_timeout = pTlv->field_22_lost_target_timeout;

    InitTonguePolys_42B6E0();

    field_13E_current_anger = 0;

    if (pTlv->field_20_hanging == Choice_short::eYes_1)
    {
        field_160_hoistX = (pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x) / 2;
        field_166_angle = Fleech_NextRandom();
        mBaseAnimatedWithPhysicsGameObject_YPos -= FP_FromInteger(pTlv->field_8_top_left.field_2_y - pTlv->field_C_bottom_right.field_2_y);
        TongueHangingFromWall_42B9A0((pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x) / 2, pTlv->field_8_top_left.field_2_y);
        mCurrentMotion = eFleechMotions::M_SleepingWithTongue_17_42F370;
        SetAnim_429D80();
    }
    else if (field_174_flags.Get(Flags_174::eBit5_asleep))
    {
        mCurrentMotion = eFleechMotions::M_Sleeping_0_42F0B0;
        SetAnim_429D80();
    }
    else
    {
        mCurrentMotion = eFleechMotions::M_Idle_3_42E850;
        field_13E_current_anger = field_140_max_anger + (field_142_attack_anger_increaser - field_140_max_anger) / 2;
    }
}

const static AnimId sFleechFrameTableOffsets_5517E4[19] = {
    AnimId::Fleech_Sleeping,
    AnimId::Fleech_WakingUp,
    AnimId::Fleech_Unused,
    AnimId::Fleech_Idle,
    AnimId::Fleech_Crawl,
    AnimId::Fleech_PatrolCry,
    AnimId::Fleech_Knockback,
    AnimId::Fleech_StopCrawling,
    AnimId::Fleech_StopMidCrawlCycle,
    AnimId::Fleech_Fall,
    AnimId::Fleech_Land,
    AnimId::Fleech_RaiseHead,
    AnimId::Fleech_Climb,
    AnimId::Fleech_SettleOnGround,
    AnimId::Fleech_ExtendTongueFromEnemy,
    AnimId::Fleech_RetractTongueFromEnemey,
    AnimId::Fleech_DeathByFalling,
    AnimId::Fleech_SleepingWithTongue,
    AnimId::Fleech_Consume};

ALIVE_VAR(1, 0x551840, s32, current_target_object_id_551840, -1);

s32 Fleech::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Fleech_State*>(pBuffer);

    auto pTlv = static_cast<Path_Fleech*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_40_tlvInfo));
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kFleechResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("FLEECH.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kFleeBlowResID_580, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("FLEEBLOW.BAN", nullptr);
    }

    auto pFleech = relive_new Fleech(pTlv, pState->field_40_tlvInfo);
    if (pFleech)
    {
        pFleech->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

        pFleech->BaseAliveGameObjectPathTLV = nullptr;
        pFleech->BaseAliveGameObjectCollisionLine = nullptr;

        pFleech->BaseAliveGameObjectId = pState->field_3C_id;

        pFleech->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_8_xpos;
        pFleech->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_C_ypos;
        pFleech->mBaseAnimatedWithPhysicsGameObject_VelX = pState->field_10_velx;
        pFleech->mBaseAnimatedWithPhysicsGameObject_VelY = pState->field_14_vely;

        pFleech->mBaseAnimatedWithPhysicsGameObject_PathNumber = pState->field_18_path_number;
        pFleech->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAE(pState->field_1A_lvl_number);
        pFleech->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pState->field_1C_sprite_scale;

        pFleech->mBaseAnimatedWithPhysicsGameObject_Red = pState->mRingRed;
        pFleech->mBaseAnimatedWithPhysicsGameObject_Green = pState->mRingGreen;
        pFleech->mBaseAnimatedWithPhysicsGameObject_Blue = pState->mRingBlue;

        pFleech->mCurrentMotion = pState->field_28_current_motion;
        const AnimRecord& animRec = AnimRec(sFleechFrameTableOffsets_5517E4[pFleech->mCurrentMotion]);
        pFleech->mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        pFleech->mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame = pState->field_2A_anim_current_frame;
        pFleech->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = pState->field_2C_frame_change_counter;

        pFleech->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pState->field_26_bFlipX & 1);
        pFleech->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_2E_bRender & 1);

        pFleech->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F_bDrawable & 1);

        if (IsLastFrame(&pFleech->mBaseAnimatedWithPhysicsGameObject_Anim))
        {
            pFleech->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
        }
        pFleech->mHealth = pState->field_30_health;
        pFleech->mCurrentMotion = pState->field_34_current_motion;
        pFleech->mNextMotion = pState->field_36_next_motion;

        pFleech->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_38_lastLineYPos);

        pFleech->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

        pFleech->BaseAliveGameObjectCollisionLineType = pState->field_3A_line_type;
        pFleech->field_118_tlvInfo = pState->field_40_tlvInfo;
        pFleech->field_11C_obj_id = pState->field_44_obj_id;
        pFleech->field_120_unused = pState->field_48_unused;
        pFleech->field_178_tongue_state = pState->field_4A_save_tongue_state;
        pFleech->field_17A_tongue_sub_state = pState->field_4C_save_tongue_sub_state;
        pFleech->field_17C = pState->field_4E;
        pFleech->field_17E = pState->field_50;
        pFleech->field_180_tongue_x = pState->field_52_tongue_x;
        pFleech->field_182_tongue_y = pState->field_54_tongue_y;
        pFleech->field_184_target_x = pState->field_56_target_x;
        pFleech->field_186_target_y = pState->field_58_target_y;
        pFleech->field_188 = pState->field_5A;

        pFleech->field_18A.Set(Flags_18A::e18A_TongueActive_Bit1, pState->field_5C_tongue_active_flag & 1);
        pFleech->field_18A.Set(Flags_18A::e18A_Render_Bit2, pState->field_5D_render_flag & 1);

        pFleech->field_124_brain_state = pState->field_5E_brain_state;
        pFleech->field_126_state = pState->field_60_state;
        pFleech->field_12C = pState->field_64;
        pFleech->field_128 = pState->field_62;
        sFleechRandomIdx_5BC20C = pState->field_68_fleech_random_idx;
        pFleech->field_130 = pState->field_6A;
        pFleech->field_134_unused = pState->field_6C_unused;
        pFleech->field_138_velx_factor = pState->field_70_velx_factor;
        pFleech->field_13C_unused = pState->field_74_unused;
        pFleech->field_13E_current_anger = pState->field_76_current_anger;
        pFleech->field_140_max_anger = pState->field_78_max_anger;
        pFleech->field_142_attack_anger_increaser = pState->field_7A_attack_anger;
        pFleech->field_144_wake_up_switch_id = pState->field_7C_wakeup_id;
        pFleech->field_146_wake_up_switch_anger_value = pState->field_7E_wake_up_switch_anger_value;
        pFleech->field_148_wake_up_switch_value = pState->field_80_wake_up_switch_value;
        pFleech->field_14A_can_wake_up_switch_id = pState->field_82_can_wake_up_id;
        pFleech->field_14C = pState->field_84;
        pFleech->field_14E = pState->field_86;
        pFleech->field_150_patrol_range = pState->field_88_patrol_range;
        pFleech->field_152 = pState->field_8A;
        pFleech->field_154 = pState->field_8C;
        pFleech->field_156_rnd_crawl = pState->field_8E;
        pFleech->field_158_chase_delay = pState->field_90_chase_delay;
        pFleech->field_15A_chase_timer = pState->field_92_chase_timer;
        pFleech->field_15C_lost_target_timeout = pState->field_94_lost_target_timeout;
        pFleech->field_15E_lost_target_timer = pState->field_96_lost_target_timer;
        pFleech->field_160_hoistX = pState->field_98_hoistX;
        pFleech->field_162_hoistY = pState->field_9A_hoistY;
        pFleech->field_164 = pState->field_9C;
        pFleech->field_166_angle = pState->field_9E_angle;
        pFleech->field_168 = pState->field_A0;
        pFleech->field_16C = pState->field_A4;
        pFleech->field_170_danger_obj = pState->field_A8;

        if (pState->field_4_obj_id == pState->field_AC_obj_id)
        {
            current_target_object_id_551840 = pFleech->field_8_object_id;
        }

        pFleech->field_174_flags.Set(Flags_174::eBit1_bHoistDone, pState->field_B0.Get(Fleech_State::eBit1));
        pFleech->field_174_flags.Set(Flags_174::eBit2, pState->field_B0.Get(Fleech_State::eBit2));
        pFleech->field_174_flags.Set(Flags_174::eBit3, pState->field_B0.Get(Fleech_State::eBit3));
        pFleech->field_174_flags.Set(Flags_174::eBit4, pState->field_B0.Get(Fleech_State::eBit4));
        pFleech->field_174_flags.Set(Flags_174::eBit5_asleep, pState->field_B0.Get(Fleech_State::eBit5));
        pFleech->field_174_flags.Set(Flags_174::eBit6_goes_to_sleep, pState->field_B0.Get(Fleech_State::eBit6));
        pFleech->field_174_flags.Set(Flags_174::eBit7_persistant, pState->field_B0.Get(Fleech_State::eBit7));
    }

    return sizeof(Fleech_State);
}

s32 Fleech::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Fleech_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eFleech_50;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;
    pState->field_8_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_C_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pState->field_10_velx = mBaseAnimatedWithPhysicsGameObject_VelX;
    pState->field_14_vely = mBaseAnimatedWithPhysicsGameObject_VelY;
    pState->field_70_velx_factor = field_138_velx_factor;
    pState->field_18_path_number = mBaseAnimatedWithPhysicsGameObject_PathNumber;
    pState->field_1A_lvl_number = MapWrapper::ToAE(mBaseAnimatedWithPhysicsGameObject_LvlNumber);
    pState->field_1C_sprite_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    pState->mRingRed = mBaseAnimatedWithPhysicsGameObject_Red;
    pState->mRingGreen = mBaseAnimatedWithPhysicsGameObject_Green;
    pState->mRingBlue = mBaseAnimatedWithPhysicsGameObject_Blue;
    pState->field_26_bFlipX = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_28_current_motion = mCurrentMotion;
    pState->field_2A_anim_current_frame = mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame;
    pState->field_2C_frame_change_counter = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter;
    pState->field_2F_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2E_bRender = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render);
    pState->field_30_health = mHealth;
    pState->field_34_current_motion = mCurrentMotion;
    pState->field_36_next_motion = mNextMotion;
    pState->field_38_lastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_3A_line_type = BaseAliveGameObjectCollisionLine->field_8_type;
    }
    else
    {
        pState->field_3A_line_type = -1;
    }

    if (BaseAliveGameObjectId != -1)
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(BaseAliveGameObjectId);
        if (pObj)
        {
            pState->field_3C_id = pObj->mBaseGameObjectTlvInfo;
        }
    }
    else
    {
        pState->field_3C_id = -1;
    }

    if (field_11C_obj_id != -1)
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(field_11C_obj_id);
        if (pObj)
        {
            pState->field_44_obj_id = pObj->mBaseGameObjectTlvInfo;
        }
    }
    else
    {
        pState->field_44_obj_id = -1;
    }

    pState->field_40_tlvInfo = field_118_tlvInfo;
    pState->field_48_unused = field_120_unused;
    pState->field_4A_save_tongue_state = field_178_tongue_state;
    pState->field_4C_save_tongue_sub_state = field_17A_tongue_sub_state;
    pState->field_4E = field_17C;
    pState->field_50 = field_17E;
    pState->field_52_tongue_x = field_180_tongue_x;
    pState->field_54_tongue_y = field_182_tongue_y;
    pState->field_56_target_x = field_184_target_x;
    pState->field_58_target_y = field_186_target_y;
    pState->field_5A = field_188;
    pState->field_5C_tongue_active_flag = field_18A.Get(Flags_18A::e18A_TongueActive_Bit1);
    pState->field_5D_render_flag = field_18A.Get(Flags_18A::e18A_Render_Bit2);
    pState->field_5E_brain_state = field_124_brain_state;
    pState->field_60_state = field_126_state;
    pState->field_64 = field_12C - sGnFrame;
    pState->field_62 = field_128;
    pState->field_68_fleech_random_idx = sFleechRandomIdx_5BC20C;
    pState->field_6A = field_130;
    pState->field_6C_unused = field_134_unused;
    pState->field_70_velx_factor = field_138_velx_factor;
    pState->field_74_unused = field_13C_unused;
    pState->field_76_current_anger = field_13E_current_anger;
    pState->field_78_max_anger = field_140_max_anger;
    pState->field_7A_attack_anger = field_142_attack_anger_increaser;
    pState->field_7C_wakeup_id = field_144_wake_up_switch_id;
    pState->field_7E_wake_up_switch_anger_value = field_146_wake_up_switch_anger_value;
    pState->field_80_wake_up_switch_value = field_148_wake_up_switch_value;
    pState->field_82_can_wake_up_id = field_14A_can_wake_up_switch_id;
    pState->field_84 = field_14C;
    pState->field_86 = field_14E;
    pState->field_88_patrol_range = field_150_patrol_range;
    pState->field_8A = field_152;
    pState->field_8C = field_154;
    pState->field_8E = field_156_rnd_crawl;
    pState->field_90_chase_delay = field_158_chase_delay;
    pState->field_92_chase_timer = field_15A_chase_timer;
    pState->field_94_lost_target_timeout = field_15C_lost_target_timeout;
    pState->field_96_lost_target_timer = field_15E_lost_target_timer;
    pState->field_98_hoistX = field_160_hoistX;
    pState->field_9A_hoistY = field_162_hoistY;
    pState->field_9C = field_164;
    pState->field_9E_angle = field_166_angle;
    pState->field_A0 = field_168;
    pState->field_A4 = field_16C;

    if (field_170_danger_obj != -1)
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(field_170_danger_obj);
        if (pObj)
        {
            pState->field_A8 = pObj->mBaseGameObjectTlvInfo;
        }
    }
    else
    {
        pState->field_A8 = -1;
    }

    if (current_target_object_id_551840 == field_8_object_id)
    {
        pState->field_AC_obj_id = mBaseGameObjectTlvInfo;
    }
    else
    {
        pState->field_AC_obj_id = -1;
    }

    pState->field_B0.Set(Fleech_State::eBit1, field_174_flags.Get(Flags_174::eBit1_bHoistDone));
    pState->field_B0.Set(Fleech_State::eBit2, field_174_flags.Get(Flags_174::eBit2));
    pState->field_B0.Set(Fleech_State::eBit3, field_174_flags.Get(Flags_174::eBit3));
    pState->field_B0.Set(Fleech_State::eBit4, field_174_flags.Get(Flags_174::eBit4));
    pState->field_B0.Set(Fleech_State::eBit5, field_174_flags.Get(Flags_174::eBit5_asleep));
    pState->field_B0.Set(Fleech_State::eBit6, field_174_flags.Get(Flags_174::eBit6_goes_to_sleep));
    pState->field_B0.Set(Fleech_State::eBit7, field_174_flags.Get(Flags_174::eBit7_persistant));

    return sizeof(Fleech_State);
}

void Fleech::M_Sleeping_0_42F0B0()
{
    if (mNextMotion == -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame || sGnFrame & 3)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 4 && !(sGnFrame & 3))
            {
                Sound_430520(FleechSound::SleepingExhale_4);

                if (gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
                {
                    const FP yPos = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-20)) + mBaseAnimatedWithPhysicsGameObject_YPos;
                    FP xOff = {};
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        xOff = -(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10));
                    }
                    else
                    {
                        xOff = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10));
                    }
                    relive_new SnoozeParticle(xOff + mBaseAnimatedWithPhysicsGameObject_XPos, yPos, mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer, mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale);
                }
            }
        }
        else
        {
            Sound_430520(FleechSound::SleepingInhale_3);
        }
    }
    else
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
    }
}

void Fleech::M_WakingUp_1_42F270()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSnoozeParticle)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1E4_state = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (mNextMotion != -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
            Sound_430520(FleechSound::WakeUp_6);
        }
    }
}

// this function is actually used
void Fleech::M_Unknown_2_42F2F0()
{
    if (mNextMotion != -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
        }
    }
}

void Fleech::M_Idle_3_42E850()
{
    if (!CanMove_42E3E0())
    {
        if (mNextMotion == -1)
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850 && mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 0 && !sCollisions->Raycast(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(5), mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5), mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(5), mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(1), &pLine, &hitX, &hitY, mBaseAnimatedWithPhysicsGameObject_Scale != 0 ? 1 : 16))
            {
                field_138_velx_factor = FP_FromInteger(0);
                BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
                mCurrentMotion = eFleechMotions::M_Fall_9_42ECD0;
            }
        }
        else
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
        }
    }
}

const FP sFleechCrawlVelX_544FA0[7] = {
    FP_FromDouble(3.8364105224609375),
    FP_FromDouble(3.36883544921875),
    FP_FromDouble(4.6882781982421875),
    FP_FromDouble(4.9884185791015625),
    FP_FromDouble(2.5774078369140625),
    FP_FromDouble(3.3113861083984375),
    FP_FromDouble(2.2292022705078125)};

void Fleech::M_Crawl_4_42E960()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame > 6)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame = 0;
    }

    FP velXTable = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        velXTable = -sFleechCrawlVelX_544FA0[mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame];
    }
    else
    {
        velXTable = sFleechCrawlVelX_544FA0[mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame];
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * velXTable);

    const FP k1Directed = FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != 0) ? -1 : 1);
    if (WallHit(FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1)) ? 10 : 5), ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * k1Directed))
    {
        ToIdle_42E520();
    }
    else
    {
        MoveAlongFloor_42E600();
        if (mCurrentMotion == eFleechMotions::M_Crawl_4_42E960)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 4)
            {
                Sound_430520(FleechSound::CrawlRNG1_14);
            }
            else if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 6)
            {
                if (field_130 == 0)
                {
                    field_130 = 1;
                    MapFollowMe(TRUE);
                }

                if (mNextMotion == eFleechMotions::M_Idle_3_42E850)
                {
                    mCurrentMotion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
                    mNextMotion = -1;
                }
                else if (mNextMotion != -1)
                {
                    mCurrentMotion = mNextMotion;
                    mNextMotion = -1;
                }
            }
            else
            {
                field_130 = 0;
            }
        }
    }
}

void Fleech::M_PatrolCry_5_42E810()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 0)
    {
        Sound_430520(FleechSound::PatrolCry_0);
        field_13C_unused = 1;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

void Fleech::M_Knockback_6_42EAF0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        ToIdle_42E520();
    }
}

void Fleech::M_StopCrawling_7_42EBB0()
{
    const FP k1Directed = FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != 0 ? -1 : 1);
    if (WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * k1Directed))
    {
        ToIdle_42E520();
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe(TRUE);
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(7));
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(7));
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
        }
        mCurrentMotion = eFleechMotions::M_Crawl_4_42E960;
    }
}

void Fleech::M_StopMidCrawlCycle_8_42EB20()
{
    const FP k1Directed = FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? -1 : 1); // TODO: Correct way around ??
    if (WallHit(
            FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5),
            ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * k1Directed)
        || mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

void Fleech::M_Fall_9_42ECD0()
{
    mBaseAnimatedWithPhysicsGameObject_VelY += mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(1.8);

    if (mBaseAnimatedWithPhysicsGameObject_VelY > (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20)))
    {
        mBaseAnimatedWithPhysicsGameObject_VelY = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20));
    }

    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromDouble(3.6))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromDouble(3.6);
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromDouble(-3.6))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromDouble(-3.6);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX -= mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_138_velx_factor;
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX += mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_138_velx_factor;
        if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }

    const FP xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP ypos = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5));
    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions->Raycast(
            xpos,
            ypos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == 1 ? 0x0F : 0xF0))
    {
        switch (pLine->field_8_type)
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                MapFollowMe(TRUE);
                if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_32 || BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_36)
                {
                    const PSX_RECT bRect = VGetBoundingRect();
                    VOnCollisionWith(
                        {bRect.x, static_cast<s16>(bRect.y + 5)},
                        {bRect.w, static_cast<s16>(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) + 5)},
                        ObjList_5C1B78,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                }
                if (mBaseAnimatedWithPhysicsGameObject_YPos - BaseAliveGameObjectLastLineYPos <= FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 20 : 10)) * FP_FromInteger(15))
                {
                    mCurrentMotion = eFleechMotions::M_Land_10_42F330;
                }
                else
                {
                    mCurrentMotion = eFleechMotions::M_DeathByFalling_16_42FCE0;
                }
                break;

            case 1u:
            case 2u:
            case 5u:
            case 6u:
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX - mBaseAnimatedWithPhysicsGameObject_VelX;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                MapFollowMe(TRUE);
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                break;

            default:
                return;
        }
    }
}

void Fleech::M_Land_10_42F330()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 0)
    {
        Sound_430520(FleechSound::LandOnFloor_9);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

void Fleech::M_RaiseHead_11_42F590()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_174_flags.Clear(Flags_174::eBit1_bHoistDone);
        mCurrentMotion = eFleechMotions::M_Climb_12_42F7F0;

        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(-1);

        const s16 yOff = mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 0 : -10;
        auto pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            field_160_hoistX,
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger((yOff + 20))),
            field_160_hoistX,
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger((yOff + 20))),
            TlvTypes::Hoist_2));

        if (pHoist->field_10_type == Path_Hoist::Type::eOffScreen)
        {
            const FP doubleYOff = FP_FromInteger(yOff + 20) * FP_FromInteger(2);
            pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                field_160_hoistX,
                FP_GetExponent(FP_FromInteger(pHoist->field_8_top_left.field_2_y) - doubleYOff),
                field_160_hoistX,
                FP_GetExponent(FP_FromInteger(pHoist->field_8_top_left.field_2_y) - doubleYOff),
                TlvTypes::Hoist_2));

            field_162_hoistY = pHoist->field_8_top_left.field_2_y;
        }
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        field_168 = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(field_162_hoistY);
        BaseAliveGameObjectCollisionLine = nullptr;
        field_16C = FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(field_160_hoistX));

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            field_166_angle = -64;
        }
        else
        {
            field_166_angle = 64;
        }
        field_164 = 0;
        sub_42BA10();
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame < 4)
    {
        mBaseAnimatedWithPhysicsGameObject_XPos += FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? 1 : -1);
    }
}

void Fleech::M_Climb_12_42F7F0()
{
    if (!TongueActive_42B8A0())
    {
        if (field_174_flags.Get(Flags_174::eBit1_bHoistDone))
        {
            mCurrentMotion = eFleechMotions::M_SettleOnGround_13_42FB00;
            return;
        }

        const FP velYOff = FP_FromInteger(field_164) * FP_FromDouble(0.045);
        mBaseAnimatedWithPhysicsGameObject_VelY -= mBaseAnimatedWithPhysicsGameObject_SpriteScale * (FP_FromDouble(0.7) - velYOff);
        if (field_164 > 0)
        {
            field_164--;
        }

        const FP xOff = (Math_Cosine_496CD0(field_166_angle) * (field_16C * ((mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(field_162_hoistY)) / field_168)));

        FP pX1 = {};
        if (xOff < FP_FromInteger(0))
        {
            pX1 = xOff + mBaseAnimatedWithPhysicsGameObject_XPos;
        }
        else
        {
            pX1 = mBaseAnimatedWithPhysicsGameObject_XPos;
        }

        const FP pY1 = mBaseAnimatedWithPhysicsGameObject_VelY + mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(1) ? 10 : 20);

        FP pX2;
        if (xOff < FP_FromInteger(0))
        {
            pX2 = mBaseAnimatedWithPhysicsGameObject_XPos;
        }
        else
        {
            pX2 = xOff + mBaseAnimatedWithPhysicsGameObject_XPos;
        }

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions->Raycast(pX1, pY1, pX2, mBaseAnimatedWithPhysicsGameObject_VelY + mBaseAnimatedWithPhysicsGameObject_YPos, &pLine, &hitX, &hitY, mBaseAnimatedWithPhysicsGameObject_Scale != 0 ? 6 : 0x60))
        {
            switch (pLine->field_8_type)
            {
                case 1u:
                case 5u:
                    Sound_430520(FleechSound::LandOnFloor_9);
                    // TODO: Somewhat suspect that these branches are equal - OG bug?
                    if (field_166_angle >= 64u && field_166_angle > 192u)
                    {
                        field_166_angle = -128 - field_166_angle;
                    }
                    else
                    {
                        field_166_angle = -128 - field_166_angle;
                    }
                    break;

                case 2u:
                case 6u:
                    Sound_430520(FleechSound::LandOnFloor_9);
                    if (field_166_angle > 64u && field_166_angle < 128u)
                    {
                        field_166_angle = -128 - field_166_angle;
                    }
                    else if (field_166_angle > 128 && field_166_angle < 192)
                    {
                        field_166_angle = -128 - field_166_angle;
                    }
                    break;

                default:
                    break;
            }
        }

        mBaseAnimatedWithPhysicsGameObject_XPos = xOff + FP_FromInteger(field_160_hoistX);
        field_166_angle = field_166_angle + 16;
        mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

        if (mBaseAnimatedWithPhysicsGameObject_YPos <= FP_FromInteger(field_162_hoistY))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(field_160_hoistX);
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(field_162_hoistY);
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            field_174_flags.Set(Flags_174::eBit1_bHoistDone);
        }
        return;
    }
}

void Fleech::M_SettleOnGround_13_42FB00()
{
    if (!mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame)
    {
        SetTongueState5_42BAD0();

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(5),
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5),
                mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(5),
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5),
                &pLine,
                &hitX,
                &hitY,
                mBaseAnimatedWithPhysicsGameObject_Scale != 0 ? 1 : 16))
        {
            BaseAliveGameObjectCollisionLine = pLine;
            MapFollowMe(TRUE);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Fleech::ToIdle_42E520();
    }
}

void Fleech::M_ExtendTongueFromEnemy_14_42FBD0()
{
    if (field_11C_obj_id == sActiveHero->field_8_object_id && (sActiveHero->CantBeDamaged_44BAB0() || sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible)))
    {
        ToIdle_42E520();
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        PullTargetIn_42BAF0();
        mCurrentMotion = eFleechMotions::M_RetractTongueFromEnemey_15_42FC40;
    }
}

void Fleech::M_RetractTongueFromEnemey_15_42FC40()
{
    if (sObjectIds.Find_Impl(field_11C_obj_id) == sActiveHero && ((sActiveHero->CantBeDamaged_44BAB0()) || sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible)))
    {
        sub_42B8C0();
        ToIdle_42E520();
    }
    else
    {
        if (!TongueActive_42B8A0())
        {
            ToIdle_42E520();
        }

        if (mNextMotion == eFleechMotions::M_Consume_18_42FDF0)
        {
            mCurrentMotion = eFleechMotions::M_Consume_18_42FDF0;
            mNextMotion = -1;
        }
    }
}

void Fleech::M_DeathByFalling_16_42FCE0()
{
    if (mHealth > FP_FromInteger(0))
    {
        relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(8), FP_FromInteger(0), -FP_FromInteger(5), mBaseAnimatedWithPhysicsGameObject_SpriteScale, 50);

        Sound_430520(FleechSound::DeathByHeight_12);
        Sound_430520(FleechSound::Scared_7);

        mHealth = FP_FromInteger(0);
        field_124_brain_state = eFleechBrains::eBrain_3_Death_42D1E0;
        field_174_flags.Set(Flags_174::eBit3);
        mNextMotion = -1;
        field_12C = sGnFrame + 127;
        sFleechCount_5BC20E--;
    }
}

void Fleech::M_SleepingWithTongue_17_42F370()
{
    if (mNextMotion != -1)
    {
        SetTongueState1_42B9F0();
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame || sGnFrame & 3)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 4 && !(sGnFrame & 3))
            {
                Sound_430520(FleechSound::SleepingExhale_4);
                if (gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
                {
                    const FP yPos = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-20)) + mBaseAnimatedWithPhysicsGameObject_YPos;
                    FP xOff = {};
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        xOff = -(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10));
                    }
                    else
                    {
                        xOff = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10));
                    }
                    relive_new SnoozeParticle(xOff + mBaseAnimatedWithPhysicsGameObject_XPos, yPos, mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer, mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale);
                }
            }
        }
        else
        {
            Sound_430520(FleechSound::SleepingInhale_3);
        }

        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(field_160_hoistX) + (Math_Cosine_496CD0(field_166_angle) * FP_FromInteger(4)) - FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_XOffset);
        field_166_angle += 2;
    }
}

void Fleech::M_Consume_18_42FDF0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 2)
    {
        Sound_430520(FleechSound::Digesting_2);
    }
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 15 && field_11C_obj_id == sActiveHero->field_8_object_id)
    {
        sActiveHero->SetAsDead_459430();

        Sound_430520(FleechSound::Burp_1);

        for (s32 i = 0; i < 3; i++)
        {
            const AnimRecord& doveRec = AnimRec(AnimId::Dove_Flying);
            auto pDove = relive_new Dove(
                doveRec.mFrameTableOffset,
                doveRec.mMaxW,
                doveRec.mMaxH,
                doveRec.mResourceId,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(10),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale);

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                pDove->mBaseAnimatedWithPhysicsGameObject_XPos += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(15));
            }
            else
            {
                pDove->mBaseAnimatedWithPhysicsGameObject_XPos -= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(15));
            }

            pDove->mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

Fleech::~Fleech()
{
    field_170_danger_obj = -1;
    if (field_118_tlvInfo != 0xFFFF)
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, mHealth <= FP_FromInteger(0));
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    ResetTarget_42CF70();

    if (mCurrentMotion == 18)
    {
        if (sActiveHero)
        {
            if (field_11C_obj_id == sActiveHero->field_8_object_id)
            {
                sActiveHero->SetAsDead_459430();
            }
        }
    }
    field_11C_obj_id = -1;

    if (!field_174_flags.Get(Flags_174::eBit3))
    {
        sFleechCount_5BC20E--;
    }
}

void Fleech::VUpdate()
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mBaseAnimatedWithPhysicsGameObject_XPos,
                &mBaseAnimatedWithPhysicsGameObject_YPos,
                1 << BaseAliveGameObjectCollisionLineType);
        }
        BaseAliveGameObjectCollisionLineType = 0;
        field_11C_obj_id = BaseGameObject::RefreshId(field_11C_obj_id);
        field_170_danger_obj = BaseGameObject::RefreshId(field_170_danger_obj);
        BaseAliveGameObjectId = BaseGameObject::RefreshId(BaseAliveGameObjectId);
    }

    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if ((FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos) <= FP_FromInteger(750) && FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos) <= FP_FromInteger(520)) || field_174_flags.Get(Flags_174::eBit7_persistant))
    {
        const auto oldMotion = mCurrentMotion;

        field_126_state = (this->*sFleechAiTable[field_124_brain_state])();

        TongueUpdate_42BD30();

        if (mBaseAnimatedWithPhysicsGameObject_YPos < FP_FromInteger(0))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }

        const FP oldY = mBaseAnimatedWithPhysicsGameObject_YPos;
        const FP oldX = mBaseAnimatedWithPhysicsGameObject_XPos;

        (this->*sFleech_motion_table_551798[mCurrentMotion])();

        if (oldX != mBaseAnimatedWithPhysicsGameObject_XPos || oldY != mBaseAnimatedWithPhysicsGameObject_YPos)
        {
            BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos);
            VOn_TLV_Collision(BaseAliveGameObjectPathTLV);
        }

        if (oldMotion == mCurrentMotion)
        {
            if (field_128)
            {
                mCurrentMotion = mPreviousMotion;
                SetAnim_429D80();
                mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
                field_128 = 0;
            }
        }
        else
        {
            SetAnim_429D80();
        }
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Fleech::VRender(PrimHeader** ot)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
        RenderEx(ot);
    }
}

void Fleech::RenderEx(PrimHeader** ot)
{
    if (field_18A.Get(Fleech::Flags_18A::e18A_Render_Bit2))
    {
        FP tongueBlock_X[5] = {};
        FP tongueBlock_Y[5] = {};

        const FP_Point* camPos = pScreenManager->mCamPos;
        const s16 camX = FP_GetExponent(camPos->field_0_x);
        const s16 camY = FP_GetExponent(camPos->field_4_y);

        tongueBlock_X[0] = FP_FromInteger(field_180_tongue_x - camX);
        tongueBlock_Y[0] = FP_FromInteger(field_182_tongue_y - camY);
        tongueBlock_X[4] = FP_FromInteger(field_184_target_x - camX);
        tongueBlock_Y[4] = FP_FromInteger(field_186_target_y - camY);

        const FP distanceX_squared = (tongueBlock_X[0] - tongueBlock_X[4]) * (tongueBlock_X[0] - tongueBlock_X[4]);
        const FP distanceY_squared = (tongueBlock_Y[0] - tongueBlock_Y[4]) * (tongueBlock_Y[0] - tongueBlock_Y[4]);
        const FP distanceXY_squareRoot = Math_SquareRoot_FP_496E90(distanceY_squared + distanceX_squared);
        const FP Tan_fp = Math_Tan_496F70(
            tongueBlock_Y[0] - tongueBlock_Y[4],
            tongueBlock_X[4] - tongueBlock_X[0]);
        const FP distanceCosine = Math_Cosine_496CD0(static_cast<u8>(FP_GetExponent(Tan_fp)));
        const FP SineTan = Math_Sine_496DD0(static_cast<u8>(FP_GetExponent(Tan_fp)));

        for (s32 i = 0; i < 4; i++)
        {
            const FP distanceXY_squareRoot_multiplied = distanceXY_squareRoot * FP_FromInteger(i + 1) * FP_FromDouble(0.25);
            const FP cosineIt_times_field188 = Math_Cosine_496CD0(static_cast<u8>(32 * (i + 1))) * FP_FromInteger(field_188);
            tongueBlock_X[i + 1] = tongueBlock_X[0] + SineTan * distanceXY_squareRoot_multiplied - cosineIt_times_field188 * distanceCosine;
            tongueBlock_Y[i + 1] = tongueBlock_Y[0] + SineTan * cosineIt_times_field188 + distanceCosine * distanceXY_squareRoot_multiplied;
        }

        const FP lastTongueBlockModX = tongueBlock_X[4] - FP_FromInteger(field_184_target_x + 0xFFFF * camX);
        const FP lastTongueBlockModY = tongueBlock_Y[4] - FP_FromInteger(field_186_target_y + 0xFFFF * camY);
        for (s32 i = 0; i < 4; i++)
        {
            const FP lastTongueBlockModX_mult = lastTongueBlockModX * FP_FromInteger(i + 1);
            const FP lastTongueBlockModY_mult = lastTongueBlockModY * FP_FromInteger(i + 1);
            tongueBlock_X[i + 1] -= lastTongueBlockModX_mult * FP_FromDouble(0.25);
            tongueBlock_Y[i + 1] -= lastTongueBlockModY_mult * FP_FromDouble(0.25);
        }

        for (s32 i = 0; i < 4; i++)
        {
            s16 r = static_cast<s16>((i + 1) * 150 / 4);
            s16 g = static_cast<s16>((i + 1) * 100 / 4);
            s16 b = static_cast<s16>((i + 1) * 100 / 4);
            s16 r2 = static_cast<s16>(i * 150 / 4);
            s16 g2 = static_cast<s16>(i * 100 / 4);
            s16 b2 = static_cast<s16>(i * 100 / 4);

            const FP currTongueBlock_Y = tongueBlock_Y[i];
            const FP currTongueBlock_X = tongueBlock_X[i];

            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(currTongueBlock_X + FP_FromInteger(camX)),
                FP_GetExponent(currTongueBlock_Y + FP_FromInteger(camY)),
                mBaseAnimatedWithPhysicsGameObject_Scale,
                &r,
                &g,
                &b);
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(currTongueBlock_X + FP_FromInteger(camX)),
                FP_GetExponent(currTongueBlock_Y + FP_FromInteger(camY)),
                mBaseAnimatedWithPhysicsGameObject_Scale,
                &r2,
                &g2,
                &b2);

            Poly_G4* currTonguePoly1 = &field_18C_tongue_polys1[i][gPsxDisplay_5C1130.field_C_buffer_index];

            const s32 tonguePolyX1 = PsxToPCX(FP_GetExponent(currTongueBlock_X));
            const s32 tonguePolyY1 = FP_GetExponent(currTongueBlock_Y);
            const s32 tonguePolyX2 = PsxToPCX(FP_GetExponent(tongueBlock_X[i + 1]));
            const s32 tonguePolyY2 = FP_GetExponent(tongueBlock_Y[i + 1]);

            SetXY0(
                currTonguePoly1,
                static_cast<s16>(tonguePolyX1),
                static_cast<s16>(tonguePolyY1 - 1));
            SetXY1(
                currTonguePoly1,
                static_cast<s16>(tonguePolyX2),
                static_cast<s16>(tonguePolyY2 - 1));
            SetXY2(
                currTonguePoly1,
                static_cast<s16>(tonguePolyX1),
                static_cast<s16>(tonguePolyY1 + 1));
            SetXY3(
                currTonguePoly1,
                static_cast<s16>(tonguePolyX2),
                static_cast<s16>(tonguePolyY2 + 1));

            SetRGB0(
                currTonguePoly1,
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            SetRGB1(
                currTonguePoly1,
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));
            SetRGB2(
                currTonguePoly1,
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            SetRGB3(
                currTonguePoly1,
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));

            OrderingTable_Add_4F8AA0(OtLayer(ot, mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer), &currTonguePoly1->mBase.header);

            Poly_G4* currTonguePoly2 = &field_2CC_tongue_polys2[i][gPsxDisplay_5C1130.field_C_buffer_index];

            s32 minus_one_one_switch = -1;
            if (FP_GetExponent(Tan_fp) <= 64 || FP_GetExponent(Tan_fp) >= 192)
            {
                minus_one_one_switch = 1;
            }

            SetXY0(
                currTonguePoly2,
                static_cast<s16>(tonguePolyX1 - minus_one_one_switch),
                static_cast<s16>(tonguePolyY1 - 1));
            SetXY1(
                currTonguePoly2,
                static_cast<s16>(tonguePolyX2 - minus_one_one_switch),
                static_cast<s16>(tonguePolyY2 - 1));
            SetXY2(
                currTonguePoly2,
                static_cast<s16>(tonguePolyX1 + minus_one_one_switch),
                static_cast<s16>(tonguePolyY1 + 1));
            SetXY3(
                currTonguePoly2,
                static_cast<s16>(tonguePolyX2 + minus_one_one_switch),
                static_cast<s16>(tonguePolyY2 + 1));

            SetRGB0(
                currTonguePoly2,
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            SetRGB1(
                currTonguePoly2,
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));
            SetRGB2(
                currTonguePoly2,
                static_cast<u8>(r2),
                static_cast<u8>(g2),
                static_cast<u8>(b2));
            SetRGB3(
                currTonguePoly2,
                static_cast<u8>(r),
                static_cast<u8>(g),
                static_cast<u8>(b));

            OrderingTable_Add_4F8AA0(OtLayer(ot, mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer), &currTonguePoly2->mBase.header);

            s16 invRect_x;
            s16 invRect_y;
            s16 invRect_w;
            s16 invRect_h;

            const s16 smallerof1andBaseX = std::min(
                currTonguePoly2->mVerts[1].mVert.x,
                currTonguePoly2->mBase.vert.x);
            const s16 biggerof2and0X = std::max(
                currTonguePoly2->mVerts[0].mVert.x,
                currTonguePoly2->mVerts[2].mVert.x);

            const s16 smallerof0andBaseY = std::min(
                currTonguePoly2->mVerts[0].mVert.y,
                currTonguePoly2->mBase.vert.y);
            const s16 biggerof1and2Y = std::max(
                currTonguePoly2->mVerts[1].mVert.y,
                currTonguePoly2->mVerts[2].mVert.y);

            if (smallerof1andBaseX < biggerof2and0X)
            {
                invRect_x = smallerof1andBaseX;
                invRect_w = biggerof2and0X;
            }
            else
            {
                invRect_x = biggerof2and0X;
                invRect_w = smallerof1andBaseX;
            }

            if (smallerof0andBaseY < biggerof1and2Y)
            {
                invRect_y = smallerof0andBaseY;
                invRect_h = biggerof1and2Y;
            }
            else
            {
                invRect_y = biggerof1and2Y;
                invRect_h = smallerof0andBaseY;
            }

            pScreenManager->InvalidateRect(
                invRect_x,
                invRect_y,
                invRect_w,
                invRect_h,
                pScreenManager->mIdx);
        }
        const s32 tPage = PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_0, 0, 0);
        Init_SetTPage(&field_40C[gPsxDisplay_5C1130.field_C_buffer_index], 1, 0, tPage);
        OrderingTable_Add_4F8AA0(OtLayer(ot, mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer), &field_40C[gPsxDisplay_5C1130.field_C_buffer_index].mBase);
        return;
    }
}

void Fleech::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_11C_obj_id = -1;
        field_170_danger_obj = -1;
    }
}

void Fleech::VOn_TLV_Collision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            mHealth = FP_FromInteger(0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
    }
}

s16 Fleech::IsScrabOrParamiteNear(FP radius)
{
    BaseAliveGameObject* pNearestScrabOrParamite = nullptr;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pBaseObj = gBaseGameObjects->ItemAt(i);
        if (!pBaseObj)
        {
            break;
        }

        if (pBaseObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            auto pObj = static_cast<BaseAliveGameObject*>(pBaseObj);
            if ((pObj->Type() == ReliveTypes::eScrab || pObj->Type() == ReliveTypes::eParamite) && pObj->mHealth > FP_FromInteger(0))
            {
                if (pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                {
                    bool check = false;
                    if (mBaseAnimatedWithPhysicsGameObject_YPos >= pObj->mBaseAnimatedWithPhysicsGameObject_YPos)
                    {
                        if (mBaseAnimatedWithPhysicsGameObject_YPos - pObj->mBaseAnimatedWithPhysicsGameObject_YPos > (FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 20 : 10) * FP_FromInteger(2)))
                        {
                            if (mBaseAnimatedWithPhysicsGameObject_YPos < pObj->mBaseAnimatedWithPhysicsGameObject_YPos)
                            {
                                if (pObj->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos <= (FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 2 : 1)))
                                {
                                    check = true;
                                }
                            }
                        }
                        else
                        {
                            check = true;
                        }
                    }
                    else
                    {
                        if (pObj->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos <= (FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? +2 : 1)))
                        {
                            check = true;
                        }
                    }

                    if (check)
                    {
                        if (VIsObjNearby(radius, pObj))
                        {
                            if (!WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5), pObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos))
                            {
                                if (!pNearestScrabOrParamite)
                                {
                                    pNearestScrabOrParamite = pObj;
                                }
                                else if (FP_GetExponent(FP_Abs(pObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos)) < FP_GetExponent(FP_Abs(pNearestScrabOrParamite->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos)))
                                {
                                    pNearestScrabOrParamite = pObj;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (pNearestScrabOrParamite)
    {
        field_170_danger_obj = pNearestScrabOrParamite->field_8_object_id;
        return 1;
    }

    field_170_danger_obj = -1;
    return 0;
}

const TintEntry kFleechTints_551844[15] = {
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

s32 Animation_OnFrame_Fleech_449A60(BaseGameObject* pObj, s16* pData)
{
    reinterpret_cast<Fleech*>(pObj)->vOnFrame_42BC50(pData);
    return 1;
}

void Fleech::Init_42A170()
{
    const AnimRecord& rec = AnimRec(AnimId::Fleech_Idle);
    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, rec.mResourceId, TRUE, FALSE));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kFleeBlowResID_580, TRUE, FALSE));

    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, field_10_resources_array.ItemAt(0), 1, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.field_1C_fn_ptr_array = kFleech_Anim_Frame_Fns_55EFD0;

    SetType(ReliveTypes::eFleech);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_174_flags.Clear(Flags_174::eBit3);
    field_174_flags.Clear(Flags_174::eBit4);

    mApplyShadows |= 2u;

    field_12C = 0;
    field_126_state = 0;
    mNextMotion = -1;
    BaseAliveGameObjectId = -1;
    field_128 = 0;
    field_11C_obj_id = -1;
    field_170_danger_obj = -1;
    field_15E_lost_target_timer = 0;

    SetTint(&kFleechTints_551844[0], gMap.mCurrentLevel);

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SlogFleech_34;
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SlogFleech_Half_15;
        mBaseAnimatedWithPhysicsGameObject_Scale = 0;
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale != 0 ? 1 : 16)
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    MapFollowMe(TRUE);

    VStackOnObjectsOfType(ReliveTypes::eFleech);

    mShadow = relive_new Shadow();

    sFleechCount_5BC20E++;
}

void Fleech::InitTonguePolys_42B6E0()
{
    field_18A.Clear(Flags_18A::e18A_TongueActive_Bit1);
    field_18A.Clear(Flags_18A::e18A_Render_Bit2);

    field_180_tongue_x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
    field_182_tongue_y = FP_GetExponent(FP_FromInteger(2) * ((FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? -10 : -5)) + mBaseAnimatedWithPhysicsGameObject_YPos));

    field_178_tongue_state = 1;

    field_184_target_x = -1;
    field_186_target_y = -1;

    for (s32 i = 0; i < 4; i++)
    {
        for (s32 j = 0; j < 2; j++)
        {
            PolyG4_Init_4F88B0(&field_18C_tongue_polys1[i][j]);
            SetRGB0(&field_18C_tongue_polys1[i][j], 150, 100, 100);
            SetRGB1(&field_18C_tongue_polys1[i][j], 150, 100, 100);
            SetRGB2(&field_18C_tongue_polys1[i][j], 150, 100, 100);
            SetRGB3(&field_18C_tongue_polys1[i][j], 150, 100, 100);
            Poly_Set_SemiTrans_4F8A60(&field_18C_tongue_polys1[i][j].mBase.header, FALSE);

            PolyG4_Init_4F88B0(&field_2CC_tongue_polys2[i][j]);
            SetRGB0(&field_2CC_tongue_polys2[i][j], 150, 100, 100);
            SetRGB1(&field_2CC_tongue_polys2[i][j], 150, 100, 100);
            SetRGB2(&field_2CC_tongue_polys2[i][j], 150, 100, 100);
            SetRGB3(&field_2CC_tongue_polys2[i][j], 150, 100, 100);
            Poly_Set_SemiTrans_4F8A60(&field_2CC_tongue_polys2[i][j].mBase.header, TRUE);
        }
    }
}

void Fleech::SetAnim_429D80()
{
    u8** ppRes = ResBlockForMotion_42A530(mCurrentMotion);
    const AnimRecord& animRec = AnimRec(sFleechFrameTableOffsets_5517E4[mCurrentMotion]);
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, ppRes);
}

void Fleech::ResetTarget_42CF70()
{
    if (current_target_object_id_551840 == field_8_object_id)
    {
        current_target_object_id_551840 = -1;
    }
}

s16 Fleech::GotNoTarget_42CFA0()
{
    return current_target_object_id_551840 == -1 && !mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted);
}

void Fleech::SetTarget_42CF50()
{
    current_target_object_id_551840 = field_8_object_id;
}

void Fleech::TongueHangingFromWall_42B9A0(s16 target_x, s16 target_y)
{
    field_18A.Set(Flags_18A::e18A_Render_Bit2);
    field_178_tongue_state = 2;
    field_186_target_y = target_y;
    field_184_target_x = target_x;
    field_188 = 0;
}

void Fleech::TongueUpdate_42BD30()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_11C_obj_id));
    if (!gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        field_18A.Clear(Flags_18A::e18A_Render_Bit2);
    }

    switch (field_178_tongue_state)
    {
        case 1:
            field_18A.Clear(Flags_18A::e18A_TongueActive_Bit1);
            field_18A.Clear(Flags_18A::e18A_Render_Bit2);
            return;

        case 2:
        case 4:
            field_18A.Set(Flags_18A::e18A_Render_Bit2);
            return;

        case 3:
            field_18A.Set(Flags_18A::e18A_Render_Bit2);
            switch (field_17A_tongue_sub_state++)
            {
                case 0:
                    field_184_target_x = field_160_hoistX;
                    field_186_target_y = field_162_hoistY;
                    Sound_430520(FleechSound::LedgeHoist_11);
                    break;

                case 1:
                    field_188 = 6;
                    break;

                case 2:
                    field_188 = -6;
                    break;

                case 3:
                    field_188 = 3;
                    break;

                case 4:
                    field_188 = -3;
                    break;

                case 5:
                    field_188 = 0;
                    field_178_tongue_state = 4;
                    field_18A.Clear(Flags_18A::e18A_TongueActive_Bit1);
                    break;
                default:
                    break;
            }
            return;

        case 5:
        case 10:
            field_178_tongue_state = 1;
            return;

        case 6:
            if (pTarget)
            {
                const PSX_RECT bRect = pTarget->VGetBoundingRect();

                field_18A.Set(Flags_18A::e18A_Render_Bit2);
                field_17C = FP_GetExponent(pTarget->mBaseAnimatedWithPhysicsGameObject_XPos);
                field_17E = (bRect.y + bRect.h) >> 1;
                field_186_target_y = (bRect.y + bRect.h) >> 1;
                field_184_target_x = field_17C;

                const FP v12 = (FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 20 : 10) * FP_FromInteger(7));
                if (FP_FromInteger(Math_Distance(
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                        field_184_target_x, field_186_target_y))
                    <= v12)
                {
                    switch (field_17A_tongue_sub_state++)
                    {
                        case 0:
                        {
                            Sound_430520(FleechSound::LickTarget_10);
                            relive_new Blood(
                                FP_FromInteger(field_17C),
                                FP_FromInteger(field_17E),
                                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != 0 ? FP_FromInteger(2) : FP_FromInteger(-2),
                                FP_FromInteger(1),
                                mBaseAnimatedWithPhysicsGameObject_SpriteScale, 20);
                            break;
                        }

                        case 1:
                            field_188 = 9;
                            break;

                        case 2:
                            field_188 = -6;
                            ResetTarget_42CF70();
                            break;

                        case 3:
                            field_188 = 5;
                            break;

                        case 4:
                            field_188 = -3;
                            break;

                        case 5:
                            field_178_tongue_state = 9;
                            field_188 = 0;
                            if (pTarget->mHealth > FP_FromInteger(0))
                            {
                                pTarget->VTakeDamage(this);
                                if (pTarget->mHealth <= FP_FromInteger(0))
                                {
                                    field_178_tongue_state = 7;
                                    field_17A_tongue_sub_state = 0;
                                    Sound_430520(FleechSound::Devour_8);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    field_18A.Clear(Flags_18A::e18A_TongueActive_Bit1);
                    field_178_tongue_state = 1;
                }
            }
            else
            {
                sub_42B8C0();
            }
            return;

        case 7:
            if (pTarget)
            {
                const PSX_RECT bRect = pTarget->VGetBoundingRect();

                switch (field_17A_tongue_sub_state++)
                {
                    case 4:
                        mNextMotion = eFleechMotions::M_Consume_18_42FDF0;
                        [[fallthrough]];
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 5:
                        if (field_17A_tongue_sub_state == 5 && pTarget->Type() == ReliveTypes::eScrab)
                        {
                            pTarget->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                        }
                        pTarget->mBaseAnimatedWithPhysicsGameObject_XPos -= (pTarget->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) * FP_FromDouble(0.25);
                        pTarget->mBaseAnimatedWithPhysicsGameObject_YPos -= (pTarget->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos) * FP_FromDouble(0.25);
                        break;

                    case 6:
                        field_18A.Clear(Flags_18A::e18A_Render_Bit2);
                        field_178_tongue_state = 8;
                        pTarget->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                        pTarget->mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_XPos;
                        pTarget->mBaseAnimatedWithPhysicsGameObject_YPos = mBaseAnimatedWithPhysicsGameObject_YPos;
                        if (pTarget == sActiveHero)
                        {
                            sActiveHero->FleechDeath_459350();
                        }
                        break;

                    default:
                        break;
                }

                field_17C = FP_GetExponent(pTarget->mBaseAnimatedWithPhysicsGameObject_XPos);
                field_184_target_x = field_17C;
                field_17E = (bRect.y + bRect.h) >> 1;
                field_186_target_y = (bRect.y + bRect.h) >> 1;
            }
            else
            {
                sub_42B8C0();
            }
            return;

        case 8:
            field_18A.Clear(Flags_18A::e18A_TongueActive_Bit1);
            field_178_tongue_state = 1;
            return;

        case 9:
            if (pTarget)
            {
                const PSX_RECT bRect = pTarget->VGetBoundingRect();
                field_17C = FP_GetExponent((mBaseAnimatedWithPhysicsGameObject_XPos + pTarget->mBaseAnimatedWithPhysicsGameObject_XPos) * FP_FromDouble(0.5));
                field_17E = (bRect.y + bRect.h) >> 1;
                field_184_target_x = FP_GetExponent((mBaseAnimatedWithPhysicsGameObject_XPos + pTarget->mBaseAnimatedWithPhysicsGameObject_XPos) * FP_FromDouble(0.5));
                field_186_target_y = (bRect.y + bRect.h) >> 1;
                field_18A.Clear(Flags_18A::e18A_TongueActive_Bit1);
                field_178_tongue_state = 1;
            }
            else
            {
                sub_42B8C0();
            }
            break;

        default:
            break;
    }
}

void Fleech::ToIdle_42E520()
{
    MapFollowMe(TRUE);
    field_138_velx_factor = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    mCurrentMotion = eFleechMotions::M_Idle_3_42E850;
    mNextMotion = -1;
    field_134_unused = 60 * sRandomBytes_546744[sFleechRandomIdx_5BC20C++] / 256 + sGnFrame + 120;
}

const SfxDefinition getSfxDef(FleechSound effectId)
{
    return stru_5518E0[static_cast<s32>(effectId)];
}

s32 Fleech::Sound_430520(FleechSound soundId)
{
    SfxDefinition effectDef = getSfxDef(soundId);
    s16 defaultSndIdxVol = effectDef.field_3_default_volume;
    if (soundId == FleechSound::CrawlRNG1_14)
    {
        const s32 rndIdx = Math_RandomRange(14, 16);
        effectDef = getSfxDef(static_cast<FleechSound>(rndIdx));
        defaultSndIdxVol = effectDef.field_3_default_volume + Math_RandomRange(-10, 10);
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
    {
        defaultSndIdxVol = 2 * defaultSndIdxVol / 3;
    }

    s16 volumeLeft = 0;
    s16 volumeRight = defaultSndIdxVol;
    const CameraPos direction = gMap.GetDirection_4811A0(
        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
        mBaseAnimatedWithPhysicsGameObject_PathNumber,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos);

    PSX_RECT pRect = {};
    gMap.Get_Camera_World_Rect(direction, &pRect);
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
            volumeLeft = volumeRight;
            break;
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
            volumeRight = volumeLeft;
        }
        break;
        case CameraPos::eCamLeft_3:
        {
            const FP percentHowFar = (FP_FromInteger(pRect.w) - mBaseAnimatedWithPhysicsGameObject_XPos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
        }
        break;
        case CameraPos::eCamRight_4:
        {
            const FP percentHowFar = (mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
        }
        break;
        default:
            return 0;
    }

    return SFX_SfxDefinition_Play_Stereo(
        &effectDef,
        volumeLeft,
        volumeRight,
        effectDef.field_4_pitch_min,
        effectDef.field_6_pitch_max);
}

u8** Fleech::ResBlockForMotion_42A530(s32 /*motion*/)
{
    return field_10_resources_array.ItemAt(0);
}

s16 Fleech::CanMove_42E3E0()
{
    if (mNextMotion == eFleechMotions::M_Knockback_6_42EAF0)
    {
        mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
        mNextMotion = -1;
        return 1;
    }

    if (mNextMotion != eFleechMotions::M_Crawl_4_42E960)
    {
        return 0;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(7));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(7));
    }

    const FP yDist = FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5);
    const FP xDist = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? -1 : 1); // TODO: Correct way around ?
    if (!WallHit(yDist, xDist) && !HandleEnemyStopperOrSlamDoor_42ADC0(1))
    {
        mCurrentMotion = eFleechMotions::M_Crawl_4_42E960;
        mNextMotion = -1;
        return 1;
    }

    ToIdle_42E520();
    return 0;
}

s16 Fleech::HandleEnemyStopperOrSlamDoor_42ADC0(s32 velX)
{
    const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    FP nextXPos = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        nextXPos = mBaseAnimatedWithPhysicsGameObject_XPos - (kGridSize * FP_FromInteger(velX));
    }
    else
    {
        nextXPos = (kGridSize * FP_FromInteger(velX)) + mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    FP stopperXPos = {};
    if (mBaseAnimatedWithPhysicsGameObject_XPos <= nextXPos)
    {
        stopperXPos = nextXPos;
    }
    else
    {
        stopperXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    auto pStopper = static_cast<Path_EnemyStopper*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(stopperXPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(stopperXPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::EnemyStopper_47));

    if (pStopper && (pStopper->field_10_stop_direction == (nextXPos >= mBaseAnimatedWithPhysicsGameObject_XPos ? Path_EnemyStopper::StopDirection::Right_1 : Path_EnemyStopper::StopDirection::Left_0)) && SwitchStates_Get(pStopper->field_12_switch_id))
    {
        return 1;
    }

    FP slamDoorXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    if (mBaseAnimatedWithPhysicsGameObject_XPos <= nextXPos)
    {
        slamDoorXPos = nextXPos;
    }

    if (mBaseAnimatedWithPhysicsGameObject_XPos > nextXPos)
    {
        slamDoorXPos = nextXPos;
    }

    auto pSlamDoor = static_cast<Path_SlamDoor*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(slamDoorXPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(slamDoorXPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::SlamDoor_85));

    return (pSlamDoor && ((pSlamDoor->field_10_bStart_closed == Choice_short::eYes_1 && !SwitchStates_Get(pSlamDoor->field_14_switch_id)) || (pSlamDoor->field_10_bStart_closed == Choice_short::eNo_0 && SwitchStates_Get(pSlamDoor->field_14_switch_id))));
}

s32 Fleech::UpdateWakeUpSwitchValue_4308B0()
{
    const s16 curSwitchValue = static_cast<s16>(SwitchStates_Get(field_144_wake_up_switch_id));
    const s16 wakeUpValue = field_148_wake_up_switch_value;

    if (curSwitchValue == wakeUpValue)
    {
        return 0;
    }

    if (curSwitchValue)
    {
        field_148_wake_up_switch_value = curSwitchValue;
        return (curSwitchValue - wakeUpValue) ? 1 : 0;
    }
    else
    {
        field_148_wake_up_switch_value = 0;
        return 1;
    }
}

s16 Fleech::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return 0;
    }

    sub_42B8C0();
    ResetTarget_42CF70();

    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        case ReliveTypes::eDrill:
        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eExplosion:
        case ReliveTypes::eSlig:
        {
            Sound_430520(FleechSound::Scared_7);
            mHealth = FP_FromInteger(0);

            relive_new Gibs(GibType::Fleech_10, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX, mBaseAnimatedWithPhysicsGameObject_VelY, mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0);

            const PSX_RECT bRect = VGetBoundingRect();
            relive_new Blood(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2),
                FP_FromInteger(0),
                FP_FromInteger(0),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale, 50);

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        break;


        case ReliveTypes::eRockSpawner:
            Sound_430520(FleechSound::Scared_7);
            mHealth = FP_FromInteger(0);
            field_124_brain_state = eFleechBrains::eBrain_3_Death_42D1E0;
            mNextMotion = -1;
            field_12C = sGnFrame + 127;
            mCurrentMotion = eFleechMotions::M_Idle_3_42E850;
            SetAnim_429D80();
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
            break;

        case ReliveTypes::eParamite:
            Sound_430520(FleechSound::Dismember_13);
            [[fallthrough]];

        case ReliveTypes::eScrab:
        {
            relive_new Gibs(GibType::Fleech_10, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX, mBaseAnimatedWithPhysicsGameObject_VelY, mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0);

            if (static_cast<BaseAliveGameObject*>(pFrom)->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(8), -FP_FromInteger(5), -FP_FromInteger(5), mBaseAnimatedWithPhysicsGameObject_SpriteScale, 50);
            }
            else
            {
                relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(8), FP_FromInteger(5), -FP_FromInteger(5), mBaseAnimatedWithPhysicsGameObject_SpriteScale, 50);
            }

            if (!field_174_flags.Get(Flags_174::eBit4))
            {
                field_174_flags.Set(Flags_174::eBit4);
                Sound_430520(FleechSound::Scared_7);
            }

            mHealth = FP_FromInteger(0);
            field_124_brain_state = eFleechBrains::eBrain_3_Death_42D1E0;
            mCurrentMotion = eFleechMotions::M_Idle_3_42E850;
            field_12C = sGnFrame + 127;
            mNextMotion = -1;
            SetAnim_429D80();
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
            field_174_flags.Set(Flags_174::eBit3);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            sFleechCount_5BC20E--;
        }
        break;

        case ReliveTypes::eElectrocute:
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            mHealth = FP_FromInteger(0);
            field_124_brain_state = eFleechBrains::eBrain_3_Death_42D1E0;
            break;

        default:
            Sound_430520(FleechSound::Scared_7);
            mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit7_Electrocuted);
            sub_42B8C0();
            break;
    }
    return 1;
}

void Fleech::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObjectId = -1;
    }
}

void Fleech::SetTongueState1_42B9F0()
{
    field_178_tongue_state = 1;
}

void Fleech::IncreaseAnger_430920()
{
    if (gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        IBaseAnimatedWithPhysicsGameObject* pEvent = Event_Is_Event_In_Range(kEventSpeaking, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_Scale);

        if (!pEvent)
        {
            pEvent = Event_Is_Event_In_Range(kEventAlarm, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_Scale);
        }

        if (!pEvent)
        {
            pEvent = Event_Is_Event_In_Range(kEventLoudNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_Scale);
        }

        if (pEvent)
        {
            if ((pEvent != sActiveHero || !sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible)) && gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, pEvent->mBaseAnimatedWithPhysicsGameObject_XPos, pEvent->mBaseAnimatedWithPhysicsGameObject_YPos, 0))
            {
                field_13E_current_anger += field_142_attack_anger_increaser;
                if (VOnSameYLevel(static_cast<BaseAnimatedWithPhysicsGameObject*>(pEvent)))
                {
                    if (pEvent->Type() == ReliveTypes::eScrab || pEvent->Type() == ReliveTypes::eParamite)
                    {
                        field_14E = FP_GetExponent(pEvent->mBaseAnimatedWithPhysicsGameObject_XPos);
                    }
                    else
                    {
                        field_14C = FP_GetExponent(pEvent->mBaseAnimatedWithPhysicsGameObject_YPos);
                    }
                }
            }
        }

        pEvent = Event_Is_Event_In_Range(kEventNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_Scale);
        if (pEvent)
        {
            if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(6),static_cast<BaseAnimatedWithPhysicsGameObject*>(pEvent)))
            {
                if ((pEvent != sActiveHero || !sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible)) && gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, pEvent->mBaseAnimatedWithPhysicsGameObject_XPos, pEvent->mBaseAnimatedWithPhysicsGameObject_YPos, 0))
                {
                    field_13E_current_anger += field_140_max_anger;
                }
            }
        }
    }
}

s16 Fleech::InRange_4307C0(BaseAliveGameObject* pObj)
{
    if (!pObj || (pObj == sActiveHero && sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible)))
    {
        return FALSE;
    }

    if (FP_Abs(pObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) >= ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(10) || FP_Abs(pObj->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos) >= ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(1) || pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale != mBaseAnimatedWithPhysicsGameObject_SpriteScale)
    {
        return FALSE;
    }

    return TRUE;
}

s32 Fleech::TongueActive_42B8A0()
{
    return field_18A.Get(Flags_18A::e18A_TongueActive_Bit1);
}

void Fleech::PullTargetIn_42BAF0()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_11C_obj_id));
    if (pTarget)
    {
        field_18A.Set(Flags_18A::e18A_TongueActive_Bit1);
        field_18A.Set(Flags_18A::e18A_Render_Bit2);
        field_178_tongue_state = 6;
        field_17A_tongue_sub_state = 0;

        const PSX_RECT bRect = pTarget->VGetBoundingRect();

        field_17C = FP_GetExponent(pTarget->mBaseAnimatedWithPhysicsGameObject_XPos);
        field_17E = (bRect.y + bRect.h) / 2;
        field_184_target_x = FP_GetExponent(((FP_FromInteger(field_17C) + mBaseAnimatedWithPhysicsGameObject_XPos) / FP_FromInteger(2)));
        field_188 = 0;
        field_186_target_y = FP_GetExponent(((FP_FromInteger(field_17E) + mBaseAnimatedWithPhysicsGameObject_YPos) / FP_FromInteger(2)));
    }
}

void Fleech::sub_42B8C0()
{
    ResetTarget_42CF70();

    if (field_178_tongue_state > 1)
    {
        field_178_tongue_state = 10;
        field_184_target_x = FP_GetExponent(((FP_FromInteger(field_184_target_x)) + mBaseAnimatedWithPhysicsGameObject_XPos) / FP_FromInteger(2));
        field_188 = 0;
        field_186_target_y = FP_GetExponent(((FP_FromInteger(field_186_target_y)) + mBaseAnimatedWithPhysicsGameObject_YPos) / FP_FromInteger(2));
    }
    else
    {
        field_18A.Clear(Flags_18A::e18A_Render_Bit2);
        field_178_tongue_state = 1;
    }
}

void Fleech::sub_42BA10()
{
    field_18A.Set(Flags_18A::e18A_TongueActive_Bit1);
    field_18A.Set(Flags_18A::e18A_Render_Bit2);
    field_178_tongue_state = 3;
    field_17A_tongue_sub_state = 0;
    field_184_target_x = FP_GetExponent(((FP_FromInteger(field_160_hoistX)) + mBaseAnimatedWithPhysicsGameObject_XPos) / FP_FromInteger(2));
    field_188 = 0;
    field_186_target_y = FP_GetExponent(((FP_FromInteger(field_162_hoistY)) + mBaseAnimatedWithPhysicsGameObject_YPos) / FP_FromInteger(2));
}

void Fleech::SetTongueState5_42BAD0()
{
    field_178_tongue_state = 5;
}

BaseAliveGameObject* Fleech::FindMudOrAbe_42CFD0()
{
    BaseAliveGameObject* pRet = nullptr;
    FP lastDist = FP_FromInteger(gPsxDisplay_5C1130.field_0_width);
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if ((pObj->Type() == ReliveTypes::eMudokon || pObj->Type() == ReliveTypes::eAbe) && pObj->mBaseAnimatedWithPhysicsGameObject_Scale == mBaseAnimatedWithPhysicsGameObject_Scale && pObj->mHealth > FP_FromInteger(0))
        {
            const FP dist = FP_FromInteger(
                Math_Distance(
                    FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_YPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos)));

            if (dist < lastDist && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) / 375 == (FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos) / 375) && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) / 260 == (FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_YPos) / 260))
            {
                lastDist = dist;
                pRet = pObj;
            }
        }
    }
    return pRet;
}

void Fleech::MoveAlongFloor_42E600()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));

    const FP prev_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP prev_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        PathLine* pOldLine = BaseAliveGameObjectCollisionLine;
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX);
        if (BaseAliveGameObjectCollisionLine && ((mBaseAnimatedWithPhysicsGameObject_Scale != 0 ? 1 : 16) & (1 << BaseAliveGameObjectCollisionLine->field_8_type)))
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->field_8_type != 32 && BaseAliveGameObjectCollisionLine->field_8_type != 36)
                {
                    pPlatform->VRemove(this);
                    BaseAliveGameObjectId = -1;
                }
            }
            else if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_32 || BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_36)
            {
                const PSX_RECT bRect = VGetBoundingRect();
                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                VOnCollisionWith(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
            }
        }
        else if (field_124_brain_state != eFleechBrains::eBrain_0_Patrol_430BA0)
        {
            VOnTrapDoorOpen();
            field_138_velx_factor = FP_FromInteger(0);
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
            mBaseAnimatedWithPhysicsGameObject_XPos = prev_xpos + mBaseAnimatedWithPhysicsGameObject_VelX;
            mCurrentMotion = eFleechMotions::M_Fall_9_42ECD0;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_XPos = prev_xpos;
            mBaseAnimatedWithPhysicsGameObject_YPos = prev_ypos;
            BaseAliveGameObjectCollisionLine = pOldLine;
            mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
        }
    }
    else
    {
        BaseAliveGameObjectLastLineYPos = prev_ypos;
        field_138_velx_factor = FP_FromInteger(0);
        mCurrentMotion = eFleechMotions::M_Fall_9_42ECD0;
    }
}

s16 Fleech::IsNear_428670(BaseAliveGameObject* pObj)
{
    if (pObj && mBaseAnimatedWithPhysicsGameObject_SpriteScale == pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale && FP_GetExponent(FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - pObj->mBaseAnimatedWithPhysicsGameObject_XPos)) <= 750 && FP_GetExponent(FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pObj->mBaseAnimatedWithPhysicsGameObject_YPos)) <= 260)
    {
        return TRUE;
    }
    return FALSE;
}

void Fleech::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    field_13E_current_anger += field_142_attack_anger_increaser;
}

Bool32 Fleech::Collision_42B290(s16 alwaysOne)
{
    const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const FP quaterScaled = (kGridSize * FP_FromDouble(0.25));

    FP x2 = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        x2 = mBaseAnimatedWithPhysicsGameObject_XPos - (quaterScaled * FP_FromInteger(alwaysOne >= 0 ? 1 : -1));
    }
    else
    {
        x2 = (kGridSize * FP_FromInteger(alwaysOne)) + mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    FP x1 = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        x1 = mBaseAnimatedWithPhysicsGameObject_XPos - (kGridSize * FP_FromInteger(alwaysOne));
    }
    else
    {
        x1 = (quaterScaled * FP_FromInteger(alwaysOne >= 0 ? 1 : -1)) + mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    const FP y2 = mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(2);
    const FP y1 = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(2);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions->Raycast(x1, y1, x2, y2, &pLine, &hitX, &hitY, mBaseAnimatedWithPhysicsGameObject_Scale ? 0x01 : 0x10) == 0;
}

Path_Hoist* Fleech::TryGetHoist_42AFD0(s32 xDistance, s16 bIgnoreDirection)
{
    if (mCurrentMotion == eFleechMotions::M_Fall_9_42ECD0)
    {
        return nullptr;
    }

    const FP yAmount = FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(1) ? 10 : 20);
    const FP y1 = mBaseAnimatedWithPhysicsGameObject_YPos - yAmount;
    const FP y2 = y1 - (yAmount * FP_FromInteger(1));

    const FP xSnapped = FP_FromInteger(SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)));
    FP xCheck = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xCheck = xSnapped - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(xDistance));
    }
    else
    {
        xCheck = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(xDistance)) + xSnapped;
    }

    auto pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(std::min(xCheck, mBaseAnimatedWithPhysicsGameObject_XPos)),
        FP_GetExponent(y2),
        FP_GetExponent(std::max(xCheck, mBaseAnimatedWithPhysicsGameObject_XPos)),
        FP_GetExponent(y1),
        TlvTypes::Hoist_2));

    if (!pHoist)
    {
        return nullptr;
    }

    if (WallHit(
            FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(1) ? 5 : 10),
            FP_FromInteger(pHoist->field_8_top_left.field_0_x + (mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(1) ? 6 : 12)) - mBaseAnimatedWithPhysicsGameObject_XPos))
    {
        return nullptr;
    }

    if (HandleEnemyStopperOrSlamDoor_42ADC0(xDistance))
    {
        return nullptr;
    }

    if (pHoist->field_16_scale != (mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(1) ? Scale_short::eHalf_1 : Scale_short::eFull_0) || mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(pHoist->field_8_top_left.field_2_y) > FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 20 : 10) * FP_FromDouble(5.5))
    {
        return nullptr;
    }

    if (bIgnoreDirection)
    {
        return pHoist;
    }

    if (pHoist->field_12_grab_direction == (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? Path_Hoist::GrabDirection::eFacingLeft : Path_Hoist::GrabDirection::eFacingRight) || pHoist->field_12_grab_direction == Path_Hoist::GrabDirection::eFacingAnyDirection)
    {
        return pHoist;
    }

    return nullptr;
}

void Fleech::vOnFrame_42BC50(s16* pData)
{
    FP xpos = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xpos = mBaseAnimatedWithPhysicsGameObject_XPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]));
    }
    else
    {
        xpos = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0])) + mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    field_180_tongue_x = FP_GetExponent(xpos) + mBaseAnimatedWithPhysicsGameObject_XOffset;
    field_182_tongue_y = FP_GetExponent((mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[1])) + mBaseAnimatedWithPhysicsGameObject_YPos);
}

const s8 byte_551984[] = {
    0,
    0,
    5,
    0,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    0,
    0,
    0,
    0};

enum PatrolStates
{
    State_0_Init = 0,
    State_1_Sleeping = 1,
    State_2 = 2,
    State_3_To_Sleep = 3,
    State_4_Alert = 4,
    State_5 = 5,
    State_6 = 6,
    State_7 = 7,
    State_8 = 8,
    State_9 = 9,
    State_10 = 10,
};

s16 Fleech::Brain_0_Patrol_430BA0()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_11C_obj_id));
    if (!pTarget || pTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead) || pTarget->mHealth <= FP_FromInteger(0) || pTarget->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_11C_obj_id = -1;
        pTarget = nullptr;
    }

    if (gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        MusicController::static_PlayMusic(static_cast<MusicController::MusicTypes>(byte_551984[field_126_state]), this, 0, 0);
    }
    else
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (field_126_state)
    {
        case PatrolStates::State_0_Init:
            return Brain_Patrol_State_0();

        case PatrolStates::State_1_Sleeping:
            return Brain_Patrol_State_1();

        case PatrolStates::State_2:
            return Brain_Patrol_State_2();

        case PatrolStates::State_3_To_Sleep:
            return Brain_Patrol_State_3();

        case PatrolStates::State_4_Alert:
            return Brain_Patrol_State_4(pTarget);

        case PatrolStates::State_5:
            return Brain_Patrol_State_5();

        case PatrolStates::State_6:
            return Brain_Patrol_State_6();

        case PatrolStates::State_7:
            return Brain_Patrol_State_7();

        case PatrolStates::State_8:
            return Brain_Patrol_State_8(pTarget);

        case PatrolStates::State_9:
            return Brain_Patrol_State_9();

        case PatrolStates::State_10:
            return Brain_Patrol_State_10();

        default:
            return field_126_state;
    }
}

s16 Fleech::Brain_Patrol_State_0()
{
    field_156_rnd_crawl = Fleech_NextRandom() & 0x3F;
    field_15A_chase_timer = 0;
    field_152 = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
    field_14C = -1;
    field_14E = -1;

    if (field_13E_current_anger > field_140_max_anger)
    {
        return PatrolStates::State_4_Alert;
    }

    if (mCurrentMotion == eFleechMotions::M_Sleeping_0_42F0B0)
    {
        return PatrolStates::State_1_Sleeping;
    }

    if (mCurrentMotion != eFleechMotions::M_SleepingWithTongue_17_42F370 && !field_174_flags.Get(Flags_174::eBit6_goes_to_sleep))
    {
        return PatrolStates::State_4_Alert;
    }

    if (mCurrentMotion == eFleechMotions::M_Sleeping_0_42F0B0 || mCurrentMotion == eFleechMotions::M_SleepingWithTongue_17_42F370)
    {
        return PatrolStates::State_1_Sleeping;
    }

    return PatrolStates::State_3_To_Sleep;
}

s16 Fleech::Brain_Patrol_State_1()
{
    if (!SwitchStates_Get(field_14A_can_wake_up_switch_id))
    {
        return field_126_state;
    }

    if (!(static_cast<s32>(sGnFrame) % 16))
    {
        if (field_13E_current_anger > 0)
        {
            field_13E_current_anger--;
        }
    }

    IncreaseAnger_430920();

    if (UpdateWakeUpSwitchValue_4308B0())
    {
        const s16 v11 = (field_142_attack_anger_increaser - field_140_max_anger) / 2;
        if (field_13E_current_anger < v11 + field_140_max_anger)
        {
            field_13E_current_anger = field_140_max_anger + v11;
        }
    }

    if (!IsScrabOrParamiteNear(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(8)))
    {
        if (field_13E_current_anger <= field_140_max_anger)
        {
            return field_126_state;
        }

        if (mCurrentMotion == eFleechMotions::M_SleepingWithTongue_17_42F370)
        {
            mNextMotion = eFleechMotions::M_Fall_9_42ECD0;
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
            return PatrolStates::State_4_Alert;
        }
    }
    else
    {
        BaseGameObject* pDangerObj = sObjectIds.Find_Impl(field_170_danger_obj);
        if (pDangerObj && pDangerObj->Type() != ReliveTypes::eParamite)
        {
            if (mCurrentMotion == eFleechMotions::M_SleepingWithTongue_17_42F370)
            {
                mNextMotion = eFleechMotions::M_Fall_9_42ECD0;
                BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
                return PatrolStates::State_4_Alert;
            }
        }
        else
        {
            field_170_danger_obj = -1;
            if (field_13E_current_anger <= field_140_max_anger)
            {
                return field_126_state;
            }

            if (mCurrentMotion == eFleechMotions::M_SleepingWithTongue_17_42F370)
            {
                mNextMotion = eFleechMotions::M_Fall_9_42ECD0;
                BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
                return PatrolStates::State_4_Alert;
            }
        }
    }
    mNextMotion = eFleechMotions::M_WakingUp_1_42F270;
    return PatrolStates::State_2;
}

s16 Fleech::Brain_Patrol_State_2()
{
    if (mCurrentMotion != eFleechMotions::M_WakingUp_1_42F270)
    {
        return field_126_state;
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        mNextMotion = eFleechMotions::M_PatrolCry_5_42E810;
    }
    else
    {
        mNextMotion = eFleechMotions::M_Fall_9_42ECD0;
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
    }
    return PatrolStates::State_4_Alert;
}

s16 Fleech::Brain_Patrol_State_3()
{
    if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
    {
        mNextMotion = eFleechMotions::M_Unknown_2_42F2F0;
    }

    if (mCurrentMotion != eFleechMotions::M_Unknown_2_42F2F0)
    {
        return field_126_state;
    }

    field_13E_current_anger = 0;
    mNextMotion = eFleechMotions::M_Sleeping_0_42F0B0;
    return PatrolStates::State_1_Sleeping;
}

s16 Fleech::Brain_Patrol_State_4(BaseAliveGameObject* pTarget)
{
    if (field_11C_obj_id == -1)
    {
        pTarget = FindMudOrAbe_42CFD0();
        if (pTarget)
        {
            field_11C_obj_id = pTarget->field_8_object_id;
        }
    }

    if (!(static_cast<s32>(sGnFrame) % 32))
    {
        if (field_13E_current_anger > 0)
        {
            field_13E_current_anger--;
        }
    }

    if (IsScrabOrParamiteNear(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(8)))
    {
        auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
        if (pDangerObj == sControlledCharacter_5C1B8C)
        {
            field_124_brain_state = eFleechBrains::eBrain_2_Scared_42D310;
            return 0;
        }

        if (VIsObjNearby(FP_FromInteger(2) * ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale), pDangerObj))
        {
            const s16 v27 = (field_142_attack_anger_increaser - field_140_max_anger) / 2;
            if (field_13E_current_anger < v27 + field_140_max_anger)
            {
                field_13E_current_anger = field_140_max_anger + v27;
            }
            field_14E = FP_GetExponent(pDangerObj->mBaseAnimatedWithPhysicsGameObject_XPos); // TODO: abs ?
        }
    }

    // TODO: Check OFSUB branches
    if (field_14E >= 0)
    {
        if ((FP_FromInteger(field_14E) > mBaseAnimatedWithPhysicsGameObject_XPos && !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) || (FP_FromInteger(field_14E) < mBaseAnimatedWithPhysicsGameObject_XPos && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
        {
            if (mCurrentMotion == eFleechMotions::M_Crawl_4_42E960)
            {
                mCurrentMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
            }
            else if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
            {
                mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
            }
        }
        return PatrolStates::State_5;
    }

    IncreaseAnger_430920();

    if (InRange_4307C0(pTarget))
    {
        field_13E_current_anger += field_142_attack_anger_increaser;
    }

    if (UpdateWakeUpSwitchValue_4308B0())
    {
        field_13E_current_anger += field_146_wake_up_switch_anger_value;
    }

    if (pTarget)
    {
        if (!pTarget->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && VOnSameYLevel(pTarget) && gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, pTarget->mBaseAnimatedWithPhysicsGameObject_XPos, pTarget->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5), pTarget->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos))
        {
            field_13E_current_anger = field_142_attack_anger_increaser + 1;
            return PatrolStates::State_8;
        }

        if (field_13E_current_anger > field_142_attack_anger_increaser && !pTarget->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) && field_170_danger_obj == -1)
        {
            return PatrolStates::State_8;
        }
    }

    field_15A_chase_timer = 0;
    Path_Hoist* pHoist = TryGetHoist_42AFD0(0, 0);
    if (pHoist)
    {
        if (mCurrentMotion == eFleechMotions::M_Crawl_4_42E960)
        {
            mNextMotion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
        }
        else if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
        {
            mNextMotion = eFleechMotions::M_Idle_3_42E850;
        }
        field_160_hoistX = pHoist->field_8_top_left.field_0_x + (mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 12 : 6);
        field_162_hoistY = pHoist->field_8_top_left.field_2_y;
        return 9;
    }

    if (!(Fleech_NextRandom() % 32) && mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
    {
        mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
        return field_126_state;
    }

    if (mCurrentMotion == eFleechMotions::M_Crawl_4_42E960)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            if (mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(field_154))
            {
                mNextMotion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
            }
        }
        else if (mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(field_154))
        {
            mNextMotion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
        }
    }

    if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
    {
        if (field_14C >= 0)
        {
            if (field_150_patrol_range > 0)
            {
                if (FP_FromInteger(field_14C) <= mBaseAnimatedWithPhysicsGameObject_XPos)
                {
                    s16 patrolRangeDelta = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) - field_150_patrol_range;
                    if (field_14C > patrolRangeDelta)
                    {
                        patrolRangeDelta = field_14C;
                    }
                    field_154 = patrolRangeDelta;

                    if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                    }
                }
                else
                {
                    s16 patrolRangeDelta = field_150_patrol_range + FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
                    if (field_14C <= patrolRangeDelta)
                    {
                        patrolRangeDelta = field_14C;
                    }
                    field_154 = patrolRangeDelta;

                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                    }
                }

                mNextMotion = eFleechMotions::M_Crawl_4_42E960;
                field_14C = -1;
            }
            else
            {
                // TODO: Check __OFSUB__ on else branch
                if (FP_FromInteger(field_14C) > mBaseAnimatedWithPhysicsGameObject_XPos)
                {
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                        field_14C = -1;
                    }
                }
                else if (FP_FromInteger(field_14C) < mBaseAnimatedWithPhysicsGameObject_XPos)
                {
                    if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                        field_14C = -1;
                    }
                }
            }
        }
        else
        {
            if (field_150_patrol_range > 0)
            {
                if (field_156_rnd_crawl > 0)
                {
                    field_156_rnd_crawl--;
                }
                else
                {
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        field_154 = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) - Fleech_NextRandom() * (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) + field_150_patrol_range - field_152) / 255;
                    }
                    else
                    {
                        field_154 = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) + Fleech_NextRandom() * (field_150_patrol_range + field_152 - FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)) / 255;
                    }
                    field_156_rnd_crawl = Fleech_NextRandom() & 0x3F;
                    mNextMotion = eFleechMotions::M_Crawl_4_42E960;
                }
            }
        }
    }

    if (!field_174_flags.Get(Flags_174::eBit6_goes_to_sleep) || (field_13E_current_anger >= field_140_max_anger && gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0)))
    {
        if ((Fleech_NextRandom() % 64) || mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
        {
            return field_126_state;
        }
        mCurrentMotion = eFleechMotions::M_PatrolCry_5_42E810;
        return field_126_state;
    }
    field_13E_current_anger = 0;
    mNextMotion = eFleechMotions::M_Unknown_2_42F2F0;
    return PatrolStates::State_3_To_Sleep;
}

s16 Fleech::Brain_Patrol_State_5()
{
    if (mCurrentMotion == eFleechMotions::M_StopCrawling_7_42EBB0 || mCurrentMotion == eFleechMotions::M_Knockback_6_42EAF0)
    {
        return field_126_state;
    }

    if ((FP_FromInteger(field_14E) > mBaseAnimatedWithPhysicsGameObject_XPos && !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) || (FP_FromInteger(field_14E) < mBaseAnimatedWithPhysicsGameObject_XPos && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
    {
        if (mCurrentMotion != eFleechMotions::M_Crawl_4_42E960)
        {
            if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
            {
                mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
            }
        }
        else
        {
            mCurrentMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
        }
        return field_126_state;
    }

    field_14E = -1;
    mNextMotion = eFleechMotions::M_Crawl_4_42E960;
    CanMove_42E3E0();
    return PatrolStates::State_6;
}

s16 Fleech::Brain_Patrol_State_6()
{
    auto pDangerObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
    if (mCurrentMotion != eFleechMotions::M_Crawl_4_42E960 || (pDangerObj && (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(5), pDangerObj))))
    {
        if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
        {
            return field_126_state;
        }
    }
    else
    {
        mNextMotion = eFleechMotions::M_Idle_3_42E850;
    }
    return PatrolStates::State_7;
}

s16 Fleech::Brain_Patrol_State_7()
{
    if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
    {
        return field_126_state;
    }
    return PatrolStates::State_4_Alert;
}

s16 Fleech::Brain_Patrol_State_8(BaseAliveGameObject* pTarget)
{
    if (pTarget == sActiveHero && sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        return PatrolStates::State_0_Init;
    }

    if (field_15A_chase_timer < field_158_chase_delay)
    {
        field_15A_chase_timer++;
        return field_126_state;
    }

    field_124_brain_state = eFleechBrains::eBrain_1_ChasingAbe_428760;
    return 0;
}

s16 Fleech::Brain_Patrol_State_9()
{
    if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
    {
        return field_126_state;
    }
    if ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && FP_FromInteger(field_160_hoistX) > mBaseAnimatedWithPhysicsGameObject_XPos) || (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && FP_FromInteger(field_160_hoistX) < mBaseAnimatedWithPhysicsGameObject_XPos))
    {
        mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
        return field_126_state;
    }
    mNextMotion = eFleechMotions::M_RaiseHead_11_42F590;
    return PatrolStates::State_10;
}

s16 Fleech::Brain_Patrol_State_10()
{
    if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
    {
        return field_126_state;
    }
    field_152 = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
    return PatrolStates::State_4_Alert;
}

const s8 byte_551784[] = {
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    0};

s16 Fleech::Brain_1_ChasingAbe_428760()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_11C_obj_id));
    if (pObj)
    {
        if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead) || (pObj == sActiveHero && sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible)))
        {
            field_11C_obj_id = -1;
            pObj = nullptr;
        }
    }

    if (gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        MusicController::static_PlayMusic(static_cast<MusicController::MusicTypes>(byte_551784[field_126_state]), this, 0, 0);
    }
    else
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (field_126_state)
    {
        case 0u:
            return Brain_ChasingAbe_State_0(pObj);
        case 1u:
            return Brain_ChasingAbe_State_1(pObj);
        case 2u:
            return Brain_ChasingAbe_State_2(pObj);
        case 3u:
        case 16u:
            if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
            {
                return field_126_state;
            }
            return 1;

        case 4u:
        {
            if (!pObj || pObj->mHealth <= FP_FromInteger(0))
            {
                return 13;
            }

            BaseAliveGameObject* pMudOrAbe = FindMudOrAbe_42CFD0();
            if (pMudOrAbe)
            {
                if (pMudOrAbe->field_8_object_id != field_11C_obj_id)
                {
                    pObj = pMudOrAbe;
                    field_11C_obj_id = pMudOrAbe->field_8_object_id;
                }
            }

            field_174_flags.Clear(Flags_174::eBit2);
            if (!WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)))
            {
                return 1;
            }

            Path_Hoist* pHoist = TryGetHoist_42AFD0(0, FALSE);
            if (pHoist)
            {
                mNextMotion = eFleechMotions::M_Idle_3_42E850;
                field_160_hoistX = pHoist->field_8_top_left.field_0_x + (mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(1) ? 6 : 12);
                field_162_hoistY = pHoist->field_8_top_left.field_2_y;
                return 14;
            }
            [[fallthrough]];
        }

        case 5u:
            if (!pObj || pObj->mHealth <= FP_FromInteger(0))
            {
                return 13;
            }

            if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
            {
                return field_126_state;
            }

            if (pObj == sActiveHero && VOnSameYLevel(sActiveHero) && gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos, sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) && !WallHit(FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5)), sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos))
            {
                return 1;
            }

            if (!IsNear_428670(pObj))
            {
                return 2;
            }

            if (field_174_flags.Get(Flags_174::eBit2))
            {
                if (pObj->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(2))
                {
                    return field_126_state;
                }
            }
            else
            {
                if (pObj->mBaseAnimatedWithPhysicsGameObject_XPos >= mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(2))
                {
                    return field_126_state;
                }
            }
            return 1;

        case 6u:
            if (mCurrentMotion != eFleechMotions::M_StopCrawling_7_42EBB0 && mCurrentMotion != eFleechMotions::M_Knockback_6_42EAF0 && mNextMotion != -1)
            {
                return field_126_state;
            }
            mNextMotion = eFleechMotions::M_Crawl_4_42E960;
            return 7;

        case 7u:
        {
            auto v70 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
            if (mCurrentMotion != eFleechMotions::M_Crawl_4_42E960 || (v70 && VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(5), v70)))
            {
                if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850 && mNextMotion != -1)
                {
                    return field_126_state;
                }
                mNextMotion = eFleechMotions::M_Knockback_6_42EAF0;
            }
            else
            {
                mNextMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
            }
            return 9;
        }

        case 8u:
            if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
            {
                return field_126_state;
            }
            mNextMotion = eFleechMotions::M_Crawl_4_42E960;
            return 9;

        case 9u:
            return Brain_ChasingAbe_State_9(pObj);

        case 10u:
            if (pObj)
            {
                if (mCurrentMotion == eFleechMotions::M_ExtendTongueFromEnemy_14_42FBD0)
                {
                    return 11;
                }
                else if (mNextMotion == eFleechMotions::M_ExtendTongueFromEnemy_14_42FBD0)
                {
                    return field_126_state;
                }

                ResetTarget_42CF70();
                return 1;
            }
            sub_42B8C0();
            return 13;

        case 11u:
            if (pObj)
            {
                if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
                {
                    return field_126_state;
                }

                ResetTarget_42CF70();

                if (pObj->mHealth <= FP_FromInteger(0))
                {
                    return 13;
                }

                if (VIsObj_GettingNear(pObj))
                {
                    mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                    return 12;
                }

                return 1;
            }
            sub_42B8C0();
            return 13;

        case 12u:
            if (pObj)
            {
                if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
                {
                    return field_126_state;
                }
                if (!Collision_42B290(1))
                {
                    mNextMotion = eFleechMotions::M_Crawl_4_42E960;
                }
                return 1;
            }
            sub_42B8C0();
            return 13;

        case 13u:
            mNextMotion = eFleechMotions::M_Idle_3_42E850;
            field_124_brain_state = eFleechBrains::eBrain_0_Patrol_430BA0;
            field_13E_current_anger = field_140_max_anger + (field_142_attack_anger_increaser - field_140_max_anger) / 2;
            return 0;

        case 14u:
            if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
            {
                return field_126_state;
            }
            if ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && FP_FromInteger(field_160_hoistX) > mBaseAnimatedWithPhysicsGameObject_XPos) || (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && FP_FromInteger(field_160_hoistX) < mBaseAnimatedWithPhysicsGameObject_XPos))
            {
                mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                return field_126_state;
            }
            mNextMotion = eFleechMotions::M_RaiseHead_11_42F590;
            return 15;

        case 15u:
            if (mCurrentMotion != eFleechMotions::M_SettleOnGround_13_42FB00)
            {
                return field_126_state;
            }
            return 16;

        default:
            return field_126_state;
    }
}

s16 Fleech::Brain_ChasingAbe_State_9(BaseAliveGameObject* pObj)
{
    if (!IsScrabOrParamiteNear(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(6)))
    {
        return 1;
    }

    auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
    if (pDangerObj == sControlledCharacter_5C1B8C)
    {
        if (Collision_42B290(1) || HandleEnemyStopperOrSlamDoor_42ADC0(1) || WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != 0 ? -1 : 1)))
        {
            mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
            field_124_brain_state = eFleechBrains::eBrain_2_Scared_42D310;
            return 0;
        }
    }

    if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
    {
        mNextMotion = eFleechMotions::M_Idle_3_42E850;
        return field_126_state;
    }

    if (IsScrabOrParamiteNear(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(5)))
    {
        auto v82 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
        if (VIsFacingMe(v82))
        {
            mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
        }
        return 8;
    }

    if (!pObj || !IsNear_428670(pObj))
    {
        return 13;
    }

    if (pObj->mBaseAnimatedWithPhysicsGameObject_XPos < mBaseAnimatedWithPhysicsGameObject_XPos)
    {
        if (pDangerObj->mBaseAnimatedWithPhysicsGameObject_XPos > mBaseAnimatedWithPhysicsGameObject_XPos)
        {
            return 1;
        }
    }

    if (pDangerObj->mBaseAnimatedWithPhysicsGameObject_XPos < mBaseAnimatedWithPhysicsGameObject_XPos)
    {
        return 1;
    }

    if (pObj->mBaseAnimatedWithPhysicsGameObject_XPos <= mBaseAnimatedWithPhysicsGameObject_XPos)
    {
        if (pDangerObj->mBaseAnimatedWithPhysicsGameObject_XPos > mBaseAnimatedWithPhysicsGameObject_XPos)
        {
            return 1;
        }
    }

    if (!(Fleech_NextRandom() % 32) && mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
    {
        mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
        return field_126_state;
    }

    if ((Fleech_NextRandom() % 64) || mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
    {
        return field_126_state;
    }
    mCurrentMotion = eFleechMotions::M_PatrolCry_5_42E810;
    return field_126_state;
}

s16 Fleech::Brain_ChasingAbe_State_2(BaseAliveGameObject* pObj)
{
    if (!pObj || pObj->mHealth <= FP_FromInteger(0))
    {
        return 13;
    }

    if (IsNear_428670(pObj))
    {
        return 1;
    }

    if (VIsFacingMe(pObj) || mCurrentMotion == eFleechMotions::M_StopCrawling_7_42EBB0 || mCurrentMotion == eFleechMotions::M_Knockback_6_42EAF0)
    {
        if (mCurrentMotion != eFleechMotions::M_Crawl_4_42E960)
        {
            mNextMotion = eFleechMotions::M_Crawl_4_42E960;
        }

        if (IsScrabOrParamiteNear(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(5)))
        {
            auto v56 = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
            if (VIsFacingMe(v56))
            {
                mNextMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
            }
            Sound_430520(FleechSound::Scared_7);
            return 6;
        }

        if (field_15E_lost_target_timer < field_15C_lost_target_timeout)
        {
            field_15E_lost_target_timer++;
            Path_Hoist* pHoist = TryGetHoist_42AFD0(1, FALSE);
            if (pHoist)
            {
                mNextMotion = eFleechMotions::M_Idle_3_42E850;
                field_160_hoistX = pHoist->field_8_top_left.field_0_x + (mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(1) ? 6 : 12);
                field_162_hoistY = pHoist->field_8_top_left.field_2_y;
                return 14;
            }

            if (Fleech_NextRandom() % 64)
            {
                return field_126_state;
            }

            mCurrentMotion = eFleechMotions::M_PatrolCry_5_42E810;
            return field_126_state;
        }

        field_15E_lost_target_timer = 0;
        field_124_brain_state = eFleechBrains::eBrain_0_Patrol_430BA0;
        field_13E_current_anger = field_142_attack_anger_increaser - 1;
        return 0;
    }
    else if (mCurrentMotion == eFleechMotions::M_Crawl_4_42E960) // TODO: Check v52 was cur motion
    {
        mNextMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
        return field_126_state;
    }
    else
    {
        mNextMotion = eFleechMotions::M_Knockback_6_42EAF0;
        return field_126_state;
    }
}

s16 Fleech::Brain_ChasingAbe_State_0(BaseAliveGameObject* pObj)
{
    if (!pObj)
    {
        field_11C_obj_id = -1;
        BaseAliveGameObject* pMudOrAbe = FindMudOrAbe_42CFD0();
        if (!pMudOrAbe)
        {
            return 13;
        }
        field_11C_obj_id = pMudOrAbe->field_8_object_id;
    }
    field_120_unused = 0;
    field_15E_lost_target_timer = 0;
    mNextMotion = eFleechMotions::M_Crawl_4_42E960;
    Sound_430520(FleechSound::PatrolCry_0);
    return 1;
}

s16 Fleech::Brain_ChasingAbe_State_1(BaseAliveGameObject* pObj)
{
    if (!pObj || pObj->mHealth <= FP_FromInteger(0))
    {
        return 13;
    }

    // Is moving?
    if (mBaseAnimatedWithPhysicsGameObject_VelX != FP_FromInteger(0))
    {
        // Check for blocked by wall
        const FP k1Directed = FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? -1 : 1);
        if (WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * k1Directed))
        {
            mNextMotion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
            field_174_flags.Set(Flags_174::eBit2, mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0));
            return 4;
        }

        // Check for enemy stopper or slam door
        if (HandleEnemyStopperOrSlamDoor_42ADC0(1))
        {
            mNextMotion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
            field_174_flags.Set(Flags_174::eBit2, mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0));

            FP xOffset = mBaseAnimatedWithPhysicsGameObject_XPos;
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOffset -= ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                xOffset += ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            FP slamDoorX = xOffset;
            if (mBaseAnimatedWithPhysicsGameObject_XPos <= xOffset)
            {
                slamDoorX = mBaseAnimatedWithPhysicsGameObject_XPos;
            }

            FP slamDoorW = mBaseAnimatedWithPhysicsGameObject_XPos;
            if (mBaseAnimatedWithPhysicsGameObject_XPos <= xOffset)
            {
                slamDoorW = xOffset;
            }

            Path_TLV* pSlamDoor = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(slamDoorX),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                FP_GetExponent(slamDoorW),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
                TlvTypes::SlamDoor_85);
            return 5 - (pSlamDoor != nullptr);
        }
    }

    // Check for danger object
    if (IsScrabOrParamiteNear(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(5)))
    {
        auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
        if (VIsFacingMe(pDangerObj))
        {
            if (mCurrentMotion == eFleechMotions::M_Crawl_4_42E960)
            {
                mNextMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
                Sound_430520(FleechSound::Scared_7);
                return 6;
            }

            if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
            {
                mNextMotion = eFleechMotions::M_Knockback_6_42EAF0;
            }
        }

        Sound_430520(FleechSound::Scared_7);
        return 6;
    }

    // Check for food object
    BaseAliveGameObject* pAbeOrMud = FindMudOrAbe_42CFD0();
    if (pAbeOrMud)
    {
        if (pAbeOrMud->field_8_object_id != field_11C_obj_id)
        {
            pObj = pAbeOrMud;
            field_11C_obj_id = pAbeOrMud->field_8_object_id;
        }
    }

    // Can we get to them on this level?
    if (VOnSameYLevel(pObj))
    {
        if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), pObj))
        {
            if (pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && VIsFacingMe(pObj) && !WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5), pObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) && GotNoTarget_42CFA0() && gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
            {
                SetTarget_42CF50();
                mNextMotion = eFleechMotions::M_ExtendTongueFromEnemy_14_42FBD0;
                return 10;
            }
        }
    }

    // Can we get to a hanging abe?
    if (pObj == sActiveHero && pObj->mCurrentMotion == eAbeMotions::Motion_67_LedgeHang_454E20 && mBaseAnimatedWithPhysicsGameObject_YPos > pObj->mBaseAnimatedWithPhysicsGameObject_YPos)
    {
        if (mBaseAnimatedWithPhysicsGameObject_YPos - pObj->mBaseAnimatedWithPhysicsGameObject_YPos <= (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(6)))
        {
            if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2), pObj))
            {
                if (pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale
                    && VIsFacingMe(pObj) && !WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5), pObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) && GotNoTarget_42CFA0() && gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
                {
                    SetTarget_42CF50();
                    mNextMotion = eFleechMotions::M_ExtendTongueFromEnemy_14_42FBD0;
                    return 10;
                }
            }
        }
    }

    if (pObj->mBaseAnimatedWithPhysicsGameObject_YPos >= mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 18 : 9)) || pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale != mBaseAnimatedWithPhysicsGameObject_SpriteScale)
    {
        return Brain_ChasingAbe_State1_Helper(pObj);
    }

    // Find hoist in front us?
    Path_Hoist* pHoist = TryGetHoist_42AFD0(1, FALSE);
    if (pHoist)
    {
        mNextMotion = eFleechMotions::M_Idle_3_42E850;
        field_160_hoistX = pHoist->field_8_top_left.field_0_x + (mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 12 : 6);
        field_162_hoistY = pHoist->field_8_top_left.field_2_y;
        return 14;
    }

    // Find host in front or behind us?
    pHoist = TryGetHoist_42AFD0(0, TRUE);
    if (pHoist)
    {
        if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
        {
            // TODO: Check left VS flip is correct
            if ((pHoist->field_12_grab_direction == Path_Hoist::GrabDirection::eFacingLeft && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) && pHoist->field_12_grab_direction != Path_Hoist::GrabDirection::eFacingAnyDirection)
            {
                mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
            }

            mNextMotion = eFleechMotions::M_Idle_3_42E850;
            field_160_hoistX = pHoist->field_8_top_left.field_0_x + (mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 12 : 6);
            field_162_hoistY = pHoist->field_8_top_left.field_2_y;
            return 14;
        }
        else
        {
            mNextMotion = eFleechMotions::M_Idle_3_42E850;
            return field_126_state;
        }
    }

    // Look 12 ahead fora hoist
    s32 k12BlocksCheck = 1;
    do
    {
        pHoist = TryGetHoist_42AFD0(k12BlocksCheck, TRUE);
        if (pHoist)
        {
            mNextMotion = eFleechMotions::M_Crawl_4_42E960;
            field_160_hoistX = pHoist->field_8_top_left.field_0_x + (mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 12 : 6);
            field_162_hoistY = pHoist->field_8_top_left.field_2_y;
            return field_126_state;
        }

        ++k12BlocksCheck;
    }
    while (k12BlocksCheck <= 12);

    // Look 8 behind for a hoist
    s32 k8BlocksCheck = 1;
    while (1)
    {
        pHoist = TryGetHoist_42AFD0(-k8BlocksCheck, 1);
        if (pHoist)
        {
            switch (mCurrentMotion)
            {
                case eFleechMotions::M_Crawl_4_42E960:
                    mNextMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
                    break;

                case eFleechMotions::M_Idle_3_42E850:
                    mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                    break;

                case eFleechMotions::M_Knockback_6_42EAF0:
                case eFleechMotions::M_StopCrawling_7_42EBB0:
                    mNextMotion = eFleechMotions::M_Crawl_4_42E960;
                    break;
            }

            field_160_hoistX = pHoist->field_8_top_left.field_0_x + (mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 12 : 6);
            field_162_hoistY = pHoist->field_8_top_left.field_2_y;
            return field_126_state;
        }

        if (++k8BlocksCheck > 8)
        {
            return Brain_ChasingAbe_State1_Helper(pObj);
        }
    }
}

s16 Fleech::Brain_ChasingAbe_State1_Helper(BaseAliveGameObject* pObj)
{
    if (pObj->mBaseAnimatedWithPhysicsGameObject_YPos < mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 18 : 9)) && pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && IsNear_428670(pObj))
    {
        const FP v45 = mBaseAnimatedWithPhysicsGameObject_XPos - pObj->mBaseAnimatedWithPhysicsGameObject_XPos;
        if (FP_Abs(v45) < ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromDouble(0.5))
        {
            return 13;
        }
    }

    if (!VIsFacingMe(pObj) && mCurrentMotion == eFleechMotions::M_Crawl_4_42E960)
    {
        mNextMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
    }

    if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
    {
        if (VIsFacingMe(pObj))
        {
            const FP v48 = FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? -1 : 1);
            if (WallHit(
                    FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5),
                    ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * v48))
            {
                field_174_flags.Set(Flags_174::eBit2, mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX));
                return 4;
            }
            mNextMotion = eFleechMotions::M_Crawl_4_42E960;
        }
        else
        {
            mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
        }
    }

    if (mCurrentMotion == eFleechMotions::M_StopCrawling_7_42EBB0)
    {
        mNextMotion = eFleechMotions::M_Crawl_4_42E960;
    }

    if (mCurrentMotion == eFleechMotions::M_Fall_9_42ECD0)
    {
        return 3;
    }

    if (IsNear_428670(pObj))
    {
        return field_126_state;
    }

    if (!VIsFacingMe(pObj))
    {
        if (mCurrentMotion != eFleechMotions::M_StopCrawling_7_42EBB0 && mCurrentMotion != eFleechMotions::M_Knockback_6_42EAF0)
        {
            if (mCurrentMotion == eFleechMotions::M_Crawl_4_42E960)
            {
                mNextMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
                return 2;
            }
            mNextMotion = eFleechMotions::M_Knockback_6_42EAF0;
        }
    }
    return 2;
}

const s8 byte_5518B0[16] = {
    6,
    6,
    6,
    6,
    6,
    5,
    5,
    5,
    5,
    6,
    6,
    6,
    6,
    0,
    0,
    0};

s16 Fleech::Brain_2_Scared_42D310()
{
    auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
    if (pDangerObj && pDangerObj->mHealth > FP_FromInteger(0))
    {
        // Danger target is dead, check if there is another one who is still alive.
        IsScrabOrParamiteNear(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(8));
    }

    pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_170_danger_obj));
    if (pDangerObj)
    {
        if (pDangerObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            field_170_danger_obj = -1;
            pDangerObj = 0;
        }
    }

    if (gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        MusicController::static_PlayMusic(static_cast<MusicController::MusicTypes>(byte_5518B0[field_126_state]), this, 0, 0);
    }
    else
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (field_126_state)
    {
        case 0u:
            if (!pDangerObj || pDangerObj != sControlledCharacter_5C1B8C)
            {
                return 9;
            }

            if (!VIsFacingMe(pDangerObj))
            {
                mNextMotion = eFleechMotions::M_Crawl_4_42E960;
                Sound_430520(FleechSound::Scared_7);
                return 1;
            }

            if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
            {
                mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                mNextMotion = eFleechMotions::M_Crawl_4_42E960;
                Sound_430520(FleechSound::Scared_7);
                return 1;
            }

            if (mCurrentMotion != eFleechMotions::M_Crawl_4_42E960)
            {
                Sound_430520(FleechSound::Scared_7);
                return 1;
            }

            mNextMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
            Sound_430520(FleechSound::Scared_7);
            return 1;

        case 1u:
        {
            if (!pDangerObj || pDangerObj->mHealth <= FP_FromInteger(0) || pDangerObj != sControlledCharacter_5C1B8C)
            {
                return 9;
            }

            if (mBaseAnimatedWithPhysicsGameObject_VelX != FP_FromInteger(0))
            {
                const FP v9 = FP_FromInteger((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? -1 : 1);
                if (WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(0) ? 10 : 5), ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * v9))
                {
                    mNextMotion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
                    field_174_flags.Set(Flags_174::eBit2, mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0));
                    return 3;
                }

                if (mBaseAnimatedWithPhysicsGameObject_VelX != FP_FromInteger(0) && (Collision_42B290(1) || HandleEnemyStopperOrSlamDoor_42ADC0(1)))
                {
                    mNextMotion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
                    field_174_flags.Set(Flags_174::eBit2, mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0));
                    return 4;
                }
            }

            Path_Hoist* pHoist = TryGetHoist_42AFD0(1, 0);
            if (pHoist)
            {
                mNextMotion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
                field_160_hoistX = pHoist->field_8_top_left.field_0_x + 12;
                field_162_hoistY = pHoist->field_8_top_left.field_2_y;
                return 10;
            }

            if (VIsFacingMe(pDangerObj))
            {
                if (mCurrentMotion == eFleechMotions::M_Crawl_4_42E960)
                {
                    mNextMotion = eFleechMotions::M_StopCrawling_7_42EBB0;
                }
            }

            if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
            {
                if (VIsFacingMe(pDangerObj))
                {
                    mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                }
                else
                {
                    const FP k1Directed = FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != 0 ? -1 : 1);
                    if (WallHit(
                            FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5),
                            ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * k1Directed))
                    {
                        field_174_flags.Set(Flags_174::eBit2, mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX));
                        return 4;
                    }
                    mNextMotion = eFleechMotions::M_Crawl_4_42E960;
                }
            }

            if (mCurrentMotion != eFleechMotions::M_Fall_9_42ECD0)
            {
                return field_126_state;
            }
            return 2;
        }

        case 2u:
            if (mCurrentMotion == eFleechMotions::M_Idle_3_42E850)
            {
                mCurrentMotion = eFleechMotions::M_Crawl_4_42E960;
                return 1;
            }
            return field_126_state;

        case 3u:
        {
            const FP v22 = FP_FromInteger((field_174_flags.Get(Flags_174::eBit2)) != 0 ? -1 : 1);
            if (!WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * v22))
            {
                return 1;
            }

            Path_Hoist* pHoist = TryGetHoist_42AFD0(0, 0);
            if (pHoist)
            {
                if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
                {
                    mNextMotion = eFleechMotions::M_Idle_3_42E850;
                }

                field_160_hoistX = pHoist->field_8_top_left.field_0_x + (mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 12 : 6);
                field_162_hoistY = pHoist->field_8_top_left.field_2_y;
                return 10;
            }
            [[fallthrough]];
        }

        case 4u:
        {
            if (!pDangerObj || pDangerObj != sControlledCharacter_5C1B8C)
            {
                return 9;
            }

            if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
            {
                return field_126_state;
            }

            if (field_174_flags.Get(Flags_174::eBit2))
            {
                if (pDangerObj->mBaseAnimatedWithPhysicsGameObject_XPos < mBaseAnimatedWithPhysicsGameObject_XPos)
                {
                    return 1;
                }
            }
            else if (pDangerObj->mBaseAnimatedWithPhysicsGameObject_XPos > mBaseAnimatedWithPhysicsGameObject_XPos)
            {
                return 1;
            }

            if (VOnSameYLevel(pDangerObj) && VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), pDangerObj) && pDangerObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && VIsFacingMe(pDangerObj) && !WallHit(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5), pDangerObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) && GotNoTarget_42CFA0())
            {
                field_11C_obj_id = field_170_danger_obj;
                return 5;
            }
            else
            {
                if ((Fleech_NextRandom() % 32) || mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
                {
                    return field_126_state;
                }
                mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                return field_126_state;
            }
            break;
        }

        case 5u:
            if (!pDangerObj || pDangerObj != sControlledCharacter_5C1B8C)
            {
                return 9;
            }

            if (VIsFacingMe(pDangerObj) || mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
            {
                if (!VOnSameYLevel(pDangerObj))
                {
                    return field_126_state;
                }

                if (!VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), pDangerObj))
                {
                    return field_126_state;
                }

                if (pDangerObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale != mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                {
                    return field_126_state;
                }

                if (!VIsFacingMe(pDangerObj))
                {
                    return field_126_state;
                }

                if (WallHit(
                        FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_SpriteScale >= FP_FromInteger(1) ? 10 : 5),
                        pDangerObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos)
                    || !GotNoTarget_42CFA0())
                {
                    return field_126_state;
                }

                SetTarget_42CF50();
                mNextMotion = eFleechMotions::M_ExtendTongueFromEnemy_14_42FBD0;
                return 6;
            }
            else
            {
                mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                return field_126_state;
            }
            break;

        case 6u:
            if (pDangerObj)
            {
                if (mCurrentMotion == eFleechMotions::M_ExtendTongueFromEnemy_14_42FBD0)
                {
                    return 7;
                }
                else
                {
                    if (mNextMotion == eFleechMotions::M_ExtendTongueFromEnemy_14_42FBD0)
                    {
                        return field_126_state;
                    }
                    ResetTarget_42CF70();
                    return 1;
                }
            }
            else
            {
                sub_42B8C0();
                return 9;
            }
            break;

        case 7u:
            if (pDangerObj)
            {
                if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
                {
                    return field_126_state;
                }

                ResetTarget_42CF70();

                if (pDangerObj->mHealth <= FP_FromInteger(0))
                {
                    return 9;
                }
                mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                return 8;
            }
            else
            {
                sub_42B8C0();
                return 9;
            }
            break;

        case 8u:
            if (pDangerObj)
            {
                if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
                {
                    return field_126_state;
                }
                return 5;
            }
            else
            {
                sub_42B8C0();
                return 9;
            }
            break;

        case 9u:
            mNextMotion = eFleechMotions::M_Idle_3_42E850;
            field_170_danger_obj = -1;
            field_11C_obj_id = -1;
            field_124_brain_state = eFleechBrains::eBrain_0_Patrol_430BA0;
            return 0;

        case 10u:
            if (mCurrentMotion != eFleechMotions::M_Idle_3_42E850)
            {
                return field_126_state;
            }
            if ((!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) || FP_FromInteger(field_160_hoistX) <= mBaseAnimatedWithPhysicsGameObject_XPos) && (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) || FP_FromInteger(field_160_hoistX) >= mBaseAnimatedWithPhysicsGameObject_XPos))
            {
                mNextMotion = eFleechMotions::M_RaiseHead_11_42F590;
                return 11;
            }
            else
            {
                mCurrentMotion = eFleechMotions::M_Knockback_6_42EAF0;
                return field_126_state;
            }
            break;

        case 11u:
            if (mCurrentMotion != eFleechMotions::M_SettleOnGround_13_42FB00)
            {
                return field_126_state;
            }
            return 9;

        default:
            return field_126_state;
    }
}

s16 Fleech::Brain_3_Death_42D1E0()
{
    field_11C_obj_id = -1;
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (field_12C < static_cast<s32>(sGnFrame + 80))
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale -= FP_FromDouble(0.022);
        mBaseAnimatedWithPhysicsGameObject_Red -= 2;
        mBaseAnimatedWithPhysicsGameObject_Green -= 2;
        mBaseAnimatedWithPhysicsGameObject_Blue -= 2;
    }

    if (static_cast<s32>(sGnFrame) < field_12C - 24)
    {
        DeathSmokeEffect(false);
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 100;
}
