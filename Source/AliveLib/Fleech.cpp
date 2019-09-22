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
#include "Gibs.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "PsxDisplay.hpp"

ALIVE_VAR(1, 0x5BC20C, BYTE, sFleechRandomIdx_5BC20C, 0);
ALIVE_VAR(1, 0x5BC20E, short, sFleechCount_5BC20E, 0);

const TFleechMotionFn sFleech_motion_table_551798[19] =
{
    &Fleech::M_Sleeping_0_42F0B0,
    &Fleech::M_WakingUp_1_42F270,
    &Fleech::M_Unknown_2_42F2F0,
    &Fleech::M_Idle_3_42E850,
    &Fleech::M_Crawl_4_42E960,
    &Fleech::M_PatrolCry_5_42E810,
    &Fleech::M_Knockback_6_42EAF0,
    &Fleech::M_StopCrawling_7_42EBB0, //Stop due to being aligned vertically with target
    &Fleech::M_StopMidCrawlCycle_8_42EB20, //Stop mid-crawl cycle
    &Fleech::M_Fall_9_42ECD0,
    &Fleech::M_Land_10_42F330,
    &Fleech::M_RaiseHead_11_42F590,
    &Fleech::M_Climb_12_42F7F0,
    &Fleech::M_SettleOnGround_13_42FB00,
    &Fleech::M_ExtendTongueFromEnemy_14_42FBD0,
    &Fleech::M_RetractTongueFromEnemey_15_42FC40,
    &Fleech::M_DeathByFalling_16_42FCE0,
    &Fleech::M_SleepingWithTongue_17_42F370,
    &Fleech::M_Consume_18_42FDF0
};

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

const TFleechAIFn sFleech_ai_table_551830[4] =
{
    &Fleech::AI_Patrol_0_430BA0,
    &Fleech::AI_ChasingAbe_1_428760,
    &Fleech::AI_Scared_2_42D310,
    &Fleech::AI_Death_3_42D1E0
};

enum eFleechBrains
{
    eAI_Patrol_0_430BA0 = 0,
    eAI_ChasingAbe_1_428760 = 1,
    eAI_Scared_2_42D310 = 2,
    eAI_Death_3_42D1E0 = 3,
};

const static AIFunctionData<TFleechAIFn> sFleechAiTable[4] =
{
    { &Fleech::AI_Patrol_0_430BA0, 0x430BA0, "AI_Patrol_0" }, // no stub ??
    { &Fleech::AI_ChasingAbe_1_428760, 0x428760, "AI_ChasingAbe_1" }, // no stub ??
    { &Fleech::AI_Scared_2_42D310, 0x42D310, "AI_Scared_2" }, // no stub ??
    { &Fleech::AI_Death_3_42D1E0, 0x42D1E0, "AI_Death_3" }, // no stub ??
};

static BYTE Fleech_NextRandom()
{
    return sRandomBytes_546744[sFleechRandomIdx_5BC20C++];
}

Fleech* Fleech::ctor_429DC0(Path_Fleech* pTlv, int tlvInfo)
{
    ctor_408240(2);
    SetVTable(this, 0x544F28);
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_C_objectId = tlvInfo;

    if (pTlv->field_10_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
    }

    Init_42A170();

    field_118_tlvInfo = tlvInfo;
    field_124_brain_state = eFleechBrains::eAI_Patrol_0_430BA0;
    field_11C_obj_id = -1;
    field_170_danger_obj = -1;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_direction == 0);

    field_174_flags.Set(Flags_174::eBit5_asleep, pTlv->field_14_asleep & 1);
    field_174_flags.Set(Flags_174::eBit6_goes_to_sleep, pTlv->field_24_goes_to_sleep & 1);
    field_174_flags.Set(Flags_174::eBit7_persistant, pTlv->field_2C_persistant & 1);

    field_140 = 2;
    field_158 = 10;

    field_142_attack_anger = pTlv->field_1A_attack_anger + 2;
    field_144_wake_up_id = pTlv->field_1E_wake_up_id;
    field_146_tlv_28 = pTlv->field_28_unused;
    field_148_wake_up_switch_value = SwitchStates_Get_466020(pTlv->field_1E_wake_up_id) & 0xFFFF;
    field_14A_allow_wake_up_id = pTlv->field_2A_allow_wake_up_id;
    field_150_patrol_range = FP_GetExponent(FP_FromInteger(pTlv->field_26_patrol_range_in_grids) * ScaleToGridSize_4498B0(field_CC_sprite_scale));
    field_15C_lost_target_timeout = pTlv->field_22_lost_target_timeout;

    InitTonguePolys_42B6E0();

    field_13E_anger = 0;

    if (pTlv->field_20_hanging)
    {
        field_160_hoistX = (pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x) / 2;
        field_166_angle = Fleech_NextRandom();
        field_BC_ypos -= FP_FromInteger(pTlv->field_8_top_left.field_2_y - pTlv->field_C_bottom_right.field_2_y);
        sub_42B9A0((pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x) / 2, pTlv->field_8_top_left.field_2_y);
        field_106_current_motion = eFleechMotions::M_SleepingWithTongue_17_42F370;
        SetAnim_429D80();
    }
    else if (field_174_flags.Get(Flags_174::eBit5_asleep))
    {
        field_106_current_motion = eFleechMotions::M_Sleeping_0_42F0B0;
        SetAnim_429D80();
    }
    else
    {
        field_106_current_motion = eFleechMotions::M_Idle_3_42E850;
        field_13E_anger = field_140 + (field_142_attack_anger - field_140) / 2;
    }

    return this;
}

BaseGameObject* Fleech::VDestructor(signed int flags)
{
    return vdtor_42A140(flags);
}

void Fleech::VUpdate()
{
    vUpdate_42AB20();
}

void Fleech::VRender(int** pOt)
{
    vRender_42A550(pOt);
}

void Fleech::VScreenChanged()
{
    vScreenChanged_42A4C0();
}

void Fleech::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    vOn_Tlv_Collision_42AAB0(pTlv);
}

__int16 Fleech::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_42A5C0(pFrom);
}

void Fleech::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_42E5C0();
}

void Fleech::VOnThrowableHit(BaseGameObject* pFrom)
{
    vOnThrowableHit_42A590(pFrom);
}

int Fleech::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_42FF80(reinterpret_cast<Fleech_State*>(pSaveBuffer));
}


const static int sFleechFrameTableOffsets_5517E4[19] =
{
    37808,
    37884,
    37896,
    37704,
    37748,
    37784,
    37924,
    37936,
    37960,
    37984,
    38060,
    38112,
    38156,
    38208,
    38260,
    37848,
    38248,
    38396,
    38276
};

ALIVE_VAR(1, 0x551840, int, dword_551840, -1);

int CC Fleech::CreateFromSaveState_42DD50(const BYTE* pBuffer)
{
    auto pState = reinterpret_cast<const Fleech_State*>(pBuffer);

    auto pTlv = static_cast<Path_Fleech*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_40_tlvInfo));
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kFleechResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("FLEECH.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 580, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("FLEEBLOW.BAN", nullptr);
    }

    auto pFleech = alive_new<Fleech>();
    pFleech->ctor_429DC0(pTlv, pState->field_40_tlvInfo);
    pFleech->field_C_objectId = pState->field_4_obj_id;

    pFleech->field_FC_pPathTLV = nullptr;
    pFleech->field_100_pCollisionLine = nullptr;

    pFleech->field_110_id = pState->field_3C_id;

    pFleech->field_B8_xpos = pState->field_8_xpos;
    pFleech->field_BC_ypos = pState->field_C_ypos;
    pFleech->field_C4_velx = pState->field_10_velx;
    pFleech->field_C8_vely = pState->field_14_vely;

    pFleech->field_C0_path_number = pState->field_18_path_number;
    pFleech->field_C2_lvl_number = pState->field_1A_lvl_number;
    pFleech->field_CC_sprite_scale = pState->field_1C_sprite_scale;

    pFleech->field_D0_r = pState->field_20_r;
    pFleech->field_D2_g = pState->field_22_g;
    pFleech->field_D4_b = pState->field_24_b;

    pFleech->field_106_current_motion = pState->field_28_current_motion;
    BYTE** ppRes = pFleech->ResBlockForMotion_42A530(pState->field_28_current_motion);
    pFleech->field_20_animation.Set_Animation_Data_409C80(sFleechFrameTableOffsets_5517E4[pFleech->field_106_current_motion], ppRes);
    pFleech->field_20_animation.field_92_current_frame = pState->field_2A_anim_current_frame;
    pFleech->field_20_animation.field_E_frame_change_counter = pState->field_2C_frame_change_counter;

    pFleech->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_26_bFlipX & 1);
    pFleech->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2E_bRender & 1);

    pFleech->field_6_flags.Set(BaseGameObject::eDrawable, pState->field_2F_bDrawable & 1);

    if (IsLastFrame(&pFleech->field_20_animation))
    {
        pFleech->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }
    pFleech->field_10C_health = pState->field_30_health;
    pFleech->field_106_current_motion = pState->field_34_current_motion;
    pFleech->field_108_next_motion = pState->field_36_next_motion;

    pFleech->field_F8_LastLineYPos = FP_FromInteger(pState->field_38_lastLineYPos);

    pFleech->field_114_flags.Set(Flags_114::e114_Bit9);

    pFleech->field_104_collision_line_type = pState->field_3A_line_type;
    pFleech->field_118_tlvInfo = pState->field_40_tlvInfo;
    pFleech->field_11C_obj_id = pState->field_44_obj_id;
    pFleech->field_120 = pState->field_48;
    pFleech->field_178_tongue_state = pState->field_4A;
    pFleech->field_17A_tongue_sub_state = pState->field_4C;
    pFleech->field_17C = pState->field_4E;
    pFleech->field_17E = pState->field_50;
    pFleech->field_180_tongue_x = pState->field_52;
    pFleech->field_182_tongue_y = pState->field_54;
    pFleech->field_184 = pState->field_56;
    pFleech->field_186 = pState->field_58;
    pFleech->field_188 = pState->field_5A;

    pFleech->field_18A.Set(Flags_18A::e18A_TongueActive_Bit1, pState->field_5C & 1);
    pFleech->field_18A.Set(Flags_18A::e18A_Bit2, pState->field_5D & 1);

    pFleech->field_124_brain_state = pState->field_5E_brain_state;
    pFleech->field_126_state = pState->field_60_state;
    pFleech->field_12C = pState->field_64;
    pFleech->field_128 = pState->field_62;
    sFleechRandomIdx_5BC20C = pState->field_68_fleech_random_idx;
    pFleech->field_130 = pState->field_6A;
    pFleech->field_134 = pState->field_6C;
    pFleech->field_138 = pState->field_70;
    pFleech->field_13C = pState->field_74;
    pFleech->field_13E_anger = pState->field_76_anger;
    pFleech->field_140 = pState->field_78;
    pFleech->field_142_attack_anger = pState->field_7A_attack_anger;
    pFleech->field_144_wake_up_id = pState->field_7C_wakeup_id;
    pFleech->field_146_tlv_28 = pState->field_7E_tlv;
    pFleech->field_148_wake_up_switch_value = pState->field_80_wake_up_switch_value;
    pFleech->field_14A_allow_wake_up_id = pState->field_82_allow_wake_up_id;
    pFleech->field_14C = pState->field_84;
    pFleech->field_14E = pState->field_86;
    pFleech->field_150_patrol_range = pState->field_88_patrol_range;
    pFleech->field_152 = pState->field_8A;
    pFleech->field_154 = pState->field_8C;
    pFleech->field_156 = pState->field_8E;
    pFleech->field_158 = pState->field_90;
    pFleech->field_15A = pState->field_92;
    pFleech->field_15C_lost_target_timeout = pState->field_94_lost_target_timeout;
    pFleech->field_15E = pState->field_96;
    pFleech->field_160_hoistX = pState->field_98;
    pFleech->field_162_hoistY = pState->field_9A;
    pFleech->field_164 = pState->field_9C;
    pFleech->field_166_angle = pState->field_9E_angle;
    pFleech->field_168 = pState->field_A0;
    pFleech->field_16C = pState->field_A4;
    pFleech->field_170_danger_obj = pState->field_A8;

    if (pState->field_4_obj_id == pState->field_AC_obj_id)
    {
        dword_551840 = pFleech->field_8_object_id;
    }

    pFleech->field_174_flags.Set(Flags_174::eBit1_bHoistDone, pState->field_B0.Get(Fleech_State::eBit1));
    pFleech->field_174_flags.Set(Flags_174::eBit2, pState->field_B0.Get(Fleech_State::eBit2));
    pFleech->field_174_flags.Set(Flags_174::eBit3, pState->field_B0.Get(Fleech_State::eBit3));
    pFleech->field_174_flags.Set(Flags_174::eBit4, pState->field_B0.Get(Fleech_State::eBit4));
    pFleech->field_174_flags.Set(Flags_174::eBit5_asleep, pState->field_B0.Get(Fleech_State::eBit5));
    pFleech->field_174_flags.Set(Flags_174::eBit6_goes_to_sleep, pState->field_B0.Get(Fleech_State::eBit6));
    pFleech->field_174_flags.Set(Flags_174::eBit7_persistant, pState->field_B0.Get(Fleech_State::eBit7));

    return sizeof(Fleech_State);
}

int Fleech::vGetSaveState_42FF80(Fleech_State* pState)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    pState->field_0_type = Types::eFleech_50;
    pState->field_4_obj_id = field_C_objectId;
    pState->field_8_xpos = field_B8_xpos;
    pState->field_C_ypos = field_BC_ypos;
    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;
    pState->field_70 = field_138;
    pState->field_18_path_number = field_C0_path_number;
    pState->field_1A_lvl_number = field_C2_lvl_number;
    pState->field_1C_sprite_scale = field_CC_sprite_scale;
    pState->field_20_r = field_D0_r;
    pState->field_22_g = field_D2_g;
    pState->field_24_b = field_D4_b;
    pState->field_26_bFlipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_28_current_motion = field_106_current_motion;
    pState->field_2A_anim_current_frame = field_20_animation.field_92_current_frame;
    pState->field_2C_frame_change_counter = field_20_animation.field_E_frame_change_counter;
    pState->field_2F_bDrawable = field_6_flags.Get(BaseGameObject::eDrawable);
    pState->field_2E_bRender = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pState->field_30_health = field_10C_health;
    pState->field_34_current_motion = field_106_current_motion;
    pState->field_36_next_motion = field_108_next_motion;
    pState->field_38_lastLineYPos = FP_GetExponent(field_F8_LastLineYPos);

    if (field_100_pCollisionLine)
    {
        pState->field_3A_line_type = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pState->field_3A_line_type = -1;
    }

    if (field_110_id != -1)
    {
        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_110_id);
        if (pObj)
        {
            pState->field_3C_id = pObj->field_C_objectId;
        }
    }
    else
    {
        pState->field_3C_id = -1;
    }

    if (field_11C_obj_id != -1)
    {
        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_11C_obj_id);
        if (pObj)
        {
            pState->field_44_obj_id = pObj->field_C_objectId;
        }
    }
    else
    {
        pState->field_44_obj_id = -1;
    }

    pState->field_40_tlvInfo = field_118_tlvInfo;
    pState->field_48 = field_120;
    pState->field_4A = field_178_tongue_state;
    pState->field_4C = field_17A_tongue_sub_state;
    pState->field_4E = field_17C;
    pState->field_50 = field_17E;
    pState->field_52 = field_180_tongue_x;
    pState->field_54 = field_182_tongue_y;
    pState->field_56 = field_184;
    pState->field_58 = field_186;
    pState->field_5A = field_188;
    pState->field_5C = field_18A.Get(Flags_18A::e18A_TongueActive_Bit1);
    pState->field_5D = field_18A.Get(Flags_18A::e18A_Bit2);
    pState->field_5E_brain_state = field_124_brain_state;
    pState->field_60_state = field_126_state;
    pState->field_64 = field_12C - sGnFrame_5C1B84;
    pState->field_62 = field_128;
    pState->field_68_fleech_random_idx = sFleechRandomIdx_5BC20C;
    pState->field_6A = field_130;
    pState->field_6C = field_134;
    pState->field_70 = field_138;
    pState->field_74 = field_13C;
    pState->field_76_anger = field_13E_anger;
    pState->field_78 = field_140;
    pState->field_7A_attack_anger = field_142_attack_anger;
    pState->field_7C_wakeup_id = field_144_wake_up_id;
    pState->field_7E_tlv = field_146_tlv_28;
    pState->field_80_wake_up_switch_value = field_148_wake_up_switch_value;
    pState->field_82_allow_wake_up_id = field_14A_allow_wake_up_id;
    pState->field_84 = field_14C;
    pState->field_86 = field_14E;
    pState->field_88_patrol_range = field_150_patrol_range;
    pState->field_8A = field_152;
    pState->field_8C = field_154;
    pState->field_8E = field_156;
    pState->field_90 = field_158;
    pState->field_92 = field_15A;
    pState->field_94_lost_target_timeout = field_15C_lost_target_timeout;
    pState->field_96 = field_15E;
    pState->field_98 = field_160_hoistX;
    pState->field_9A = field_162_hoistY;
    pState->field_9C = field_164;
    pState->field_9E_angle = field_166_angle;
    pState->field_A0 = field_168;
    pState->field_A4 = field_16C;

    if (field_170_danger_obj != -1)
    {
        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_170_danger_obj);
        if (pObj)
        {
            pState->field_A8 = pObj->field_C_objectId;
        }
    }
    else
    {
        pState->field_A8 = -1;
    }

    if (dword_551840 == field_8_object_id)
    {
        pState->field_AC_obj_id = field_C_objectId;
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
    if (field_108_next_motion == -1)
    {
        if (field_20_animation.field_92_current_frame || sGnFrame_5C1B84 & 3)
        {
            if (field_20_animation.field_92_current_frame == 4 && !(sGnFrame_5C1B84 & 3))
            {
                Sound_430520(4);

                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                {
                    auto pSnoozeParticle = alive_new<SnoozeParticle>();
                    if (pSnoozeParticle)
                    {
                        const FP yPos = (field_CC_sprite_scale * FP_FromInteger(-20)) + field_BC_ypos;
                        FP xOff = {};
                        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            xOff = -(field_CC_sprite_scale * FP_FromInteger(10));
                        }
                        else
                        {
                            xOff = (field_CC_sprite_scale * FP_FromInteger(10));
                        }
                        pSnoozeParticle->ctor_4B06F0(xOff + field_B8_xpos, yPos, field_20_animation.field_C_render_layer, field_20_animation.field_14_scale);
                    }
                }
            }
        }
        else
        {
            Sound_430520(3);
        }
    }
    else
    {
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
    }
}

void Fleech::M_WakingUp_1_42F270()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eSnoozParticle_124)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1E4_state = SnoozeParticle::SnoozeParticleState::BlowingUp_2;
        }
    }

    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
            Sound_430520(6);
        }
    }
}

void Fleech::M_Unknown_2_42F2F0()
{
    if (field_108_next_motion != -1)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }
}

void Fleech::M_Idle_3_42E850()
{
    if (!CanMove_42E3E0())
    {
        if (field_108_next_motion == -1)
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            if (field_106_current_motion == eFleechMotions::M_Idle_3_42E850 &&
                field_20_animation.field_92_current_frame == 0 && 
                !sCollisions_DArray_5C1128->Raycast_417A60(
                    field_B8_xpos - FP_FromInteger(5),
                    field_BC_ypos - FP_FromInteger(5),
                    field_B8_xpos + FP_FromInteger(5),
                    field_BC_ypos + FP_FromInteger(1),
                    &pLine,
                    &hitX,
                    &hitY,
                    field_D6_scale != 0 ? 1 : 16))
            {
                field_138 = FP_FromInteger(0);
                field_F8_LastLineYPos = field_BC_ypos;
                field_106_current_motion = eFleechMotions::M_Fall_9_42ECD0;
            }
        }
        else
        {
            field_106_current_motion = field_108_next_motion;
            field_108_next_motion = -1;
        }
    }

}

const int sFleechCrawlVelX_544FA0[10] =
{
    251423,
    220780,
    307251,
    326921,
    168913,
    217015,
    146093,
    2097152000,
    0,
    0
};

void Fleech::M_Crawl_4_42E960()
{
    if (field_20_animation.field_92_current_frame > 6)
    {
        field_20_animation.field_92_current_frame = 0;
    }

    FP velXTable = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velXTable = -FP_FromRaw(sFleechCrawlVelX_544FA0[field_20_animation.field_92_current_frame]);
    }
    else
    {
        velXTable = FP_FromRaw(sFleechCrawlVelX_544FA0[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * velXTable);

    const FP k1Directed = FP_FromInteger((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) != 0) ? -1 : 1);
    if (WallHit_408750(FP_FromInteger((field_CC_sprite_scale >= FP_FromInteger(1)) ? 10 : 5), ScaleToGridSize_4498B0(field_CC_sprite_scale) * k1Directed))
    {
        ToIdle_42E520();
    }
    else
    {
        MoveAlongFloor_42E600();
        if (field_106_current_motion == eFleechMotions::M_Crawl_4_42E960)
        {
            if (field_20_animation.field_92_current_frame == 4)
            {
                Sound_430520(14u);
            }
            else if (field_20_animation.field_92_current_frame == 6)
            {
                if (field_130 == 0)
                {
                    field_130 = 1;
                    MapFollowMe_408D10(TRUE);
                }

                if (field_108_next_motion == eFleechMotions::M_Idle_3_42E850)
                {
                    field_106_current_motion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
                    field_108_next_motion = -1;
                }
                else if (field_108_next_motion != -1)
                {
                    field_106_current_motion = field_108_next_motion;
                    field_108_next_motion = -1;
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
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sound_430520(0);
        field_13C = 1;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

void Fleech::M_Knockback_6_42EAF0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToIdle_42E520();
    }
}

void Fleech::M_StopCrawling_7_42EBB0()
{
    const FP k1Directed = FP_FromInteger(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) != 0 ? -1 : 1);
    if (WallHit_408750(FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? 10 : 5), ScaleToGridSize_4498B0(field_CC_sprite_scale) * k1Directed))
    {
        ToIdle_42E520();
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_408D10(TRUE);
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(7));
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }
        else
        {
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(7));
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }
        field_106_current_motion = eFleechMotions::M_Crawl_4_42E960;
    }
}

void Fleech::M_StopMidCrawlCycle_8_42EB20()
{
    const FP k1Directed = FP_FromInteger(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ?  -1 : 1);
    if (WallHit_408750(
        FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? 10 : 5),
        ScaleToGridSize_4498B0(field_CC_sprite_scale) * k1Directed) ||
        field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

void Fleech::M_Fall_9_42ECD0()
{
    field_C8_vely += field_CC_sprite_scale *  FP_FromDouble(1.8);

    if (field_C8_vely > (field_CC_sprite_scale * FP_FromInteger(20)))
    {
        field_C8_vely = (field_CC_sprite_scale * FP_FromInteger(20));
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (field_C4_velx > FP_FromDouble(3.6))
        {
            field_C4_velx = FP_FromDouble(3.6);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        if (field_C4_velx < FP_FromDouble(-3.6))
        {
            field_C4_velx = FP_FromDouble(-3.6);
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx -= field_CC_sprite_scale * field_138;
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx += field_CC_sprite_scale * field_138;
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    const FP xpos = field_B8_xpos;
    const FP ypos = field_BC_ypos - FP_FromInteger((field_CC_sprite_scale >= FP_FromInteger(1) ? 10 : 5));
    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        xpos,
        ypos,
        field_B8_xpos,
        field_BC_ypos,
        &pLine,
        &hitX,
        &hitY,
        field_D6_scale == 1 ? 0x0F : 0xF0))
    {
        switch (pLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            field_100_pCollisionLine = pLine;
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            MapFollowMe_408D10(TRUE);
            if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);
                vOnCollisionWith_424EE0(
                    { bRect.x, static_cast<short>(bRect.y + 5) },
                    { bRect.w, static_cast<short>(FP_GetExponent(field_BC_ypos) + 5) },
                    ObjList_5C1B78,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
            if (field_BC_ypos - field_F8_LastLineYPos <= FP_FromInteger((field_CC_sprite_scale >= FP_FromInteger(1) ? 20 : 10)) * FP_FromInteger(15))
            {
                field_106_current_motion = eFleechMotions::M_Land_10_42F330;
            }
            else
            {
                field_106_current_motion = eFleechMotions::M_DeathByFalling_16_42FCE0;
            }
            break;

        case 1u:
        case 2u:
        case 5u:
        case 6u:
            field_B8_xpos = hitX - field_C4_velx;
            field_BC_ypos = hitY;
            MapFollowMe_408D10(TRUE);
            field_C4_velx = FP_FromInteger(0);
            break;

        default:
            return;
        }
    }
}

void Fleech::M_Land_10_42F330()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sound_430520(9);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

void Fleech::M_RaiseHead_11_42F590()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_174_flags.Clear(Flags_174::eBit1_bHoistDone);
        field_106_current_motion = eFleechMotions::M_Climb_12_42F7F0;

        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(-1);

        const short yOff = field_CC_sprite_scale >= FP_FromInteger(1) ? 0 : -10;
        auto pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            field_160_hoistX,
            FP_GetExponent(field_BC_ypos - FP_FromInteger((yOff + 20))),
            field_160_hoistX,
            FP_GetExponent(field_BC_ypos - FP_FromInteger((yOff + 20))),
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
        field_F8_LastLineYPos = field_BC_ypos;
        field_168 = field_BC_ypos - FP_FromInteger(field_162_hoistY);
        field_100_pCollisionLine = nullptr;
        field_16C = FP_Abs(field_B8_xpos - FP_FromInteger(field_160_hoistX));

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
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
    else if (field_20_animation.field_92_current_frame < 4)
    {
        field_B8_xpos += FP_FromInteger((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) != 0 ? 1 : -1);
    }
}

void Fleech::M_Climb_12_42F7F0()
{
    if (!TongueActive_42B8A0())
    {
        if (field_174_flags.Get(Flags_174::eBit1_bHoistDone))
        {
            field_106_current_motion = eFleechMotions::M_SettleOnGround_13_42FB00;
            return;
        }

        const FP velYOff = FP_FromInteger(field_164) * FP_FromDouble(0.045);
        field_C8_vely -= field_CC_sprite_scale * (FP_FromDouble(0.7) - velYOff);
        if (field_164 > 0)
        {
            field_164--;
        }

        const FP xOff = (Math_Cosine_496CD0(field_166_angle) * (field_16C * ((field_BC_ypos - FP_FromInteger(field_162_hoistY)) / field_168)));

        FP pX1 = {};
        if (xOff < FP_FromInteger(0))
        {
            pX1 = xOff + field_B8_xpos;
        }
        else
        {
            pX1 = field_B8_xpos;
        }

        const FP pY1 = field_C8_vely + field_BC_ypos - FP_FromInteger(field_CC_sprite_scale < FP_FromInteger(1) ? 10 : 20);
        
        FP pX2;
        if (xOff < FP_FromInteger(0))
        {
            pX2 = field_B8_xpos;
        }
        else
        {
            pX2 = xOff + field_B8_xpos;
        }

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(pX1, pY1, pX2, field_C8_vely + field_BC_ypos, &pLine, &hitX, &hitY, field_D6_scale != 0 ? 6 : 0x60))
        {
            switch (pLine->field_8_type)
            {
            case 1u:
            case 5u:
                Sound_430520(9u);
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
                Sound_430520(9u);
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

        field_B8_xpos = xOff + FP_FromInteger(field_160_hoistX);
        field_166_angle = field_166_angle + 16;
        field_BC_ypos += field_C8_vely;

        if (field_BC_ypos <= FP_FromInteger(field_162_hoistY))
        {
            field_B8_xpos = FP_FromInteger(field_160_hoistX);
            field_BC_ypos = FP_FromInteger(field_162_hoistY);
            field_C8_vely = FP_FromInteger(0);
            field_174_flags.Set(Flags_174::eBit1_bHoistDone);
        }
        return;
    }
}

void Fleech::M_SettleOnGround_13_42FB00()
{
    if (!field_20_animation.field_92_current_frame)
    {
        sub_42BAD0();

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - FP_FromInteger(5),
            field_BC_ypos - FP_FromInteger(5),
            field_B8_xpos + FP_FromInteger(5),
            field_BC_ypos + FP_FromInteger(5),
            &pLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16))
        {
            field_100_pCollisionLine = pLine;
            MapFollowMe_408D10(TRUE);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Fleech::ToIdle_42E520();
    }
}

void Fleech::M_ExtendTongueFromEnemy_14_42FBD0()
{
    if (field_11C_obj_id == sActiveHero_5C1B68->field_8_object_id && 
        (sActiveHero_5C1B68->CantBeDamaged_44BAB0() || sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible)))
    {
        ToIdle_42E520();
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        PullTargetIn_42BAF0();
        field_106_current_motion = eFleechMotions::M_RetractTongueFromEnemey_15_42FC40;
    }
}

void Fleech::M_RetractTongueFromEnemey_15_42FC40()
{
    if (sObjectIds_5C1B70.Find_449CF0(field_11C_obj_id) == sActiveHero_5C1B68 && 
        ((sActiveHero_5C1B68->CantBeDamaged_44BAB0()) || sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible)))
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

        if (field_108_next_motion == eFleechMotions::M_Consume_18_42FDF0)
        {
            field_106_current_motion = eFleechMotions::M_Consume_18_42FDF0;
            field_108_next_motion = -1;
        }
    }
}

void Fleech::M_DeathByFalling_16_42FCE0()
{
    if (field_10C_health > FP_FromInteger(0))
    {
        auto pBlood = alive_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(field_B8_xpos, field_BC_ypos - FP_FromInteger(8), FP_FromInteger(0), -FP_FromInteger(5), field_CC_sprite_scale, 50);
        }
        
        Sound_430520(12);
        Sound_430520(7);
        
        field_10C_health = FP_FromInteger(0);
        field_124_brain_state = eFleechBrains::eAI_Death_3_42D1E0;
        field_174_flags.Set(Flags_174::eBit3);
        field_108_next_motion = -1;
        field_12C = sGnFrame_5C1B84 + 127;
        sFleechCount_5BC20E--;
    }
}

void Fleech::M_SleepingWithTongue_17_42F370()
{
    if (field_108_next_motion != -1)
    {
        sub_42B9F0();
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
    }
    else
    {
        if (field_20_animation.field_92_current_frame || sGnFrame_5C1B84 & 3)
        {
            if (field_20_animation.field_92_current_frame == 4 && !(sGnFrame_5C1B84 & 3))
            {
                Sound_430520(4);
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                {
                    auto pSnoozeParticle = alive_new<SnoozeParticle>();
                    if (pSnoozeParticle)
                    {
                        const FP yPos = (field_CC_sprite_scale * FP_FromInteger(-20)) + field_BC_ypos;
                        FP xOff = {};
                        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            xOff = -(field_CC_sprite_scale * FP_FromInteger(10));
                        }
                        else
                        {
                            xOff = (field_CC_sprite_scale * FP_FromInteger(10));
                        }
                        pSnoozeParticle->ctor_4B06F0(xOff + field_B8_xpos, yPos, field_20_animation.field_C_render_layer, field_20_animation.field_14_scale);
                    }
                }
            }
        }
        else
        {
            Sound_430520(3u);
        }

        field_B8_xpos = FP_FromInteger(field_160_hoistX) + (Math_Cosine_496CD0(field_166_angle) * FP_FromInteger(4)) - FP_FromInteger(field_DA_xOffset);
        field_166_angle += 2;
    }
}

void Fleech::M_Consume_18_42FDF0()
{
    if (field_20_animation.field_92_current_frame == 2)
    {
        Sound_430520(2);
    }
    else if (field_20_animation.field_92_current_frame == 15 && field_11C_obj_id == sActiveHero_5C1B68->field_8_object_id)
    {
        sActiveHero_5C1B68->sub_459430();

        Sound_430520(1);

        for (int i = 0; i < 3; i++)
        {
            auto pDove = alive_new<Dove>();
            if (pDove)
            {
                pDove->ctor_41F660(5516, 41, 20u, 60, field_B8_xpos, field_BC_ypos + FP_FromInteger(10), field_CC_sprite_scale);
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                pDove->field_B8_xpos += (field_CC_sprite_scale * FP_FromInteger(15));
            }
            else
            {
                pDove->field_B8_xpos -= (field_CC_sprite_scale * FP_FromInteger(15));
            }

            pDove->field_CC_sprite_scale = field_CC_sprite_scale;
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_42E520();
    }
}

void Fleech::dtor_42A3A0()
{
    SetVTable(this, 0x544F28);
    field_170_danger_obj = -1;
    if (field_118_tlvInfo != 0xFFFF)
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, field_10C_health <= FP_FromInteger(0));
    }

    MusicController::sub_47FD60(0, this, 0, 0);
    sub_42CF70();

    if (field_106_current_motion == 18)
    {
        if (sActiveHero_5C1B68)
        {
            if (field_11C_obj_id == sActiveHero_5C1B68->field_8_object_id)
            {
                sActiveHero_5C1B68->sub_459430(); // kills abe ??
            }
        }
    }
    field_11C_obj_id = -1;

    if (!field_174_flags.Get(Flags_174::eBit3))
    {
        sFleechCount_5BC20E--;
    }

    dtor_4080B0();
}

Fleech* Fleech::vdtor_42A140(signed int flags)
{
    dtor_42A3A0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Fleech::vUpdate_42AB20()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type == -1)
        {
            field_100_pCollisionLine = nullptr;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);
        }
        field_104_collision_line_type = 0;
        field_11C_obj_id = BaseGameObject::Find_Flags_4DC170(field_11C_obj_id);
        field_170_danger_obj = BaseGameObject::Find_Flags_4DC170(field_170_danger_obj);
        field_110_id = BaseGameObject::Find_Flags_4DC170(field_110_id);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if ((FP_Abs(field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos) <= FP_FromInteger(750) &&
        FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos) <= FP_FromInteger(520)) ||
        field_174_flags.Get(Flags_174::eBit7_persistant))
    {
        const auto oldMotion = field_106_current_motion;
        const auto oldBrain = sFleech_ai_table_551830[field_124_brain_state];
        const auto oldBrainRet = field_126_state;

        field_126_state = (this->*sFleech_ai_table_551830[field_124_brain_state])();

        TongueUpdate_42BD30();

        if (field_BC_ypos < FP_FromInteger(0))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }

        const FP oldY = field_BC_ypos;
        const FP oldX = field_B8_xpos;

        (this->*sFleech_motion_table_551798[field_106_current_motion])();

        if (oldX != field_B8_xpos || oldY != field_BC_ypos)
        {
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
        }

        // TODO: This is extra debug logging to figure out the ai names
        if (oldBrain != sFleech_ai_table_551830[field_124_brain_state])
        {
            LOG_INFO("Fleech: Old brain = " << GetOriginalFn(oldBrain, sFleechAiTable).fnName << " new brain = " << GetOriginalFn(sFleech_ai_table_551830[field_124_brain_state], sFleechAiTable).fnName);
        }
        else
        {
            if (oldBrainRet != field_126_state)
            {
                LOG_INFO("Fleech: Brain state = " << GetOriginalFn(sFleech_ai_table_551830[field_124_brain_state], sFleechAiTable).fnName << " State change from " << oldBrainRet << " to " << field_126_state);
            }
        }

        if (oldMotion == field_106_current_motion)
        {
            if (field_128)
            {
                field_106_current_motion = field_F4;
                SetAnim_429D80();
                field_20_animation.SetFrame_409D50(field_F6_anim_frame);
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
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void Fleech::vRender_42A550(int** ot)
{
    if (field_1C_update_delay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
        RenderEx_42C5A0(ot);
    }
}

void Fleech::RenderEx_42C5A0(int** /*ot*/)
{
    NOT_IMPLEMENTED();
}

void Fleech::vScreenChanged_42A4C0()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId || 
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead);
        field_11C_obj_id = -1;
        field_170_danger_obj = -1;
    }
}

void Fleech::vOn_Tlv_Collision_42AAB0(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            field_10C_health = FP_FromInteger(0);
            field_6_flags.Set(BaseGameObject::eDead);
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, field_B8_xpos, field_BC_ypos, field_B8_xpos, field_BC_ypos);
    }
}

__int16 Fleech::IsScrabOrParamiteNear_42B440(FP radius)
{
    BaseAliveGameObject* pNearestScrabOrParamite = nullptr;
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pBaseObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pBaseObj)
        {
            break;
        }

        if (pBaseObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject))
        {
            auto pObj = static_cast<BaseAliveGameObject*>(pBaseObj);
            if ((pObj->field_4_typeId == Types::eScrab_112 || pObj->field_4_typeId == Types::eParamite_96) && pObj->field_10C_health > FP_FromInteger(0))
            {
                if (pObj->field_CC_sprite_scale == field_CC_sprite_scale)
                {
                    bool check = false;
                    if (field_BC_ypos >= pObj->field_BC_ypos)
                    {
                        if (field_BC_ypos - pObj->field_BC_ypos > (FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? 20 : 10) * FP_FromInteger(2)))
                        {
                            if (field_BC_ypos < pObj->field_BC_ypos)
                            {
                                if (pObj->field_BC_ypos - field_BC_ypos <= (FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? 2 : 1)))
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
                        if (pObj->field_BC_ypos - field_BC_ypos <= (FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? + 2 : 1)))
                        {
                            check = true;
                        }
                    }

                    if (check)
                    {
                        if (vIsObjNearby_4253B0(radius, pObj))
                        {
                            if (!WallHit_408750(FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? 10 : 5), pObj->field_B8_xpos - field_B8_xpos))
                            {
                                if (!pNearestScrabOrParamite)
                                {
                                    pNearestScrabOrParamite = pObj;
                                }
                                else if (FP_GetExponent(FP_Abs(pObj->field_B8_xpos - field_B8_xpos)) <
                                         FP_GetExponent(FP_Abs(pNearestScrabOrParamite->field_B8_xpos - field_B8_xpos)))
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

TintEntry stru_551844[15] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 137u, 137u, 137u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u }
};

int CC Animation_OnFrame_Fleech_449A60(void* pObj, signed __int16* pData)
{
    reinterpret_cast<Fleech*>(pObj)->vOnFrame_42BC50(pData);
    return 1;
}

void Fleech::Init_42A170()
{
    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kFleechResID, TRUE, FALSE));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 580, TRUE, FALSE));
    
    Animation_Init_424E10(37704, 73, 35u, field_10_resources_array.ItemAt(0), 1, 1);

    field_20_animation.field_1C_fn_ptr_array = kFleech_Anim_Frame_Fns_55EFD0;

    field_4_typeId = Types::eFleech_50;

    field_6_flags.Set(BaseGameObject::eCanExplode);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_174_flags.Clear(Flags_174::eBit3);
    field_174_flags.Clear(Flags_174::eBit4);

    field_DC_bApplyShadows |= 2u;

    field_12C = 0;
    field_126_state = 0;
    field_108_next_motion = -1;
    field_110_id = -1;
    field_128 = 0;
    field_11C_obj_id = -1;
    field_170_danger_obj = -1;
    field_15E = 0;
    
    SetTint_425600(&stru_551844[0], gMap_5C3030.sCurrentLevelId_5C3030);
    
    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 34;
        field_D6_scale = 1;
    }
    else
    {
        field_20_animation.field_C_render_layer = 15;
        field_D6_scale = 0;
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
    }
    
    MapFollowMe_408D10(TRUE);
    
    vStackOnObjectsOfType_425840(Types::eFleech_50);
    
    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    sFleechCount_5BC20E++;
}

void Fleech::InitTonguePolys_42B6E0()
{
    field_18A.Clear(Flags_18A::e18A_TongueActive_Bit1);
    field_18A.Clear(Flags_18A::e18A_Bit2);

    field_180_tongue_x = FP_GetExponent(field_B8_xpos);
    field_182_tongue_y = FP_GetExponent((FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? -10 : -5)) + field_BC_ypos);
    
    field_178_tongue_state = 1;

    field_184 = -1;
    field_186 = -1;

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 2; i++)
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
    BYTE** ppRes = ResBlockForMotion_42A530(field_106_current_motion);
    field_20_animation.Set_Animation_Data_409C80(sFleechFrameTableOffsets_5517E4[field_106_current_motion], ppRes);
}

void Fleech::sub_42CF70()
{
    if (dword_551840 == field_8_object_id)
    {
        dword_551840 = -1;
    }
}

__int16 Fleech::sub_42CFA0()
{
    return dword_551840 == -1 && !field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted);
}

void Fleech::sub_42CF50()
{
    dword_551840 = field_8_object_id;
}

void Fleech::sub_42B9A0(__int16 a2, __int16 a3)
{
    field_18A.Set(Flags_18A::e18A_Bit2);
    field_178_tongue_state = 2;
    field_186 = a3;
    field_184 = a2;
    field_188 = 0;
}

void Fleech::TongueUpdate_42BD30()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_11C_obj_id));
    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        field_18A.Clear(Flags_18A::e18A_Bit2);
    }

    switch (field_178_tongue_state)
    {
    case 1:
        field_18A.Clear(Flags_18A::e18A_TongueActive_Bit1);
        field_18A.Clear(Flags_18A::e18A_Bit2);
        return;

    case 2:
    case 4:
        field_18A.Set(Flags_18A::e18A_Bit2);
        return;

    case 3:
        field_18A.Set(Flags_18A::e18A_Bit2);
        switch (field_17A_tongue_sub_state++)
        {
        case 0:
            field_184 = field_160_hoistX;
            field_186 = field_162_hoistY;
            Sound_430520(11u);
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
            PSX_RECT bRect = {};
            pTarget->vGetBoundingRect_424FD0(&bRect, 1);

            field_18A.Set(Flags_18A::e18A_Bit2);
            field_17C = FP_GetExponent(pTarget->field_B8_xpos);
            field_17E = (bRect.y + bRect.h) >> 1;
            field_186 = (bRect.y + bRect.h) >> 1;
            field_184 = field_17C;

            const FP v12 = (FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? 20 : 10) * FP_FromInteger(7));
            if (FP_FromInteger(Math_Distance_496EB0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                field_184, field_186)) <= v12)
            {
                switch (field_17A_tongue_sub_state++)
                {
                case 0:
                {
                    Sound_430520(10);
                    auto pBlood = alive_new<Blood>();
                    if (pBlood)
                    {
                        pBlood->ctor_40F0B0(
                            FP_FromInteger(field_17C),
                            FP_FromInteger(field_17E),
                            field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) != 0 ? FP_FromInteger(2) : FP_FromInteger(-2),
                            FP_FromInteger(1),
                            field_CC_sprite_scale, 20);
                    }
                    break;
                }

                case 1:
                    field_188 = 9;
                    break;

                case 2:
                    field_188 = -6;
                    sub_42CF70();
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
                    if (pTarget->field_10C_health > FP_FromInteger(0))
                    {
                        pTarget->VTakeDamage_408730(this);
                        if (pTarget->field_10C_health <= FP_FromInteger(0))
                        {
                            field_178_tongue_state = 7;
                            field_17A_tongue_sub_state = 0;
                            Sound_430520(8u);
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
            PSX_RECT bRect = {};
            pTarget->vGetBoundingRect_424FD0(&bRect, 1);

            switch (field_17A_tongue_sub_state++)
            {
            case 4:
                field_108_next_motion = eFleechMotions::M_Consume_18_42FDF0;
                // Fall through
            case 0:
            case 1:
            case 2:
            case 3:
            case 5:
                if (field_17A_tongue_sub_state == 5 && pTarget->field_4_typeId == Types::eScrab_112)
                {
                    pTarget->field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                }
                pTarget->field_B8_xpos -= (pTarget->field_B8_xpos - field_B8_xpos) * FP_FromDouble(0.25);
                pTarget->field_BC_ypos -= (pTarget->field_BC_ypos - field_BC_ypos) * FP_FromDouble(0.25);
                break;

            case 6:
                field_18A.Clear(Flags_18A::e18A_Bit2);
                field_178_tongue_state = 8;
                pTarget->field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                pTarget->field_B8_xpos = field_B8_xpos;
                pTarget->field_BC_ypos = field_BC_ypos;
                if (pTarget == sActiveHero_5C1B68)
                {
                    sActiveHero_5C1B68->FleechDeath_459350();
                }
                break;

            default:
                break;
            }

            field_17C = FP_GetExponent(pTarget->field_B8_xpos);
            field_184 = field_17C;
            field_17E = (bRect.y + bRect.h) >> 1;
            field_186 = (bRect.y + bRect.h) >> 1;
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
            PSX_RECT bRect = {};
            pTarget->vGetBoundingRect_424FD0(&bRect, 1);
            field_17C = FP_GetExponent((field_B8_xpos + pTarget->field_B8_xpos) * FP_FromDouble(0.5));
            field_17E = (bRect.y + bRect.h) >> 1;
            field_184 = FP_GetExponent((field_B8_xpos + pTarget->field_B8_xpos) * FP_FromDouble(0.5));
            field_186 = (bRect.y + bRect.h) >> 1;
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
    MapFollowMe_408D10(TRUE);
    field_138 = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = eFleechMotions::M_Idle_3_42E850;
    field_108_next_motion = -1;
    field_134 = 60 * sRandomBytes_546744[sFleechRandomIdx_5BC20C++] / 256 + sGnFrame_5C1B84 + 120;
}

int Fleech::Sound_430520(unsigned __int8 /*sfx*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

BYTE** Fleech::ResBlockForMotion_42A530(int /*motion*/)
{
    return field_10_resources_array.ItemAt(0);
}

__int16 Fleech::CanMove_42E3E0()
{
    if (field_108_next_motion == eFleechMotions::M_Knockback_6_42EAF0)
    {
        field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
        field_108_next_motion = -1;
        return 1;
    }

    if (field_108_next_motion != eFleechMotions::M_Crawl_4_42E960)
    {
        return 0;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(7));
    }
    else
    {
        field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(7));
    }

    const FP yDist = FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? 10 : 5);
    const FP xDist = ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? -1 : 1);
    if (!WallHit_408750(yDist, xDist) && !HandleEnemyStopperOrSlamDoor_42ADC0(1))
    {
        field_106_current_motion = eFleechMotions::M_Crawl_4_42E960;
        field_108_next_motion = -1;
        return 1;
    }

    ToIdle_42E520();
    return 0;
}

__int16 Fleech::HandleEnemyStopperOrSlamDoor_42ADC0(int /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int Fleech::UpdateWakeUpSwitchValue_4308B0()
{
    const __int16 curSwitchValue = static_cast<__int16>(SwitchStates_Get_466020(field_144_wake_up_id));
    const __int16 wakeUpValue = field_148_wake_up_switch_value;
    
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

__int16 Fleech::vTakeDamage_42A5C0(BaseGameObject* pFrom)
{
    if (field_10C_health <= FP_FromInteger(0))
    {
        return 0;
    }
    
    sub_42B8C0();
    sub_42CF70();
    
    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_15:
    case Types::eGrinder_30:
    case Types::eBaseBomb_46:
    case Types::eExplosion_109:
    case Types::eSlig_125:
    {
        Sound_430520(7);
        field_10C_health = FP_FromInteger(0);
        auto pGibsMem = alive_new<Gibs>();
        if (pGibsMem)
        {
            pGibsMem->ctor_40FB40(10, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
        }

        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);
        auto pBloodMem = alive_new<Blood>();
        if (pBloodMem)
        {
            pBloodMem->ctor_40F0B0(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2),
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale, 50);
        }

        field_6_flags.Set(BaseGameObject::eDead);
    }
    break;


    case Types::eRockSpawner_48:
        Sound_430520(7);
        field_10C_health = FP_FromInteger(0);
        field_124_brain_state = eFleechBrains::eAI_Death_3_42D1E0;
        field_108_next_motion = -1;
        field_12C = sGnFrame_5C1B84 + 127;
        field_106_current_motion = eFleechMotions::M_Idle_3_42E850;
        SetAnim_429D80();
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        break;

    case Types::eParamite_96:
        Sound_430520(0xDu);
        // Fall through

    case Types::eScrab_112:
    {
        auto pGibsMem2 = alive_new<Gibs>();
        if (pGibsMem2)
        {
            pGibsMem2->ctor_40FB40(10, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
        }

        if (static_cast<BaseAliveGameObject*>(pFrom)->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            auto pBloodMem2 = alive_new<Blood>();
            if (pBloodMem2)
            {
                pBloodMem2->ctor_40F0B0(field_B8_xpos, field_BC_ypos - FP_FromInteger(8), -FP_FromInteger(5), -FP_FromInteger(5), field_CC_sprite_scale, 50);
            }
        }
        else
        {
            auto pBloodMem3 = alive_new<Blood>();
            if (pBloodMem3)
            {
                pBloodMem3->ctor_40F0B0(field_B8_xpos, field_BC_ypos - FP_FromInteger(8), FP_FromInteger(5), -FP_FromInteger(5), field_CC_sprite_scale, 50);
            }
        }

        if (!field_174_flags.Get(Flags_174::eBit4))
        {
            field_174_flags.Set(Flags_174::eBit4);
            Sound_430520(7);
        }

        field_10C_health = FP_FromInteger(0);
        field_124_brain_state = eFleechBrains::eAI_Death_3_42D1E0;
        field_106_current_motion = eFleechMotions::M_Idle_3_42E850;
        field_12C = sGnFrame_5C1B84 + 127;
        field_108_next_motion = -1;
        SetAnim_429D80();
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        field_174_flags.Set(Flags_174::eBit3);
        field_6_flags.Set(BaseGameObject::eDead);
        sFleechCount_5BC20E--;
    }
    break;

    case Types::eElectrocute_150:
        field_6_flags.Set(BaseGameObject::eDead);
        field_10C_health = FP_FromInteger(0);
        field_124_brain_state = eFleechBrains::eAI_Death_3_42D1E0;
        break;

    default:
        Sound_430520(7);
        field_114_flags.Set(Flags_114::e114_Bit7_Electrocuted);
        sub_42B8C0();
        break;
    }
    return 1;
}

void Fleech::vOnTrapDoorOpen_42E5C0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
    }
}

void Fleech::sub_42B9F0()
{
    field_178_tongue_state = 1;
}

void Fleech::IncreaseAnger_430920()
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        BaseAnimatedWithPhysicsGameObject* pEvent = Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, field_D6_scale);

        if (!pEvent)
        {
            pEvent = Event_Is_Event_In_Range_422C30(kEventAlarm, field_B8_xpos, field_BC_ypos, field_D6_scale);
        }

        if (!pEvent)
        {
            pEvent = Event_Is_Event_In_Range_422C30(kEventLoudNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);
        }

        if (pEvent)
        {
            if ((pEvent != sActiveHero_5C1B68 || !sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible)) &&
                gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, pEvent->field_B8_xpos, pEvent->field_BC_ypos, 0))
            {
                field_13E_anger += field_142_attack_anger;
                if (vOnSameYLevel_425520(pEvent))
                {
                    if (pEvent->field_4_typeId == Types::eScrab_112 || pEvent->field_4_typeId == Types::eParamite_96)
                    {
                        field_14E = FP_GetExponent(pEvent->field_B8_xpos);
                    }
                    else
                    {
                        field_14C = FP_GetExponent(pEvent->field_B8_xpos);
                    }
                }
            }
        }

        pEvent = Event_Is_Event_In_Range_422C30(kEventNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);
        if (pEvent)
        {
            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(6), pEvent))
            {
                if ((pEvent != sActiveHero_5C1B68 || !sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))  &&
                    gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, pEvent->field_B8_xpos, pEvent->field_BC_ypos, 0))
                {
                    field_13E_anger += field_140;
                }
            }
        }
    }
}

__int16 Fleech::InRange_4307C0(BaseAliveGameObject* pObj)
{
    if (!pObj || pObj == sActiveHero_5C1B68 && sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        return FALSE;
    }

    if (FP_Abs(pObj->field_B8_xpos - field_B8_xpos) >= ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(10) ||
        FP_Abs(pObj->field_BC_ypos - field_BC_ypos) >= ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(1) || 
        pObj->field_CC_sprite_scale != field_CC_sprite_scale)
    {
        return FALSE;
    }

    return TRUE;
}

int Fleech::TongueActive_42B8A0()
{
    return field_18A.Get(Flags_18A::e18A_TongueActive_Bit1);
}

void Fleech::PullTargetIn_42BAF0()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_11C_obj_id));
    if (pTarget)
    {
        field_18A.Set(Flags_18A::e18A_TongueActive_Bit1);
        field_18A.Set(Flags_18A::e18A_Bit2);
        field_178_tongue_state = 6;
        field_17A_tongue_sub_state = 0;

        PSX_RECT bRect = {};
        pTarget->vGetBoundingRect_424FD0(&bRect, 1);

        field_17C = FP_GetExponent(pTarget->field_B8_xpos);
        field_17E = (bRect.y + bRect.h) / 2;
        field_184 = FP_GetExponent(((FP_FromInteger(field_17C) + field_B8_xpos) / FP_FromInteger(2)));
        field_188 = 0;
        field_186 = FP_GetExponent(((FP_FromInteger(field_17E) + field_BC_ypos) / FP_FromInteger(2)));
    }
}

void Fleech::sub_42B8C0()
{
    sub_42CF70();

    if (field_178_tongue_state > 1)
    {
        field_178_tongue_state = 10;
        field_184 = FP_GetExponent(((FP_FromInteger(field_184)) + field_B8_xpos) / FP_FromInteger(2));
        field_188 = 0;
        field_186 = FP_GetExponent(((FP_FromInteger(field_186)) + field_BC_ypos) / FP_FromInteger(2));
    }
    else
    {
        field_18A.Clear(Flags_18A::e18A_Bit2);
        field_178_tongue_state = 1;
    }
}

void Fleech::sub_42BA10()
{
    field_18A.Set(Flags_18A::e18A_TongueActive_Bit1);
    field_18A.Set(Flags_18A::e18A_Bit2);
    field_178_tongue_state = 3;
    field_17A_tongue_sub_state = 0;
    field_184 = FP_GetExponent(((FP_FromInteger(field_160_hoistX)) + field_B8_xpos) / FP_FromInteger(2));
    field_188 = 0;
    field_186 = FP_GetExponent(((FP_FromInteger(field_162_hoistY)) + field_BC_ypos) / FP_FromInteger(2));
}

void Fleech::sub_42BAD0()
{
    field_178_tongue_state = 5;
}

BaseAliveGameObject* Fleech::FindMudOrAbe_42CFD0()
{
    BaseAliveGameObject* pRet = nullptr;
    FP lastDist = FP_FromInteger(gPsxDisplay_5C1130.field_0_width);
    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if ((pObj->field_4_typeId == Types::eMudokon_110 || pObj->field_4_typeId == Types::eAbe_69) &&
            pObj->field_D6_scale == field_D6_scale &&
            pObj->field_10C_health > FP_FromInteger(0))
        {
            const FP dist = FP_FromInteger(
                Math_Distance_496EB0(
                    FP_GetExponent(pObj->field_B8_xpos),
                    FP_GetExponent(pObj->field_BC_ypos),
                    FP_GetExponent(field_B8_xpos), 
                    FP_GetExponent(field_BC_ypos)));

            if (dist < lastDist && 
                FP_GetExponent(field_B8_xpos) / 375 == (FP_GetExponent(pObj->field_B8_xpos) / 375) && 
                FP_GetExponent(field_BC_ypos) / 260 == (FP_GetExponent(pObj->field_BC_ypos) / 260))
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
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));

    const FP prev_xpos = field_B8_xpos;
    const FP prev_ypos = field_BC_ypos;

    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        if (field_100_pCollisionLine && ((field_D6_scale != 0 ? 1 : 16) & (1 << field_100_pCollisionLine->field_8_type)))
        {
            if (pPlatform)
            {
                if (field_100_pCollisionLine->field_8_type != 32 && field_100_pCollisionLine->field_8_type != 36)
                {
                    pPlatform->VRemove(this);
                    field_110_id = -1;
                }
            }
            else if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);
                const PSX_Point xy = { bRect.x, static_cast<short>(bRect.y + 5) };
                const PSX_Point wh = { bRect.w, static_cast<short>(bRect.h + 5) };
                vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
        }
        else if (field_124_brain_state != eFleechBrains::eAI_Patrol_0_430BA0)
        {
            VOnTrapDoorOpen();
            field_138 = FP_FromInteger(0);
            field_F8_LastLineYPos = field_BC_ypos;
            field_B8_xpos = prev_xpos + field_C4_velx;
            field_106_current_motion = eFleechMotions::M_Fall_9_42ECD0;
        }
        else
        {
            field_B8_xpos = prev_xpos;
            field_BC_ypos = prev_ypos;
            field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
        }
    }
    else
    {
        field_F8_LastLineYPos = prev_ypos;
        field_138 = FP_FromInteger(0);
        field_106_current_motion = eFleechMotions::M_Fall_9_42ECD0;
    }
}

__int16 Fleech::IsNear_428670(BaseAliveGameObject* pObj)
{
    if (pObj && 
        field_CC_sprite_scale == pObj->field_CC_sprite_scale &&
        FP_GetExponent(FP_Abs(field_B8_xpos - pObj->field_B8_xpos)) <= 750 &&
        FP_GetExponent(FP_Abs(field_BC_ypos - pObj->field_BC_ypos)) <= 260)
    {
        return TRUE;
    }
    return FALSE;
}

void Fleech::vOnThrowableHit_42A590(BaseGameObject* /*pFrom*/)
{
    field_13E_anger += field_142_attack_anger;
}

BOOL Fleech::Collision_42B290(__int16 alwaysOne)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP quaterScaled = (kGridSize * FP_FromDouble(0.25));

    FP x2 = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        x2 = field_B8_xpos - (quaterScaled * FP_FromInteger(alwaysOne >= 0 ? 1 : -1));
    }
    else
    {
        x2 = (kGridSize * FP_FromInteger(alwaysOne)) + field_B8_xpos;
    }

    FP x1 = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        x1 = field_B8_xpos - (kGridSize * FP_FromInteger(alwaysOne));
    }
    else
    {
        x1 = (quaterScaled * FP_FromInteger(alwaysOne >= 0 ? 1 : -1)) + field_B8_xpos;
    }

    const FP y2 = field_BC_ypos + FP_FromInteger(2);
    const FP y1 = field_BC_ypos - FP_FromInteger(2);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions_DArray_5C1128->Raycast_417A60(x1, y1, x2, y2, &pLine, &hitX, &hitY, field_D6_scale ? 0x01 : 0x10) == 0;
}

Path_Hoist* Fleech::TryGetHoist_42AFD0(int xDistance, __int16 bIgnoreDirection)
{
    if (field_106_current_motion == eFleechMotions::M_Fall_9_42ECD0)
    {
        return nullptr;
    }

    const FP yAmount = FP_FromInteger(field_CC_sprite_scale < FP_FromInteger(1) ? 10 : 20);
    const FP y1 = field_BC_ypos - yAmount;
    const FP y2 = y1 - (yAmount * FP_FromInteger(1));

    const FP xSnapped = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
    FP xCheck = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        xCheck = xSnapped - (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(xDistance));
    }
    else
    {
        xCheck = (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(xDistance)) + xSnapped;
    }

    auto pHoist = static_cast<Path_Hoist*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(std::min(xCheck, field_B8_xpos)),
        FP_GetExponent(y2),
        FP_GetExponent(std::max(xCheck, field_B8_xpos)),
        FP_GetExponent(y1),
        TlvTypes::Hoist_2));

    if (!pHoist)
    {
        return nullptr;
    }

    if (WallHit_408750(
        FP_FromInteger(field_CC_sprite_scale < FP_FromInteger(1) ? 5 : 10),
        FP_FromInteger(pHoist->field_8_top_left.field_0_x + (field_CC_sprite_scale < FP_FromInteger(1) ? 6 : 12)) - field_B8_xpos))
    {
        return nullptr;
    }

    if (HandleEnemyStopperOrSlamDoor_42ADC0(xDistance))
    {
        return nullptr;
    }

    if (pHoist->field_16_scale != (field_CC_sprite_scale < FP_FromInteger(1) ? Path_Hoist::Scale::eHalf : Path_Hoist::Scale::eFull) ||
        field_BC_ypos -  FP_FromInteger(pHoist->field_8_top_left.field_2_y) >
        FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? 20 : 10) * FP_FromDouble(5.5))
    {
        return nullptr;
    }

    if (bIgnoreDirection)
    {
        return pHoist;
    }

    if (pHoist->field_12_edge_type == (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? Path_Hoist::EdgeType::eRight : Path_Hoist::EdgeType::eLeft) ||
        pHoist->field_12_edge_type == Path_Hoist::EdgeType::eBoth)
    {
        return pHoist;
    }

    return nullptr;
}

void Fleech::vOnFrame_42BC50(signed __int16* pData)
{
    FP xpos = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        xpos = field_B8_xpos - (field_CC_sprite_scale * FP_FromInteger(pData[0]));
    }
    else
    {
        xpos = (field_CC_sprite_scale * FP_FromInteger(pData[0])) + field_B8_xpos;
    }

    field_180_tongue_x = FP_GetExponent(xpos) + field_DA_xOffset;
    field_182_tongue_y = FP_GetExponent((field_CC_sprite_scale * FP_FromInteger(pData[1])) + field_BC_ypos);
}

const char byte_551984[] =
{
    '\0',
    '\0',
    '\x05',
    '\0',
    '\x05',
    '\x05',
    '\x05',
    '\x05',
    '\x05',
    '\x05',
    '\x05',
    '\x05',
    '\0',
    '\0',
    '\0',
    '\0'
};

__int16 Fleech::AI_Patrol_0_430BA0()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_11C_obj_id));
    if (!pTarget ||
        pTarget->field_6_flags.Get(BaseGameObject::eDead) ||
        pTarget->field_10C_health <= FP_FromInteger(0) ||
        pTarget->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_11C_obj_id = -1;
        pTarget = 0;
    }

    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        MusicController::sub_47FD60(byte_551984[field_126_state], this, 0, 0);
    }
    else
    {
        MusicController::sub_47FD60(0, this, 0, 0);
    }

    switch (field_126_state)
    {
    case 0u:
        field_156 = Fleech_NextRandom() & 0x3F;
        field_15A = 0;
        field_152 = FP_GetExponent(field_B8_xpos);
        field_14C = -1;
        field_14E = -1;

        if (field_13E_anger > field_140)
        {
            return 4;
        }

        if (field_106_current_motion == eFleechMotions::M_Sleeping_0_42F0B0)
        {
            return 1;
        }

        if (field_106_current_motion != eFleechMotions::M_SleepingWithTongue_17_42F370 && !field_174_flags.Get(Flags_174::eBit6_goes_to_sleep))
        {
            return 4;
        }

        if (field_106_current_motion == eFleechMotions::M_Sleeping_0_42F0B0 || field_106_current_motion == eFleechMotions::M_SleepingWithTongue_17_42F370)
        {
            return 1;
        }

        return 3;

    case 1u:
    {
        if (!SwitchStates_Get_466020(field_14A_allow_wake_up_id))
        {
            return field_126_state;
        }

        if (!(static_cast<int>(sGnFrame_5C1B84) % 16))
        {
            if (field_13E_anger > 0)
            {
                field_13E_anger--;
            }
        }

        IncreaseAnger_430920();

        if (UpdateWakeUpSwitchValue_4308B0())
        {
            const __int16 v11 = (field_142_attack_anger - field_140) / 2;
            if (field_13E_anger < v11 + field_140)
            {
                field_13E_anger = field_140 + v11;
            }
        }

        if (!IsScrabOrParamiteNear_42B440(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(8)))
        {
            if (field_13E_anger <= field_140)
            {
                return field_126_state;
            }

            if (field_106_current_motion == eFleechMotions::M_SleepingWithTongue_17_42F370)
            {
                field_108_next_motion = eFleechMotions::M_Fall_9_42ECD0;
                field_F8_LastLineYPos = field_BC_ypos;
                return 4;
            }
        }
        else
        {
            BaseGameObject* pDangerObj = sObjectIds_5C1B70.Find_449CF0(field_170_danger_obj);
            if (pDangerObj && pDangerObj->field_4_typeId != Types::eParamite_96)
            {
                if (field_106_current_motion == eFleechMotions::M_SleepingWithTongue_17_42F370)
                {
                    field_108_next_motion = eFleechMotions::M_Fall_9_42ECD0;
                    field_F8_LastLineYPos = field_BC_ypos;
                    return 4;
                }
            }
            else
            {
                field_170_danger_obj = -1;
                if (field_13E_anger <= field_140)
                {
                    return field_126_state;
                }

                if (field_106_current_motion == eFleechMotions::M_SleepingWithTongue_17_42F370)
                {
                    field_108_next_motion = eFleechMotions::M_Fall_9_42ECD0;
                    field_F8_LastLineYPos = field_BC_ypos;
                    return 4;
                }
            }
        }
        field_108_next_motion = eFleechMotions::M_WakingUp_1_42F270;
        return 2;
    }

    case 2u:
        if (field_106_current_motion != eFleechMotions::M_WakingUp_1_42F270)
        {
            return field_126_state;
        }

        if (field_100_pCollisionLine)
        {
            field_108_next_motion = eFleechMotions::M_PatrolCry_5_42E810;
        }
        else
        {
            field_108_next_motion = eFleechMotions::M_Fall_9_42ECD0;
            field_F8_LastLineYPos = field_BC_ypos;
        }
        return 4;

    case 3u:
        if (field_106_current_motion == eFleechMotions::M_Idle_3_42E850)
        {
            field_108_next_motion = eFleechMotions::M_Unknown_2_42F2F0;
        }

        if (field_106_current_motion != eFleechMotions::M_Unknown_2_42F2F0)
        {
            return field_126_state;
        }

        field_13E_anger = 0;
        field_108_next_motion = eFleechMotions::M_Sleeping_0_42F0B0;
        return 1;

    case 4u:
    {
        if (field_11C_obj_id == -1)
        {
            pTarget = FindMudOrAbe_42CFD0();
            if (pTarget)
            {
                field_11C_obj_id = pTarget->field_8_object_id;
            }
        }

        if (!(static_cast<int>(sGnFrame_5C1B84) % 32))
        {
            if (field_13E_anger > 0)
            {
                field_13E_anger--;
            }
        }

        if (IsScrabOrParamiteNear_42B440(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(8)))
        {
            auto pDangerObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_170_danger_obj));
            if (pDangerObj == sControlledCharacter_5C1B8C)
            {
                field_124_brain_state = eFleechBrains::eAI_Scared_2_42D310;
                return 0;
            }

            if (vIsObjNearby_4253B0(FP_FromInteger(2) * ScaleToGridSize_4498B0(field_CC_sprite_scale), pDangerObj))
            {
                const __int16 v27 = (field_142_attack_anger - field_140) / 2;
                if (field_13E_anger < v27 + field_140)
                {
                    field_13E_anger = field_140 + v27;
                }
                field_14E = FP_GetExponent(pDangerObj->field_B8_xpos); // TODO: abs ?
            }
        }

        // TODO: Check OFSUB branches
        if (field_14E >= 0)
        {
            if ((FP_FromInteger(field_14E) > field_B8_xpos && !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) ||
                (FP_FromInteger(field_14E) < field_B8_xpos && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
            {
                if (field_106_current_motion == eFleechMotions::M_Crawl_4_42E960)
                {
                    field_106_current_motion = eFleechMotions::M_StopCrawling_7_42EBB0;
                }
                else if (field_106_current_motion == eFleechMotions::M_Idle_3_42E850)
                {
                    field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
                }
            }
            return 5;
        }

        IncreaseAnger_430920();

        if (InRange_4307C0(pTarget))
        {
            field_13E_anger += field_142_attack_anger;
        }

        if (UpdateWakeUpSwitchValue_4308B0())
        {
            field_13E_anger += field_146_tlv_28;
        }

        if (pTarget)
        {
            if (!pTarget->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible) &&
                vOnSameYLevel_425520(pTarget) &&
                gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, pTarget->field_B8_xpos, pTarget->field_BC_ypos, 0) &&
                gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) &&
                !WallHit_408750(FP_FromInteger(field_CC_sprite_scale >= FP_FromInteger(1) ? 10 : 5), pTarget->field_B8_xpos - field_B8_xpos))
            {
                field_13E_anger = field_142_attack_anger + 1;
                return 8;
            }

            if (field_13E_anger > field_142_attack_anger && !pTarget->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible) && field_170_danger_obj == -1)
            {
                return 8;
            }
        }

        field_15A = 0;
        Path_Hoist* pHoist = TryGetHoist_42AFD0(0, 0);
        if (pHoist)
        {
            if (field_106_current_motion == eFleechMotions::M_Crawl_4_42E960)
            {
                field_108_next_motion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
            }
            else if (field_106_current_motion != eFleechMotions::M_Idle_3_42E850)
            {
                field_108_next_motion = eFleechMotions::M_Idle_3_42E850;
            }
            field_160_hoistX = pHoist->field_8_top_left.field_0_x + (field_CC_sprite_scale >= FP_FromInteger(1) ? 12 : 6);
            field_162_hoistY = pHoist->field_8_top_left.field_2_y;
            return 9;
        }

        if (!(Fleech_NextRandom() % 32) && field_106_current_motion == eFleechMotions::M_Idle_3_42E850)
        {
            field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
            return field_126_state;
        }

        if (field_106_current_motion == eFleechMotions::M_Crawl_4_42E960)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (field_B8_xpos < FP_FromInteger(field_154))
                {
                    field_108_next_motion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
                }
            }
            else if (field_B8_xpos > FP_FromInteger(field_154))
            {
                field_108_next_motion = eFleechMotions::M_StopMidCrawlCycle_8_42EB20;
            }
        }

        if (field_106_current_motion == eFleechMotions::M_Idle_3_42E850)
        {
            if (field_14C >= 0)
            {
                if (field_150_patrol_range > 0)
                {
                    if (FP_FromInteger(field_14C) <= field_B8_xpos)
                    {
                        short patrolRangeDelta = FP_GetExponent(field_B8_xpos) - field_150_patrol_range;
                        if (field_14C > patrolRangeDelta)
                        {
                            patrolRangeDelta = field_14C;
                        }
                        field_154 = patrolRangeDelta;

                        if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
                        }
                    }
                    else
                    {
                        short patrolRangeDelta = field_150_patrol_range + FP_GetExponent(field_B8_xpos);
                        if (field_14C <= patrolRangeDelta)
                        {
                            patrolRangeDelta = field_14C;
                        }
                        field_154 = patrolRangeDelta;

                        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;

                        }
                    }

                    field_108_next_motion = eFleechMotions::M_Crawl_4_42E960;
                    field_14C = -1;
                }
                else
                {
                    // TODO: Check __OFSUB__ on else branch
                    if (FP_FromInteger(field_14C) > field_B8_xpos)
                    {
                        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
                            field_14C = -1;
                        }
                    }
                    else if (FP_FromInteger(field_14C) < field_B8_xpos)
                    {
                        if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
                            field_14C = -1;
                        }
                    }
                }
            }
            else
            {
                if (field_150_patrol_range > 0)
                {
                    if (field_156 > 0)
                    {
                        field_156--;
                    }
                    else
                    {
                        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            field_154 = FP_GetExponent(field_B8_xpos) - Fleech_NextRandom() * (FP_GetExponent(field_B8_xpos) + field_150_patrol_range - field_152) / 255;
                        }
                        else
                        {
                            field_154 = FP_GetExponent(field_B8_xpos) + Fleech_NextRandom() * (field_150_patrol_range + field_152 - FP_GetExponent(field_B8_xpos)) / 255;
                        }
                        field_156 = Fleech_NextRandom() & 0x3F;
                        field_108_next_motion = eFleechMotions::M_Crawl_4_42E960;
                    }
                }
            }
        }

        if (!field_174_flags.Get(Flags_174::eBit6_goes_to_sleep) ||
            field_13E_anger >= field_140 &&
            gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            if ((Fleech_NextRandom() % 64) || field_106_current_motion != eFleechMotions::M_Idle_3_42E850)
            {
                return field_126_state;
            }
            field_106_current_motion = eFleechMotions::M_PatrolCry_5_42E810;
            return field_126_state;
        }
        field_13E_anger = 0;
        field_108_next_motion = eFleechMotions::M_Unknown_2_42F2F0;
        return 3;
    }

    case 5u:
    {
        if (field_106_current_motion == eFleechMotions::M_StopCrawling_7_42EBB0 || field_106_current_motion == eFleechMotions::M_Knockback_6_42EAF0)
        {
            return field_126_state;
        }

        if ((FP_FromInteger(field_14E) > field_B8_xpos && !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) ||
            (FP_FromInteger(field_14E) < field_B8_xpos && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
        {
            if (field_106_current_motion != eFleechMotions::M_Crawl_4_42E960)
            {
                if (field_106_current_motion == eFleechMotions::M_Idle_3_42E850)
                {
                    field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
                }
            }
            else
            {
                field_106_current_motion = eFleechMotions::M_StopCrawling_7_42EBB0;
            }
            return field_126_state;
        }

        field_14E = -1;
        field_108_next_motion = eFleechMotions::M_Crawl_4_42E960;
        CanMove_42E3E0();
        return 6;
    }

    case 6u:
    {
        auto pDangerObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_170_danger_obj));
        if (field_106_current_motion != eFleechMotions::M_Crawl_4_42E960 || pDangerObj &&
            (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(5), pDangerObj)))
        {
            if (field_106_current_motion != eFleechMotions::M_Idle_3_42E850)
            {
                return field_126_state;
            }
        }
        else
        {
            field_108_next_motion = eFleechMotions::M_Idle_3_42E850;
        }
        return 7;
    }

    case 7u:
        if (field_106_current_motion != eFleechMotions::M_Idle_3_42E850)
        {
            return field_126_state;
        }
        return 4;

    case 8u:
        if (pTarget == sActiveHero_5C1B68 && sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
        {
            return 0;
        }

        if (field_15A < field_158)
        {
            field_15A++;
            return field_126_state;
        }

        field_124_brain_state = 1;
        return 0;

    case 9u:
        if (field_106_current_motion != eFleechMotions::M_Idle_3_42E850)
        {
            return field_126_state;
        }
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && FP_FromInteger(field_160_hoistX) > field_B8_xpos ||
           !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && FP_FromInteger(field_160_hoistX) < field_B8_xpos)
        {
            field_106_current_motion = eFleechMotions::M_Knockback_6_42EAF0;
            return field_126_state;
        }
        field_108_next_motion = eFleechMotions::M_RaiseHead_11_42F590;
        return 10;

    case 10u:
        if (field_106_current_motion != eFleechMotions::M_Idle_3_42E850)
        {
            return field_126_state;
        }
        field_152 = FP_GetExponent(field_B8_xpos);
        return 4;

    default:
        return field_126_state;
    }
}

__int16 Fleech::AI_ChasingAbe_1_428760()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Fleech::AI_Scared_2_42D310()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Fleech::AI_Death_3_42D1E0()
{
    field_11C_obj_id = -1;
    MusicController::sub_47FD60(0, this, 0, 0);

    if (field_12C < static_cast<int>(sGnFrame_5C1B84 + 80))
    {
        field_CC_sprite_scale -= FP_FromDouble(0.022);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;
    }

    if (static_cast<int>(sGnFrame_5C1B84) < field_12C - 24 && !(sGnFrame_5C1B84 % 5))
    {
        const FP xRand = (FP_FromInteger(Math_RandomRange_496AB0(-24, 24)) * field_CC_sprite_scale);
        New_Particles_426C70(xRand + field_B8_xpos, field_BC_ypos - FP_FromInteger(6), field_CC_sprite_scale / FP_FromInteger(2), 2, 128u, 128u, 128u);
    }

    if (field_CC_sprite_scale < FP_FromInteger(0))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    return 100;
}
