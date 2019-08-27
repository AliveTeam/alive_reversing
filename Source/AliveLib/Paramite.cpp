#include "stdafx.h"
#include "Paramite.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "ObjectIds.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"
#include "Throwable.hpp"
#include "Map.hpp"
#include "Meat.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "PlatformBase.hpp"
#include "SwitchStates.hpp"
#include "PullRingRope.hpp"
#include "Sfx.hpp"
#include "Slurg.hpp"
#include "Particle.hpp"
#include "GameSpeak.hpp"
#include "ParamiteWeb.hpp"
#include "ParamiteWebLine.hpp"
#include "ScreenShake.hpp"

TintEntry stru_55D73C[24] =
{
    { 3u, 105u, 105u, 105u },
    { 11u, 105u, 105u, 105u },
    { -1, 105u, 105u, 105u }
};

#define MAKE_FN(VAR) &Paramite::VAR,

const TParamiteMotionFn sParamite_motion_table_55D5B0[44] = 
{
    PARAMITE_MOTIONS_ENUM(MAKE_FN)
};

const TParamiteAIFn sParamite_ai_table_55D710[10] =
{
    &Paramite::AI_Patrol_0_4835B0,
    &Paramite::AI_Death_1_484CD0,
    &Paramite::AI_ChasingAbe_2_4859D0,
    &Paramite::AI_SurpriseWeb_3_4851B0,
    &Paramite::AI_UNKNOWN_4_48F8F0,
    &Paramite::AI_SpottedMeat_5_486880,
    &Paramite::AI_Possessed_6_484BC0,
    &Paramite::AI_DeathDrop_7_484FF0,
    &Paramite::AI_ControlledByGameSpeak_8_48DFC0,
    &Paramite::AI_ParamiteSpawn_9_48ED80
};


const static AIFunctionData<TParamiteAIFn> sParamiteAITable[10] =
{
    { &Paramite::AI_Patrol_0_4835B0, 0x402A7C, "AI_Patrol_0" },
    { &Paramite::AI_Death_1_484CD0, 0x404223, "AI_Death_1" },
    { &Paramite::AI_ChasingAbe_2_4859D0, 0x401799, "AI_ChasingAbe_2" },
    { &Paramite::AI_SurpriseWeb_3_4851B0, 0x401645, "AI_SurpriseWeb_3" },
    { &Paramite::AI_UNKNOWN_4_48F8F0, 0x48F8F0, "AI_UNKNOWN_4" }, // Dead or unused ??
    { &Paramite::AI_SpottedMeat_5_486880, 0x4012E4, "AI_SpottedMeat_5" },
    { &Paramite::AI_Possessed_6_484BC0, 0x40187F, "AI_Possessed_6" },
    { &Paramite::AI_DeathDrop_7_484FF0, 0x4021A3, "AI_DeathDrop_7" },
    { &Paramite::AI_ControlledByGameSpeak_8_48DFC0, 0x4010B4, "AI_ControlledByGameSpeak_8" },
    { &Paramite::AI_ParamiteSpawn_9_48ED80, 0x401EE7, "AI_ParamiteSpawn_9" },
};

void Paramite::SetBrain(TParamiteAIFn fn)
{
    return ::SetBrain(fn, field_128_fn_brainState, sParamiteAITable);
}

bool Paramite::BrainIs(TParamiteAIFn fn)
{
    return ::BrainIs(fn, field_128_fn_brainState, sParamiteAITable);
}


Paramite* Paramite::ctor_4879B0(Path_Paramite* pTlv, int tlvInfo)
{
    ctor_408240(16);
    field_160 = -1;
    field_174 = 0;
    field_176 = -1;

    SetVTable(this, 0x54640C);
    field_4_typeId = Types::eParamite_96;

    field_C_objectId = tlvInfo;

    field_10_resources_array.SetAt(0,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 600, 1, 0));
    field_10_resources_array.SetAt(5,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 605, 1, 0));
    field_10_resources_array.SetAt(14, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 614, 1, 0));
    field_10_resources_array.SetAt(4,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 604, 1, 0));
    field_10_resources_array.SetAt(1,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 601, 1, 0));
    field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 615, 1, 0));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 610, 1, 0));
    field_10_resources_array.SetAt(9,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 609, 1, 0));
    
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 2034);
    Animation_Init_424E10(
        96696,
        137,
        65u,
        field_10_resources_array.ItemAt(0),
        1,
        1);

    SetTint_425600(&stru_55D73C[0], gMap_5C3030.sCurrentLevelId_5C3030);

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_110_id = -1;
    field_158 = -1;
    field_15A = -1;
    field_11C_web_id = -1;
    field_118 = -1;
    field_120_obj_id = -1;
    field_124 = -1;
    field_130_timer = 0;
    field_12C_brain_ret = 0;
    field_108_next_motion = 0;
    field_106_current_motion = 0;
    field_154 = 0;
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }

    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }

    switch (pTlv->field_12_entrace_type)
    {
    case 1:
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        SetBrain(&Paramite::AI_SurpriseWeb_3_4851B0);
        break;

    case 2:
        SetBrain(&Paramite::AI_ParamiteSpawn_9_48ED80);
        field_BC_ypos -= FP_FromInteger(20);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    case 3:
        SetBrain(&Paramite::AI_ParamiteSpawn_9_48ED80);
        field_B8_xpos -= ScaleToGridSize_4498B0(field_CC_sprite_scale);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    case 4:
        SetBrain(&Paramite::AI_ParamiteSpawn_9_48ED80);
        field_B8_xpos += ScaleToGridSize_4498B0(field_CC_sprite_scale);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    default:
        SetBrain(&Paramite::AI_Patrol_0_4835B0);
        break;
    }

    field_136_attack_delay = pTlv->field_14_attack_delay;
    field_12E_drop_delay = pTlv->field_16_drop_delay;
    field_134_meat_eating_time = pTlv->field_18_meat_eating_time;
    field_144_attack_duration = pTlv->field_1A_attack_duration;
    field_14C_id = pTlv->field_1E_id;

    field_178_flags.Set(Flags_178::eBit1, pTlv->field_20_hiss_before_attack & 1);
    field_178_flags.Clear(Flags_178::eBit2);
    field_178_flags.Set(Flags_178::eBit4, pTlv->field_22_delete_when_far_away & 1);
    field_178_flags.Clear(Flags_178::eBit5);
    field_178_flags.Set(Flags_178::eBit8, pTlv->field_24_deadly_scratch & 1);
    field_178_flags.Clear(Flags_178::eBit6);
    field_178_flags.Clear(Flags_178::eBit7);

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
        field_F8_LastLineYPos = hitY;
        field_BC_ypos = hitY;
        field_106_current_motion = 0;
        CheckForPlatform_489EA0();
    }

    field_F8_LastLineYPos = field_BC_ypos;
    field_140_tlvInfo = tlvInfo;
    MapFollowMe_408D10(TRUE);

    vStackOnObjectsOfType_425840(Types::eParamite_96);

    field_DC_bApplyShadows |= 2u;
    field_15C = field_DA_xOffset;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

BaseGameObject* Paramite::VDestructor(signed int flags)
{
    return vdtor_487F90(flags);
}

void Paramite::VUpdate()
{
    vUpdate_4871B0();
}


const int sParamiteFrameTableOffsets_55D660[44] =
{
    96696,
    96676,
    96548,
    96612,
    96792,
    96728,
    97164,
    97172,
    96828,
    96868,
    96848,
    96888,
    96904,
    96932,
    96944,
    96972,
    97192,
    97096,
    97120,
    97136,
    17032,
    17072,
    17088,
    17088,
    17088,
    17192,
    17088,
    17120,
    17152,
    17088,
    17120,
    19068,
    52312,
    52312,
    52344,
    52476,
    52428,
    52380,
    52548,
    52588,
    15628,
    8108,
    9636,
    10948
};

int CC Paramite::CreateFromSaveState_4855A0(const BYTE* pBuffer)
{
    auto pState = reinterpret_cast<const Paramite_State*>(pBuffer);
    auto pTlv = static_cast<Path_Paramite*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_3C_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 600, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("PARAMITE.BND", nullptr);
    }

    auto pParamite = alive_new<Paramite>();
    if (pParamite)
    {
        pParamite->ctor_4879B0(pTlv, pState->field_3C_tlvInfo);
    }

    if (pState->field_76_flags.Get(Paramite_State::eBit1))
    {
        sControlledCharacter_5C1B8C = pParamite;
    }

    pParamite->field_FC_pPathTLV = nullptr;
    pParamite->field_100_pCollisionLine = nullptr;

    pParamite->field_B8_xpos = pState->field_4_xpos;
    pParamite->field_BC_ypos = pState->field_8_ypos;

    pParamite->field_C4_velx = pState->field_C_velx;
    pParamite->field_C8_vely = pState->field_10_vely;

    pParamite->field_13C = pState->field_64;
    pParamite->field_C0_path_number = pState->field_14_path_number;
    pParamite->field_C2_lvl_number = pState->field_16_lvl_number;
    pParamite->field_CC_sprite_scale = pState->field_18_sprite_scale;

    pParamite->field_D0_r = pState->field_1C_r;
    pParamite->field_D2_g = pState->field_1E_g;
    pParamite->field_D4_b = pState->field_20_b;

    pParamite->field_106_current_motion = pState->field_24_current_motion;
    BYTE** ppRes = pParamite->ResBlockForMotion_488130(pParamite->field_106_current_motion);
    pParamite->field_20_animation.Set_Animation_Data_409C80(sParamiteFrameTableOffsets_55D660[pParamite->field_106_current_motion], ppRes);

    pParamite->field_20_animation.field_92_current_frame = pState->field_26_anim_current_frame;
    pParamite->field_20_animation.field_E_frame_change_counter = pState->field_28;

    pParamite->field_6_flags.Set(BaseGameObject::eDrawable, pState->field_2B_bit8 & 1);

    pParamite->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_22_flip_x & 1);
    pParamite->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2A_anim_bit4 & 1);

    auto pHeader = reinterpret_cast<AnimationHeader*>(&(pParamite->field_20_animation.field_20_ppBlock[pParamite->field_20_animation.field_18_frame_table_offset]));
    if (pParamite->field_20_animation.field_92_current_frame == pHeader->field_2_num_frames - 1)
    {
        pParamite->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pParamite->field_10C_health = pState->field_2C_health;
    pParamite->field_106_current_motion = pState->field_30_current_motion;
    pParamite->field_108_next_motion = pState->field_32_next_motion;
    pParamite->field_F8_LastLineYPos = FP_FromInteger(pState->field_34_last_line_ypos);
    pParamite->field_114_flags.Set(Flags_114::e114_Bit9);
    pParamite->field_104_collision_line_type = pState->field_36_line_type;

    pParamite->field_118 = pState->field_40_obj_id;
    pParamite->field_11C_web_id = pState->field_44_obj_id;
    pParamite->field_120_obj_id = pState->field_48_obj_id;
    pParamite->field_124 = pState->field_4C_obj_id;

    pParamite->SetBrain(sParamite_ai_table_55D710[pState->field_50_ai_idx]);

    pParamite->field_12C_brain_ret = pState->field_58_brain_ret;
    pParamite->field_130_timer = pState->field_5C_timer;
    pParamite->field_138 = pState->field_60;
    pParamite->field_13C = pState->field_64;

    pParamite->field_140_tlvInfo = pState->field_3C_tlvInfo;
    pParamite->field_148_timer = pState->field_68_timer;

    pParamite->field_14E_return_level = pState->field_6C_return_level;
    pParamite->field_150_return_path = pState->field_6E_return_path;
    pParamite->field_152_return_camera = pState->field_70_return_camera;

    pParamite->field_154 = InputObject::Command_To_Raw_45EE40(pState->field_72_input);
    pParamite->field_158 = pState->field_74;

    pParamite->field_178_flags.Set(Flags_178::eBit2, pState->field_76_flags.Get(Paramite_State::eBit2));
    pParamite->field_178_flags.Set(Flags_178::eBit3, pState->field_76_flags.Get(Paramite_State::eBit3));
    pParamite->field_178_flags.Set(Flags_178::eBit5, pState->field_76_flags.Get(Paramite_State::eBit4));
    pParamite->field_178_flags.Set(Flags_178::eBit6, pState->field_76_flags.Get(Paramite_State::eBit5));
    pParamite->field_178_flags.Set(Flags_178::eBit7, pState->field_76_flags.Get(Paramite_State::eBit6));
    pParamite->field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed, pState->field_76_flags.Get(Paramite_State::eBit7));

    return sizeof(Paramite_State);
}

static int ResolveId(int objId)
{
    BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(objId);
    if (pObj)
    {
        return pObj->field_C_objectId;
    }
    else
    {
        return -1;
    }
}

int Paramite::vGetSaveState_48F220(Paramite_State* pState)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    pState->field_0_type = Types::eParamite_96;
    pState->field_4_xpos = field_B8_xpos;
    pState->field_8_ypos = field_BC_ypos;
    pState->field_C_velx = field_C4_velx;
    pState->field_10_vely = field_C8_vely;

    pState->field_64 = field_13C;

    pState->field_14_path_number = field_C0_path_number;
    pState->field_16_lvl_number = field_C2_lvl_number;
    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_1C_r = field_D0_r;
    pState->field_1E_g = field_D2_g;
    pState->field_20_b = field_D4_b;

    pState->field_22_flip_x = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_24_current_motion = field_106_current_motion;
    pState->field_26_anim_current_frame = field_20_animation.field_92_current_frame;
    pState->field_28 = field_20_animation.field_E_frame_change_counter;
    pState->field_2B_bit8 = field_6_flags.Get(BaseGameObject::eDrawable);
    pState->field_2A_anim_bit4 = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pState->field_2C_health = field_10C_health;
    pState->field_30_current_motion = field_106_current_motion;
    pState->field_32_next_motion = field_108_next_motion;
   
    pState->field_34_last_line_ypos = FP_GetExponent(field_F8_LastLineYPos);
    if (field_100_pCollisionLine)
    {
        pState->field_36_line_type = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pState->field_36_line_type = -1;
    }

    pState->field_76_flags.Clear();
    pState->field_76_flags.Set(Paramite_State::eBit1, this == sControlledCharacter_5C1B8C);

    pState->field_3C_tlvInfo = field_140_tlvInfo;
    pState->field_40_obj_id = ResolveId(field_118);
    pState->field_44_obj_id = ResolveId(field_11C_web_id);
    pState->field_44_obj_id = ResolveId(field_120_obj_id);
    pState->field_44_obj_id = ResolveId(field_124);

    pState->field_50_ai_idx = 0;
    
    int idx = 0;
    for (auto& fn : sParamite_ai_table_55D710)
    {
        if (BrainIs(fn))
        {
            pState->field_50_ai_idx = idx;
            break;
        }
        idx++;
    }

    pState->field_58_brain_ret = field_12C_brain_ret;
    pState->field_5C_timer = field_130_timer;

    pState->field_60 = field_138;
    pState->field_64 = field_13C;

    pState->field_3C_tlvInfo = field_140_tlvInfo;
    pState->field_68_timer = field_148_timer;

    pState->field_6C_return_level = field_14E_return_level;
    pState->field_6E_return_path = field_150_return_path;
    pState->field_70_return_camera = field_152_return_camera;

    pState->field_72_input = InputObject::Raw_To_Command_45EF70(field_154);
    pState->field_74 = field_158;

    pState->field_76_flags.Set(Paramite_State::eBit2, field_178_flags.Get(Flags_178::eBit2));
    pState->field_76_flags.Set(Paramite_State::eBit3, field_178_flags.Get(Flags_178::eBit3));
    pState->field_76_flags.Set(Paramite_State::eBit5, field_178_flags.Get(Flags_178::eBit4));
    pState->field_76_flags.Set(Paramite_State::eBit6, field_178_flags.Get(Flags_178::eBit5));
    pState->field_76_flags.Set(Paramite_State::eBit7, field_178_flags.Get(Flags_178::eBit6));
    pState->field_76_flags.Set(Paramite_State::eBit7, field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed));

    return sizeof(Paramite_State);
}

const __int16 sParamite_patrol_motion_table_55D590[6] =
{
    eParamiteMotions::M_Hiss1_22_48C3E0,
    eParamiteMotions::M_Turn_4_48B180,
    eParamiteMotions::M_Hiss1_22_48C3E0,
    eParamiteMotions::M_Hiss1_22_48C3E0,
    eParamiteMotions::M_Turn_4_48B180,
    eParamiteMotions::M_Idle_0_489FB0
};

enum AI_Patrol
{
    eState0_Inactive_0 = 0,
    eState0_IdleForAbe_1 = 1,
    eState0_FearingAbe_2 = 2,
    eState0_RunningFromAbe_3 = 3,
    eState0_ApproachingAbe_4 = 4,
    eState0_StopApproachingAbe_5 = 5,
    eState0_TurningForAbe_6 = 6,
    eState0_HittingAbe_7 = 7,
    eState0_StuckToWall_8 = 8,
    eState0_Attacking_9 = 9,
    eState0_LostInvisibleAbeSight_10 = 10,
    eState0_TurningAfterLoosingSight_11 = 11,
    eState0_Idle_12 = 12,
    eState0_IdleAnimation_13 = 13,
    eState0_Turning_14 = 14,
    eState0_LookingForInvisibleAbe_15 = 15,
    eState0_Panic_16 = 16
};

enum AI_ChasingAbe
{
    eState2_Inactive_0 = 0,
    eState2_Attacking_1 = 1,
    eState2_ToWarning_2 = 2,
    eState2_Warning_3 = 3,
    eState2_CloseAttack_4 = 4,
    eState2_ToChasing_5 = 5,
    eState2_QuickAttack_6 = 6,
    eState2_Chasing_7 = 7,
    eState2_Jumping_8 = 8,
    eState2_TurningWhileChasing_9 = 9,
    eState2_Turning_10 = 10,
    eState2_Walking_11 = 11,
    eState2_WalkingToHop_12 = 12,
    eState2_Eating_13 = 13,
    eState2_Idle_14 = 14,
    eState2_Panic_15 = 15
};

enum AI_SurpriseWeb
{
    eState3_Loading_0 = 0,
    eState3_Appearing_1 = 1,
    eState3_StartAnimation_2 = 2,
    eState3_StateLoop1_3 = 3,
    eState3_StateLoop2_4 = 4
};

enum AI_SpottedMeat
{
    eState5_Idle_0 = 0,
    eState5_Running_1 = 1,
    eState5_Walking_2 = 2,
    eState5_Jumping_3 = 3,
    eState5_Turning_4 = 4,
    eState5_AttentiveToMeat_5 = 5,
    eState5_Eating_6 = 6
};

enum AI_ControlledByGameSpeak
{
    eState8_Inactive_0 = 0,
    eState8_FollowingIdle_1 = 1,
    eState8_Falling_2 = 2,
    eState8_Jumping_3 = 3,
    eState8_FollowingIdleToWalking_4 = 4,
    eState8_FollowingWalking_5 = 5,
    eState8_FollowingRunning_6 = 6,
    eState8_IdleToWorking_7 = 7,
    eState8_Working_8 = 8,
    eState8_Idle_9 = 9,
    eState8_RecievedCommand_10 = 10,
    eState8_Speaking_11 = 11
};

enum AI_ParamiteSpawn
{
    eState9_Inactive_0 = 0,
    eState9_PreSpawn_1 = 1,
    eState9_SlowDescend_2 = 2,
    eState9_SlowerDescend_3 = 3,
    eState9_DescendLoop1_4 = 4,
    eState9_DescendLoop2_5 = 5
};

__int16 Paramite::AI_Patrol_0_4835B0()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (!pObj)
    {
        field_120_obj_id = -1;
    }

    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(4, this, 0, 0);
    }
    
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    switch (field_12C_brain_ret)
    {
    case AI_Patrol::eState0_Inactive_0:
        if (sNum_CamSwappers_5C1B66 > 0)
        {
            return field_12C_brain_ret;
        }
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_Patrol::eState0_IdleForAbe_1;
    
    case AI_Patrol::eState0_IdleForAbe_1:
        return AI_Patrol_State_1(pObj);

    case AI_Patrol::eState0_FearingAbe_2:
        return AI_Patrol_State_2(pObj);

    case AI_Patrol::eState0_RunningFromAbe_3:
        return AI_Patrol_State_3(pObj);

    case AI_Patrol::eState0_ApproachingAbe_4:
        return AI_Patrol_State_4(pObj);

    case AI_Patrol::eState0_StopApproachingAbe_5:
        return AI_Patrol_State_5(pObj);
    
    case AI_Patrol::eState0_TurningForAbe_6:
        if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return field_12C_brain_ret;
        }
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_Patrol::eState0_IdleForAbe_1;
    
    case AI_Patrol::eState0_HittingAbe_7:
        if (field_106_current_motion != 4 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_12C_brain_ret;
        }
        field_108_next_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
        return AI_Patrol::eState0_StuckToWall_8;
    
    case AI_Patrol::eState0_StuckToWall_8:
        return AI_Patrol_State_8(pObj);

    case AI_Patrol::eState0_Attacking_9:
        if (field_106_current_motion != eParamiteMotions::M_RunningAttack_31_48C9E0 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return field_12C_brain_ret;
        }
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        if (pObj->field_10C_health > FP_FromInteger(0))
        {
            return AI_Patrol::eState0_IdleForAbe_1;
        }
        SetBrain(&Paramite::AI_ChasingAbe_2_4859D0);
        return AI_ChasingAbe::eState2_Inactive_0;
    
    case AI_Patrol::eState0_LostInvisibleAbeSight_10:
        if (field_130_timer > static_cast<int>(sGnFrame_5C1B84))
        {
            return field_12C_brain_ret;
        }
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_Patrol::eState0_TurningAfterLoosingSight_11;
    
    case AI_Patrol::eState0_TurningAfterLoosingSight_11:
        if (field_130_timer > static_cast<int>(sGnFrame_5C1B84))
        {
            return field_12C_brain_ret;
        }
        return AI_Patrol::eState0_IdleForAbe_1;

    case AI_Patrol::eState0_Idle_12:
        return AI_Patrol_State_12(pObj);

    case AI_Patrol::eState0_IdleAnimation_13:
        if (field_106_current_motion != eParamiteMotions::M_PreHiss_21_48C180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return field_12C_brain_ret;
        }
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        field_138 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 135);
        return AI_Patrol::eState0_Idle_12;

    case AI_Patrol::eState0_Turning_14:
        if (field_106_current_motion != 4 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_12C_brain_ret;
        }
        field_138 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 135);
        return AI_Patrol::eState0_Idle_12;

    case AI_Patrol::eState0_LookingForInvisibleAbe_15:
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        if (!field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
        {
            return field_12C_brain_ret;
        }
        return AI_Patrol::eState0_Idle_12;

    case AI_Patrol::eState0_Panic_16:
        if (!Event_Get_422C00(kEventAbeOhm))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_Patrol::eState0_Idle_12;
        }
        field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
        return field_12C_brain_ret;

    default:
        return field_12C_brain_ret;
    }
}

__int16 Paramite::AI_Patrol_State_12(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    auto pMeat = FindMeat_488930();
    if (pMeat)
    {
        SetBrain(&Paramite::AI_SpottedMeat_5_486880);
        field_118 = pMeat->field_8_object_id;
        return AI_SpottedMeat::eState5_Idle_0;
    }

    if (pObj || FindTarget_488C30())
    {
        return AI_Patrol::eState0_IdleForAbe_1;
    }

    if (Event_Get_422C00(kEventAbeOhm))
    {
        field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
        return AI_Patrol::eState0_Panic_16;
    }

    const auto pEventNoise = Event_Is_Event_In_Range_422C30(kEventNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);
    const auto pEventSpeaking = Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, field_D6_scale);

    if (sActiveHero_5C1B68 == pEventNoise || sActiveHero_5C1B68 == pEventSpeaking)
    {
        if (sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
        {
            field_108_next_motion = sParamite_patrol_motion_table_55D590[Math_RandomRange_496AB0(0, 4)];
            return AI_Patrol::eState0_LookingForInvisibleAbe_15;
        }
    }

    FP gridBlockAHead = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        gridBlockAHead = field_B8_xpos - kGridSize;
    }
    else
    {
        gridBlockAHead = kGridSize + field_B8_xpos;
    }

    auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(Types::eFleech_50, gridBlockAHead, field_BC_ypos));
    if (pFleech && vIsFacingMe_4254A0(pFleech))
    {
        field_108_next_motion = eParamiteMotions::M_Attack_43_48DB70;
        return AI_Patrol::eState0_LookingForInvisibleAbe_15;
    }

    const GameSpeakEvents lastSpeak = LastSpeak_489040();
    if (lastSpeak == GameSpeakEvents::Paramite_Howdy_48)
    {
        if (sControlledCharacter_5C1B8C->field_4_typeId == Types::eParamite_96  &&
            sControlledCharacter_5C1B8C->field_10C_health > FP_FromInteger(0) &&
            sControlledCharacter_5C1B8C->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            if (CanIAcceptAGameSpeakCommand_489160())
            {
                field_178_flags.Set(Flags_178::eBit7);
                SetBrain(&Paramite::AI_ControlledByGameSpeak_8_48DFC0);
                return AI_ControlledByGameSpeak::eState8_Inactive_0;
            }
        }
    }
    else if (lastSpeak == GameSpeakEvents::Paramite_AllYa_52)
    {
        if (sControlledCharacter_5C1B8C->field_4_typeId == Types::eParamite_96 &&
            sControlledCharacter_5C1B8C->field_10C_health > FP_FromInteger(0) &&
            sControlledCharacter_5C1B8C->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            SetBrain(&Paramite::AI_ControlledByGameSpeak_8_48DFC0);
            return AI_ControlledByGameSpeak::eState8_Inactive_0;
        }
    }

    if (vOnSameYLevel_425520(sControlledCharacter_5C1B8C))
    {
        if (sControlledCharacter_5C1B8C->field_4_typeId == Types::eParamite_96)
        {
            if (sControlledCharacter_5C1B8C->field_106_current_motion == eParamiteMotions::M_Attack_43_48DB70 ||
                sControlledCharacter_5C1B8C->field_106_current_motion == eParamiteMotions::M_RunningAttack_31_48C9E0)
            {
                auto sControlledParamite = static_cast<Paramite*>(sControlledCharacter_5C1B8C);
                if (sControlledParamite->FindTarget_488C30())
                {
                    field_120_obj_id = sControlledParamite->field_120_obj_id;
                    Sound_48F600(ParamiteSpeak::Howdy_5, 0);
                    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                    SetBrain(&Paramite::AI_ChasingAbe_2_4859D0);
                    field_148_timer = sGnFrame_5C1B84 + field_144_attack_duration;
                    return AI_ChasingAbe::eState2_Inactive_0;
                }
            }
        }
    }

    if (field_138 > static_cast<int>(sGnFrame_5C1B84))
    {
        if (!field_178_flags.Get(Flags_178::eBit6) || gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            return field_12C_brain_ret;
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
            return field_12C_brain_ret;
        }
    }

    if (Math_NextRandom() >= 6u)
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_Patrol::eState0_Turning_14;
    }
    else
    {
        field_108_next_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
        return AI_Patrol::eState0_IdleAnimation_13;
    }
}

__int16 Paramite::AI_Patrol_State_5(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (!pObj || !vOnSameYLevel_425520(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_Patrol::eState0_IdleForAbe_1;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return AI_Patrol::eState0_LostInvisibleAbeSight_10;
    }

    if (AnotherParamiteNear_4886E0())
    {
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::AI_ChasingAbe_2_4859D0);
        field_148_timer = sGnFrame_5C1B84 + field_144_attack_duration;
        return AI_ChasingAbe::eState2_Inactive_0;
    }

    if (field_C4_velx < FP_FromInteger(0))
    {
        if (HandleEnemyStopper_4893B0(-2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_Patrol::eState0_IdleForAbe_1;
        }
        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine_408E90(1, 3))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_Patrol::eState0_IdleForAbe_1;
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (HandleEnemyStopper_4893B0(2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_Patrol::eState0_IdleForAbe_1;
        }
        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine_408E90(0, 3))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_Patrol::eState0_IdleForAbe_1;
        }
    }

    if (!vIsObj_GettingNear_425420(pObj))
    {
        return field_12C_brain_ret;
    }

    if (vIsObjNearby_4253B0(kGridSize * FP_FromInteger(8), pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_Patrol::eState0_IdleForAbe_1;
    }

    if (!vIsObjNearby_4253B0(kGridSize * FP_FromInteger(10), pObj))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
    return AI_Patrol::eState0_ApproachingAbe_4;
}

__int16 Paramite::AI_Patrol_State_4(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (!pObj || !vOnSameYLevel_425520(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_Patrol::eState0_IdleForAbe_1;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return AI_Patrol::eState0_LostInvisibleAbeSight_10;
    }

    if (AnotherParamiteNear_4886E0())
    {
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::AI_ChasingAbe_2_4859D0);
        field_148_timer = sGnFrame_5C1B84 + field_144_attack_duration;
        return AI_ChasingAbe::eState2_Inactive_0;
    }

    if (field_C4_velx < FP_FromInteger(0))
    {
        if (HandleEnemyStopper_4893B0(-2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_Patrol::eState0_IdleForAbe_1;
        }

        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine_408E90(1, 2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_Patrol::eState0_IdleForAbe_1;
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (HandleEnemyStopper_4893B0(2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_Patrol::eState0_IdleForAbe_1;
        }

        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine_408E90(0, 2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_Patrol::eState0_IdleForAbe_1;
        }
    }

    if (vIsObj_GettingNear_425420(pObj) || (vIsObjNearby_4253B0(kGridSize * FP_FromInteger(10), pObj)))
    {
        if (!vIsObj_GettingNear_425420(pObj))
        {
            return field_12C_brain_ret;
        }
        if (!vIsObjNearby_4253B0(kGridSize * FP_FromInteger(8), pObj))
        {
            return field_12C_brain_ret;
        }
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_Patrol::eState0_IdleForAbe_1;
    }

    field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
    return AI_Patrol::eState0_StopApproachingAbe_5;
}

__int16 Paramite::AI_Patrol_State_3(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (!pObj || !vOnSameYLevel_425520(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_Patrol::eState0_IdleForAbe_1;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return AI_Patrol::eState0_LostInvisibleAbeSight_10;
    }

    if (field_C4_velx < FP_FromInteger(0) && (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine_408E90(1, 3)))
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_Patrol::eState0_HittingAbe_7;
    }

    if (field_C4_velx > FP_FromInteger(0) && (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine_408E90(0, 3)))
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_Patrol::eState0_HittingAbe_7;
    }

    if (AnotherParamiteNear_4886E0())
    {
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::AI_ChasingAbe_2_4859D0);
        field_148_timer = sGnFrame_5C1B84 + field_144_attack_duration;
        return AI_ChasingAbe::eState2_Inactive_0;
    }

    if (vIsObj_GettingNear_425420(pObj))
    {
        return field_12C_brain_ret;
    }

    if (vIsObjNearby_4253B0(kGridSize * FP_FromInteger(7), pObj))
    {
        return field_12C_brain_ret;
    }

    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
    return AI_Patrol::eState0_FearingAbe_2;
}

__int16 Paramite::AI_Patrol_State_8(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    auto pMeat = FindMeat_488930();
    if (pMeat)
    {
        SetBrain(&Paramite::AI_SpottedMeat_5_486880);
        field_118 = pMeat->field_8_object_id;
        return AI_SpottedMeat::eState5_Idle_0;
    }

    if (!pObj || !vOnSameYLevel_425520(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_Patrol::eState0_IdleForAbe_1;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return AI_Patrol::eState0_LostInvisibleAbeSight_10;
    }

    if (vIsObjNearby_4253B0(kGridSize * FP_FromInteger(3), pObj))
    {
        field_108_next_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
        return AI_Patrol::eState0_Attacking_9;
    }

    if (vIsObj_GettingNear_425420(pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
    }
    else
    {
        if (!vIsObjNearby_4253B0(kGridSize * FP_FromInteger(8), pObj))
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return AI_Patrol::eState0_ApproachingAbe_4;
        }
    }

    if (field_106_current_motion != eParamiteMotions::M_PreHiss_21_48C180)
    {
        return field_12C_brain_ret;
    }

    if (Math_NextRandom() >= 6u)
    {
        return field_12C_brain_ret;
    }

    field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
    return field_12C_brain_ret;
}

__int16 Paramite::AI_Patrol_State_1(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    auto pFoundMeat = FindMeat_488930();
    if (pFoundMeat)
    {
        SetBrain(&Paramite::AI_SpottedMeat_5_486880);
        field_118 = pFoundMeat->field_8_object_id;
        return AI_SpottedMeat::eState5_Idle_0;
    }

    if (!pObj || !vOnSameYLevel_425520(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        if (!FindTarget_488C30())
        {
            field_138 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 135);
            return AI_Patrol::eState0_Idle_12;
        }
        pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    }

    if (pObj && pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return AI_Patrol::eState0_LostInvisibleAbeSight_10;
    }

    if (Event_Get_422C00(kEventAbeOhm) && vIsFacingMe_4254A0(pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
        return AI_Patrol::eState0_Panic_16;
    }

    if (HandleEnemyStopper_4893B0(0))
    {
        return AI_Patrol::eState0_IdleForAbe_1;
    }

    if ((AnotherParamiteNear_4886E0() || pObj->field_10C_health <= FP_FromInteger(0)) && field_CC_sprite_scale == pObj->field_CC_sprite_scale)
    {
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::AI_ChasingAbe_2_4859D0);
        field_148_timer = sGnFrame_5C1B84 + field_144_attack_duration;
        return AI_ChasingAbe::eState2_Inactive_0;
    }

    if (vIsObjNearby_4253B0(kGridSize * FP_FromInteger(6), pObj))
    {
        if (vIsFacingMe_4254A0(pObj))
        {
            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine_408E90(1, 2))
                {
                    field_108_next_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
                    return AI_Patrol::eState0_StuckToWall_8;
                }
            }

            if (Check_IsOnEndOfLine_408E90(0, 2))
            {
                field_108_next_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
                return AI_Patrol::eState0_StuckToWall_8;
            }

            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine_408E90(1, 2))
                {
                    field_108_next_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
                    return AI_Patrol::eState0_StuckToWall_8;
                }
            }

            Sound_48F600(ParamiteSpeak::Howdy_5, 0);
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return AI_Patrol::eState0_TurningForAbe_6;
        }
        else
        {
            if (vIsObjNearby_4253B0(kGridSize * FP_FromInteger(4), pObj) == 0)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine_408E90(1, 2) ||
                    !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine_408E90(0, 2))
                {
                    field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
                    return AI_Patrol::eState0_HittingAbe_7;
                }
                else
                {
                    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
                    return AI_Patrol::eState0_FearingAbe_2;
                }
            }
            else
            {
                if ((!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) || !Check_IsOnEndOfLine_408E90(1, 2)) &&
                    (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) || !Check_IsOnEndOfLine_408E90(0, 2)))
                {
                    field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
                    return AI_Patrol::eState0_RunningFromAbe_3;
                }
                field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
                return AI_Patrol::eState0_HittingAbe_7;
            }
        }
    }
    else
    {
        if (!vIsObjNearby_4253B0(kGridSize * FP_FromInteger(8), pObj))
        {
            if (vIsFacingMe_4254A0(pObj) == 0)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine_408E90(0, 2) ||
                    !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine_408E90(1, 2))
                {
                    field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
                    return AI_Patrol::eState0_TurningForAbe_6;
                }
            }
            else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine_408E90(1, 2) ||
                    !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine_408E90(0, 2))
            {
                field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
                return AI_Patrol::eState0_ApproachingAbe_4;
            }
        }

        if (vIsFacingMe_4254A0(pObj))
        {
            return field_12C_brain_ret;
        }
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_Patrol::eState0_TurningForAbe_6;
    }
}

__int16 Paramite::AI_Patrol_State_2(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (!pObj || !vOnSameYLevel_425520(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_Patrol::eState0_IdleForAbe_1;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return AI_Patrol::eState0_LostInvisibleAbeSight_10;
    }

    if (field_C4_velx < FP_FromInteger(0) && (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine_408E90(1, 2)))
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_Patrol::eState0_HittingAbe_7;
    }
    else if (field_C4_velx > FP_FromInteger(0) && (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine_408E90(0, 2)))
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_Patrol::eState0_HittingAbe_7;
    }

    if (AnotherParamiteNear_4886E0())
    {
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::AI_ChasingAbe_2_4859D0);
        field_148_timer = sGnFrame_5C1B84 + field_144_attack_duration;
        return AI_ChasingAbe::eState2_Inactive_0;
    }

    if (vIsObj_GettingNear_425420(pObj) && vIsObjNearby_4253B0(kGridSize * FP_FromInteger(4), pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
        return AI_Patrol::eState0_RunningFromAbe_3;
    }

    if (vIsObj_GettingNear_425420(pObj))
    {
        return field_12C_brain_ret;
    }

    if (vIsObjNearby_4253B0(kGridSize * FP_FromInteger(7), pObj))
    {
        return field_12C_brain_ret;
    }

    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    return AI_Patrol::eState0_IdleForAbe_1;
}

__int16 Paramite::AI_Death_1_484CD0()
{
    if (!field_100_pCollisionLine || field_100_pCollisionLine->field_8_type == 8)
    {
        PathLine* ppPathLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        if (!InAirCollision_408810(&ppPathLine, &hitX, &hitY, FP_FromDouble(1.8)))
        {
            return 100;
        }

        switch (ppPathLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            field_100_pCollisionLine = ppPathLine;
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            CheckForPlatform_489EA0();
            break;
        default:
            break;
        }
    }

    if (field_130_timer <= static_cast<int>(sGnFrame_5C1B84) || field_130_timer >= static_cast<int>(sGnFrame_5C1B84 + 80))
    {
        if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }
    else
    {
        field_CC_sprite_scale -= FP_FromDouble(0.01);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;

        if (field_CC_sprite_scale >= FP_FromDouble(0.3) && !(static_cast<int>(sGnFrame_5C1B84) % 5))
        {
            New_Particles_426C70(
                (FP_FromInteger(Math_RandomRange_496AB0(-24, 24)) * field_CC_sprite_scale) + field_B8_xpos,
                field_BC_ypos - FP_FromInteger(6),
                field_CC_sprite_scale / FP_FromInteger(2),
                2, 128, 128, 128);
            SFX_Play_46FBA0(79, 25, FP_GetExponent(FP_FromInteger(2200) * field_CC_sprite_scale));
        }
    }
    
    if (sControlledCharacter_5C1B8C == this)
    {
        if (field_130_timer < static_cast<int>(sGnFrame_5C1B84))
        {
            sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
            gMap_5C3030.SetActiveCam_480D30(field_14E_return_level, field_150_return_path, field_152_return_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        }
    }

    if (sControlledCharacter_5C1B8C != this)
    {
        if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    if (field_CC_sprite_scale <= FP_FromInteger(0) || field_130_timer < static_cast<int>(sGnFrame_5C1B84))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    return 100;
}

__int16 Paramite::AI_ChasingAbe_2_4859D0()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (pObj && !pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible) && (pObj->field_4_typeId != Types::eFleech_50 || pObj->field_10C_health > FP_FromInteger(0)))
    {
        if (field_148_timer > static_cast<int>(sGnFrame_5C1B84) || vOnSameYLevel_425520(pObj) && field_CC_sprite_scale == pObj->field_CC_sprite_scale)
        {
            if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
            {
                MusicController::sub_47FD60(8, this, 0, 0);
            }

            if (Event_Get_422C00(kEventAbeOhm))
            {
                field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
                return AI_ChasingAbe::eState2_Panic_15;
            }

            switch (field_12C_brain_ret)
            {
            case AI_ChasingAbe::eState2_Inactive_0:  return AI_ChasingAbe_State_0(pObj);
            case AI_ChasingAbe::eState2_Attacking_1:  return AI_ChasingAbe_State_1(pObj);
            case AI_ChasingAbe::eState2_ToWarning_2:  return AI_ChasingAbe_State_2();
            case AI_ChasingAbe::eState2_Warning_3:  return AI_ChasingAbe_State_3();
            case AI_ChasingAbe::eState2_CloseAttack_4:  return AI_ChasingAbe_State_4();
            case AI_ChasingAbe::eState2_ToChasing_5:  return AI_ChasingAbe_State_5(pObj);
            case AI_ChasingAbe::eState2_QuickAttack_6:  return AI_ChasingAbe_State_6();
            case AI_ChasingAbe::eState2_Chasing_7:  return AI_ChasingAbe_State_7(pObj);
            case AI_ChasingAbe::eState2_Jumping_8:  return AI_ChasingAbe_State_8();
            case AI_ChasingAbe::eState2_TurningWhileChasing_9:  return AI_ChasingAbe_State9();
            case AI_ChasingAbe::eState2_Turning_10: return AI_ChasingAbe_State_10(pObj);
            case AI_ChasingAbe::eState2_Walking_11: return AI_ChasingAbe_State_11(pObj);
            case AI_ChasingAbe::eState2_WalkingToHop_12: return AI_ChasingAbe_State_12(pObj);
            case AI_ChasingAbe::eState2_Eating_13: return AI_ChasingAbe_State_13();
            case AI_ChasingAbe::eState2_Idle_14: return AI_ChasingAbe_State_14();
            case AI_ChasingAbe::eState2_Panic_15: return AI_ChasingAbe_State_15();
            default: return field_12C_brain_ret;
            }
        }
    }
    else
    {
        field_120_obj_id = -1;
    }

    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    SetBrain(&Paramite::AI_Patrol_0_4835B0);
    return AI_Patrol::eState0_Inactive_0;
}

__int16 Paramite::AI_ChasingAbe_State_15()
{
    if (Event_Get_422C00(kEventAbeOhm))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    return AI_ChasingAbe::eState2_Inactive_0;
}

__int16 Paramite::AI_ChasingAbe_State_14()
{
    if (field_130_timer > static_cast<int>(sGnFrame_5C1B84))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
    field_130_timer = sGnFrame_5C1B84 + 28;
    return AI_ChasingAbe::eState2_Eating_13;
}

__int16 Paramite::AI_ChasingAbe_State9()
{
    if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    return AI_ChasingAbe::eState2_ToChasing_5;
}

__int16 Paramite::AI_ChasingAbe_State_13()
{
    if (field_130_timer > static_cast<int>(sGnFrame_5C1B84))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    field_130_timer = sGnFrame_5C1B84 + 15;
    return AI_ChasingAbe::eState2_Idle_14;
}

__int16 Paramite::AI_ChasingAbe_State_8()
{
    if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
    {
        return field_12C_brain_ret;
    }
    return AI_ChasingAbe::eState2_ToChasing_5;
}

__int16 Paramite::AI_ChasingAbe_State_6()
{
    if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        return field_12C_brain_ret;
    }

    if (field_178_flags.Get(Flags_178::eBit1))
    {
        field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
    }
    return AI_ChasingAbe::eState2_ToChasing_5;
}

__int16 Paramite::AI_ChasingAbe_State_3()
{
    if (field_106_current_motion != eParamiteMotions::M_Hiss1_22_48C3E0)
    {
        return field_12C_brain_ret;
    }
    return AI_ChasingAbe::eState2_ToChasing_5;
}

__int16 Paramite::AI_ChasingAbe_State_4()
{
    if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        return field_12C_brain_ret;
    }

    if (field_178_flags.Get(Flags_178::eBit1))
    {
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(0, 6);
        return AI_ChasingAbe::eState2_ToWarning_2;
    }
    else
    {
        field_130_timer = sGnFrame_5C1B84 + field_136_attack_delay;
        return AI_ChasingAbe::eState2_ToChasing_5;
    }
}

__int16 Paramite::AI_ChasingAbe_State_2()
{
    if (field_130_timer > static_cast<int>(sGnFrame_5C1B84))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
    field_130_timer = sGnFrame_5C1B84 + field_136_attack_delay;
    return AI_ChasingAbe::eState2_Warning_3;
}

__int16 Paramite::AI_ChasingAbe_State_11(BaseAliveGameObject* pObj)
{
    if (!vIsFacingMe_4254A0(pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_ChasingAbe::eState2_Turning_10;
    }

    if (!vOnSameYLevel_425520(pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_ChasingAbe::eState2_Walking_11;
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (vIsObjNearby_4253B0(kGridSize, pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
        field_130_timer = sGnFrame_5C1B84 + 28;
        return AI_ChasingAbe::eState2_Eating_13;
    }

    const FP xSnapped = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (FP_Abs(field_B8_xpos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine_408E90(0, 1))
        {
            ToHop_489C20();
            return AI_ChasingAbe::eState2_WalkingToHop_12;
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        if (FP_Abs(field_B8_xpos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine_408E90(1, 1))
        {
            ToHop_489C20();
            return AI_ChasingAbe::eState2_WalkingToHop_12;
        }
    }
    return field_12C_brain_ret;
}

__int16 Paramite::AI_ChasingAbe_State_7(BaseAliveGameObject* pObj)
{
    if (!vOnSameYLevel_425520(pObj) || WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_ChasingAbe::eState2_Inactive_0;
    }
    else
    {
        if (!vIsFacingMe_4254A0(pObj))
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return AI_ChasingAbe::eState2_TurningWhileChasing_9;
        }

        const FP xSnapped = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
        if (field_C4_velx < FP_FromInteger(0))
        {
            if (HandleEnemyStopper_4893B0(-2))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return AI_ChasingAbe::eState2_ToChasing_5;
            }

            if (FP_Abs(field_B8_xpos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine_408E90(1, 1))
            {
                ToHop_489C20();
                return AI_ChasingAbe::eState2_Jumping_8;
            }
            else
            {
                // Drop through to idle/attack case
            }
        }
        else if (field_C4_velx > FP_FromInteger(0))
        {
            if (HandleEnemyStopper_4893B0(2))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return AI_ChasingAbe::eState2_ToChasing_5;
            }

            if (FP_Abs(field_B8_xpos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine_408E90(0, 1))
            {
                ToHop_489C20();
                return AI_ChasingAbe::eState2_Jumping_8;
            }
            else
            {
                // Drop through to idle/attack case
            }
        }

        const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

        if (!vIsObjNearby_4253B0(kGridSize * FP_FromInteger(4), pObj))
        {
            return field_12C_brain_ret;
        }

        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_ChasingAbe::eState2_ToChasing_5;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
            return AI_ChasingAbe::eState2_Attacking_1;
        }
    }
}

__int16 Paramite::AI_ChasingAbe_State_12(BaseAliveGameObject * pObj)
{
    if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
    {
        return field_12C_brain_ret;
    }

    if (vIsFacingMe_4254A0(pObj))
    {
        const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

        if (vIsObjNearby_4253B0(kGridSize, pObj))
        {
            field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
            field_130_timer = sGnFrame_5C1B84 + 28;
            return AI_ChasingAbe::eState2_Eating_13;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return AI_ChasingAbe::eState2_Walking_11;
        }
    }
    else
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_ChasingAbe::eState2_Turning_10;
    }
}

__int16 Paramite::AI_ChasingAbe_State_10(BaseAliveGameObject* pObj)
{
    if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        return field_12C_brain_ret;
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (vIsObjNearby_4253B0(kGridSize, pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
        field_130_timer = sGnFrame_5C1B84 + 28;
        return AI_ChasingAbe::eState2_Eating_13;
    }

    if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (!Check_IsOnEndOfLine_408E90(0, 1))
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return AI_ChasingAbe::eState2_Walking_11;
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (!Check_IsOnEndOfLine_408E90(1, 1))
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return AI_ChasingAbe::eState2_Walking_11;
        }
    }

    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
    return AI_ChasingAbe::eState2_Jumping_8;
}

__int16 Paramite::AI_ChasingAbe_State_5(BaseAliveGameObject* pObj)
{
    if (!vOnSameYLevel_425520(pObj))
    {
        return AI_ChasingAbe::eState2_Inactive_0;
    }

    if (!vIsFacingMe_4254A0(pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_ChasingAbe::eState2_QuickAttack_6;
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (HandleEnemyStopper_4893B0(-2))
        {
            return AI_ChasingAbe::eState2_ToChasing_5;
        }
    }
    else if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (HandleEnemyStopper_4893B0(2))
        {
            return AI_ChasingAbe::eState2_ToChasing_5;
        }
    }

    if (vIsObjNearby_4253B0(kGridSize, pObj))
    {
        field_108_next_motion = eParamiteMotions::M_CloseAttack_16_48DDA0;
        return AI_ChasingAbe::eState2_Inactive_0;
    }

    if (vIsObjNearby_4253B0(kGridSize * FP_FromInteger(4), pObj))
    {
        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_ChasingAbe::eState2_ToChasing_5;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
            return AI_ChasingAbe::eState2_Attacking_1;
        }
    }

    if (field_130_timer > static_cast<int>(sGnFrame_5C1B84) && field_178_flags.Get(Flags_178::eBit1))
    {
        return field_12C_brain_ret;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine_408E90(1, 1))
        {
            field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
            return AI_ChasingAbe::eState2_Jumping_8;
        }
    }
    else if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine_408E90(0, 1))
        {
            field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
            return AI_ChasingAbe::eState2_Jumping_8;
        }
    }

    field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
    return AI_ChasingAbe::eState2_Chasing_7;
}

__int16 Paramite::AI_ChasingAbe_State_1(BaseAliveGameObject* pObj)
{
    if (field_106_current_motion != eParamiteMotions::M_RunningAttack_31_48C9E0)
    {
        if (field_108_next_motion != eParamiteMotions::M_RunningAttack_31_48C9E0)
        {
            return AI_ChasingAbe::eState2_ToChasing_5;
        }

        if (field_106_current_motion != eParamiteMotions::M_RunningAttack_31_48C9E0)
        {
            return field_12C_brain_ret;
        }
    }

    if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        return field_12C_brain_ret;
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (pObj->field_10C_health > FP_FromInteger(0))
    {
        if (vIsFacingMe_4254A0(pObj))
        {
            if (vIsObjNearby_4253B0(kGridSize * FP_FromInteger(4), pObj))
            {
                if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                {
                    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                    return AI_ChasingAbe::eState2_ToChasing_5;
                }
                else
                {
                    field_108_next_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
                    return AI_ChasingAbe::eState2_Attacking_1;
                }
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine_408E90(1, 1))
                {
                    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                    return AI_ChasingAbe::eState2_Jumping_8;
                }

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
                    return AI_ChasingAbe::eState2_Chasing_7;
                }
            }

            if (!Check_IsOnEndOfLine_408E90(0, 1))
            {
                field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
                return AI_ChasingAbe::eState2_Chasing_7;
            }

            field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
            return AI_ChasingAbe::eState2_Jumping_8;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return AI_ChasingAbe::eState2_QuickAttack_6;
        }
    }
    else
    {
        if (vIsFacingMe_4254A0(pObj))
        {
            if (vIsObjNearby_4253B0(kGridSize, pObj))
            {
                field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
                field_130_timer = sGnFrame_5C1B84 + 28;
                return AI_ChasingAbe::eState2_Eating_13;
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine_408E90(1, 1))
                {
                    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                    return AI_ChasingAbe::eState2_Jumping_8;
                }

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
                    return AI_ChasingAbe::eState2_Walking_11;
                }
            }

            if (Check_IsOnEndOfLine_408E90(0, 1))
            {
                field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                return AI_ChasingAbe::eState2_Jumping_8;
            }

            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return AI_ChasingAbe::eState2_Walking_11;
        }
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_ChasingAbe::eState2_Turning_10;
    }
}

__int16 Paramite::AI_ChasingAbe_State_0(BaseAliveGameObject * pObj)
{
    if (!vOnSameYLevel_425520(pObj))
    {
        return AI_ChasingAbe::eState2_Inactive_0;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
    {
        return AI_ChasingAbe::eState2_Inactive_0;
    }

    if (pObj->field_10C_health > FP_FromInteger(0))
    {
        if (vIsFacingMe_4254A0(pObj))
        {
            if (field_178_flags.Get(Flags_178::eBit1))
            {
                field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(0, 6);
                return AI_ChasingAbe::eState2_ToWarning_2;
            }
            else
            {
                field_130_timer = sGnFrame_5C1B84 + field_136_attack_delay;
                return AI_ChasingAbe::eState2_ToChasing_5;
            }
        }

        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_ChasingAbe::eState2_CloseAttack_4;
    }
    else
    {
        if (vIsFacingMe_4254A0(pObj))
        {
            const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

            if (vIsObjNearby_4253B0(kGridSize, pObj))
            {
                field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
                field_130_timer = sGnFrame_5C1B84 + 28;
                return AI_ChasingAbe::eState2_Eating_13;
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine_408E90(1, 1))
                {
                    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                    return AI_ChasingAbe::eState2_Jumping_8;
                }

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
                    return AI_ChasingAbe::eState2_Walking_11;
                }
            }

            if (Check_IsOnEndOfLine_408E90(0, 1))
            {
                field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                return AI_ChasingAbe::eState2_Jumping_8;
            }

            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return AI_ChasingAbe::eState2_Walking_11;
        }

        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_ChasingAbe::eState2_Turning_10;
    }
}

__int16 Paramite::AI_SurpriseWeb_3_4851B0()
{
    auto pExistingWeb = static_cast<ParamiteWeb*>(sObjectIds_5C1B70.Find_449CF0(field_11C_web_id));
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    switch (field_12C_brain_ret)
    {
    case AI_SurpriseWeb::eState3_Loading_0:
        if (sNum_CamSwappers_5C1B66 <= 0)
        {
            field_106_current_motion = eParamiteMotions::M_Idle_0_489FB0;
            MapFollowMe_408D10(TRUE);
        }
        return AI_SurpriseWeb::eState3_Appearing_1;

    case AI_SurpriseWeb::eState3_Appearing_1:
        if (field_14C_id != 0 && SwitchStates_Get_466020(field_14C_id))
        {
            field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            field_130_timer = sGnFrame_5C1B84 + field_12E_drop_delay;
            auto pNewWeb = alive_new<ParamiteWeb>();
            if (pNewWeb)
            {
                field_11C_web_id = pNewWeb->ctor_4E1840(field_B8_xpos, FP_GetExponent(field_BC_ypos) - 20, FP_GetExponent(field_BC_ypos) - 10, field_CC_sprite_scale)->field_8_object_id;
            }
            return AI_SurpriseWeb::eState3_StartAnimation_2;
        }
        break;

    case AI_SurpriseWeb::eState3_StartAnimation_2:
        if (field_130_timer <= static_cast<int>(sGnFrame_5C1B84))
        {
            field_C8_vely = FP_FromInteger(0);
            field_106_current_motion = eParamiteMotions::M_SurpriseWeb_33_48D760;
        }
        return AI_SurpriseWeb::eState3_StateLoop1_3;

    case AI_SurpriseWeb::eState3_StateLoop1_3:
        pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(field_BC_ypos)) - 10;
        pExistingWeb->field_BC_ypos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
        if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
        {
            if (field_C8_vely < (field_CC_sprite_scale * FP_FromInteger(8)))
            {
                field_C8_vely = (field_CC_sprite_scale * FP_FromDouble(0.5)) + field_C8_vely;
                return field_12C_brain_ret;
            }
        }
        else
        {
            auto pWeb = static_cast<ParamiteWeb*>(sObjectIds_5C1B70.Find_449CF0(field_11C_web_id));
            pWeb->field_104_bEnabled = 1;
            field_11C_web_id = -1;
            SetBrain(&Paramite::AI_Patrol_0_4835B0);
            return AI_Patrol::eState0_Inactive_0;
        }
        return AI_SurpriseWeb::eState3_StateLoop2_4;

    case AI_SurpriseWeb::eState3_StateLoop2_4:
        pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(field_BC_ypos)) - 10;
        pExistingWeb->field_BC_ypos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
        if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
        {
            if (field_C8_vely <= (field_CC_sprite_scale * FP_FromInteger(-1)))
            {
                return AI_SurpriseWeb::eState3_StateLoop1_3;
            }
            else
            {
                field_C8_vely = field_C8_vely - (field_CC_sprite_scale * FP_FromInteger(1));
                return field_12C_brain_ret;
            }
        }
        else
        {
            auto pWeb = static_cast<ParamiteWeb*>(sObjectIds_5C1B70.Find_449CF0(field_11C_web_id));
            pWeb->field_104_bEnabled = TRUE;
            field_11C_web_id = -1;
            SetBrain(&Paramite::AI_Patrol_0_4835B0);
            return AI_Patrol::eState0_Inactive_0;
        }
        break;

    default:
        break;
    }

    return field_12C_brain_ret;
}

__int16 Paramite::AI_UNKNOWN_4_48F8F0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    // NOTE: Some unused code removed from OG here - looked like paramite would have
    // done a game speak before going back to partol.

    if (field_106_current_motion == eParamiteMotions::M_Turn_4_48B180)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    }
    else
    {
        field_106_current_motion = eParamiteMotions::M_Idle_0_489FB0;
        field_108_next_motion = -1;
    }

    SetBrain(&Paramite::AI_Patrol_0_4835B0);
    return AI_Patrol::eState0_Inactive_0;
}

__int16 Paramite::AI_SpottedMeat_5_486880()
{
    auto pMeat = static_cast<Meat*>(sObjectIds_5C1B70.Find_449CF0(field_118));

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (!pMeat || pMeat->VIsFalling_49E330())
    {
        Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
        field_118 = -1;
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::AI_Patrol_0_4835B0);
        return AI_Patrol::eState0_Inactive_0;
    }

    if (pMeat->field_130_pLine)
    {
        if (FP_Abs(pMeat->field_BC_ypos - field_BC_ypos) > FP_FromInteger(20))
        {
            field_118 = -1;
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            SetBrain(&Paramite::AI_Patrol_0_4835B0);
            return AI_Patrol::eState0_Inactive_0;
        }
    }

    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(4, this, 0, 0);
    }

    switch (field_12C_brain_ret)
    {
    case AI_SpottedMeat::eState5_Idle_0:  return AI_SpottedMeat_State_0(pMeat);
    case AI_SpottedMeat::eState5_Running_1:  return AI_SpottedMeat_State_1(pMeat);
    case AI_SpottedMeat::eState5_Walking_2:  return AI_SpottedMeat_State_2(pMeat);
    case AI_SpottedMeat::eState5_Jumping_3:  return AI_SpottedMeat_State_3();
    case AI_SpottedMeat::eState5_Turning_4:  return AI_SpottedMeat_State_4(pMeat);
    case AI_SpottedMeat::eState5_AttentiveToMeat_5:  return AI_SpottedMeat_State_5(pMeat);
    case AI_SpottedMeat::eState5_Eating_6:  return AI_SpottedMeat_State_6(pMeat);
    default: return field_12C_brain_ret;
    }
}

__int16 Paramite::AI_SpottedMeat_State_6(Meat* pMeat)
{
    if (!vIsObjNearby_4253B0(field_CC_sprite_scale * FP_FromInteger(40), pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
        return AI_SpottedMeat::eState5_Walking_2;
    }

    if (!vIsFacingMe_4254A0(pMeat))
    {
        if (FP_Abs(pMeat->field_B8_xpos - field_B8_xpos) > FP_FromInteger(5))
        {
            Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return AI_SpottedMeat::eState5_Turning_4;
        }
    }

    if (field_130_timer > static_cast<int>(sGnFrame_5C1B84))
    {
        return field_12C_brain_ret;
    }

    pMeat->field_6_flags.Set(BaseGameObject::eDead);
    field_118 = -1;
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    SetBrain(&Paramite::AI_Patrol_0_4835B0);
    return AI_Patrol::eState0_Inactive_0;
}

__int16 Paramite::AI_SpottedMeat_State_5(Meat* pMeat)
{
    if (!vIsFacingMe_4254A0(pMeat))
    {
        Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_SpottedMeat::eState5_Turning_4;
    }

    if (vIsObj_GettingNear_425420(pMeat))
    {
        return field_12C_brain_ret;
    }

   
    if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine_408E90(0, 1))
        {
            ToHop_489C20();
            return AI_SpottedMeat::eState5_Jumping_3;
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine_408E90(1, 1))
        {
            ToHop_489C20();
            return AI_SpottedMeat::eState5_Jumping_3;
        }
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (!vIsObjNearby_4253B0(kGridSize * FP_FromInteger(3), pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
        return AI_SpottedMeat::eState5_Running_1;
    }

    if (!vIsObjNearby_4253B0(field_CC_sprite_scale * FP_FromInteger(40), pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
        return AI_SpottedMeat::eState5_Walking_2;
    }

    if (!pMeat->field_130_pLine || !field_100_pCollisionLine)
    {
        return AI_SpottedMeat::eState5_AttentiveToMeat_5;
    }

    field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
    field_130_timer = sGnFrame_5C1B84 + field_134_meat_eating_time;
    return AI_SpottedMeat::eState5_Eating_6;
}

__int16 Paramite::AI_SpottedMeat_State_4(Meat* pMeat)
{
    if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        return field_12C_brain_ret;
    }

    if (!vIsFacingMe_4254A0(pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_SpottedMeat::eState5_Idle_0;
    }

    if (FP_Abs(pMeat->field_B8_xpos - field_B8_xpos) > FP_FromInteger(5))
    {
        Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return AI_SpottedMeat::eState5_Turning_4;
    }
    else
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_SpottedMeat::eState5_Idle_0;
    }
}

__int16 Paramite::AI_SpottedMeat_State_3()
{
    if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
    {
        return field_12C_brain_ret;
    }
    return AI_SpottedMeat::eState5_Idle_0;
}

__int16 Paramite::AI_SpottedMeat_State_2(Meat* pMeat)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const int xPos_int = FP_GetExponent(field_B8_xpos);
    const int xSnapped = SnapToXGrid_449930(field_CC_sprite_scale, xPos_int);

    if (field_C4_velx < FP_FromInteger(0))
    {
        if (std::abs(xPos_int - xSnapped) < 6 && Check_IsOnEndOfLine_408E90(1, 1))
        {
            ToHop_489C20();
            return AI_SpottedMeat::eState5_Jumping_3;
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (std::abs(xPos_int - xSnapped) < 6 && Check_IsOnEndOfLine_408E90(0, 1))
        {
            ToHop_489C20();
            return AI_SpottedMeat::eState5_Jumping_3;
        }
    }

    if (!vIsFacingMe_4254A0(pMeat))
    {
        if (FP_Abs(pMeat->field_B8_xpos - field_B8_xpos) > FP_FromInteger(5))
        {
            Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return AI_SpottedMeat::eState5_Turning_4;
        }
    }

    if (!vIsObjNearby_4253B0(field_CC_sprite_scale * FP_FromInteger(40), pMeat))
    {
        return field_12C_brain_ret;
    }

    if (pMeat->field_130_pLine)
    {
        field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
        field_130_timer = sGnFrame_5C1B84 + field_134_meat_eating_time;
        return AI_SpottedMeat::eState5_Eating_6;

    }
    return AI_SpottedMeat::eState5_Walking_2;
}

__int16 Paramite::AI_SpottedMeat_State_1(Meat* pMeat)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    const int xAsInt = FP_GetExponent(field_B8_xpos);
    const int xSnapped = SnapToXGrid_449930(field_CC_sprite_scale, xAsInt);

    if (field_C4_velx < FP_FromInteger(0))
    {
        if (std::abs(xAsInt - xSnapped) < 6 && Check_IsOnEndOfLine_408E90(1, 1))
        {
            ToHop_489C20();
            return AI_SpottedMeat::eState5_Jumping_3;
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (std::abs(xAsInt - xSnapped) < 6 && Check_IsOnEndOfLine_408E90(0, 1))
        {
            ToHop_489C20();
            return AI_SpottedMeat::eState5_Jumping_3;
        }
    }

    if (!vIsFacingMe_4254A0(pMeat))
    {
        if (FP_Abs(pMeat->field_B8_xpos - field_B8_xpos) > FP_FromInteger(5))
        {
            Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return AI_SpottedMeat::eState5_Turning_4;
        }
    }

    if (vIsObjNearby_4253B0(field_CC_sprite_scale * FP_FromInteger(40), pMeat))
    {
        if (pMeat->field_130_pLine)
        {
            field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
            field_130_timer = sGnFrame_5C1B84 + field_134_meat_eating_time;
            return AI_SpottedMeat::eState5_Eating_6;
        }
        return AI_SpottedMeat::eState5_Running_1;
    }

    if (!vIsObjNearby_4253B0(kGridSize * FP_FromInteger(3), pMeat))
    {
        return field_12C_brain_ret;
    }

    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
    return AI_SpottedMeat::eState5_Walking_2;
}

__int16 Paramite::AI_SpottedMeat_State_0(Meat* pMeat)
{
    if (!vIsFacingMe_4254A0(pMeat))
    {
        if (FP_Abs(pMeat->field_B8_xpos - field_B8_xpos) > FP_FromInteger(5))
        {
            Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return AI_SpottedMeat::eState5_Turning_4;
        }
    }

    if (vIsObj_GettingNear_425420(pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return AI_SpottedMeat::eState5_AttentiveToMeat_5;
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine_408E90(0, 1))
        {
            ToHop_489C20();
            return AI_SpottedMeat::eState5_Jumping_3;
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine_408E90(1, 1))
        {
            ToHop_489C20();
            return AI_SpottedMeat::eState5_Jumping_3;
        }
    }

    if (!vIsObjNearby_4253B0(kGridSize *FP_FromInteger(3), pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
        return AI_SpottedMeat::eState5_Running_1;
    }

    if (vIsObjNearby_4253B0(field_CC_sprite_scale * FP_FromInteger(40), pMeat))
    {
        if (field_B8_xpos == pMeat->field_B8_xpos)
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return AI_SpottedMeat::eState5_Walking_2;
        }

        if (!pMeat->field_130_pLine || !field_100_pCollisionLine)
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_SpottedMeat::eState5_AttentiveToMeat_5;
        }

        field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
        field_130_timer = sGnFrame_5C1B84 + field_134_meat_eating_time;
        return AI_SpottedMeat::eState5_Eating_6;
    }

    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
    return AI_SpottedMeat::eState5_Walking_2;
}

__int16 Paramite::AI_Possessed_6_484BC0()
{
    if (field_12C_brain_ret == 1)
    {
        if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
        {
            MusicController::sub_47FD60(9, this, 0, 0);
        }

        if (Event_Get_422C00(kEventDeathReset))
        {
            if (sControlledCharacter_5C1B8C != this)
            {
                field_6_flags.Set(BaseGameObject::eDead);
            }
        }

        if (field_10C_health <= FP_FromInteger(0))
        {
            SetBrain(&Paramite::AI_Death_1_484CD0);
            field_130_timer = sGnFrame_5C1B84 + 90;
            return field_12C_brain_ret;
        }
    }
    else if (field_12C_brain_ret == 0)
    {
        if (static_cast<int>(sGnFrame_5C1B84) >= field_130_timer)
        {
            field_12C_brain_ret = 1;
            field_10C_health = FP_FromInteger(1);
        }
    }

    return field_12C_brain_ret;
}

__int16 Paramite::AI_DeathDrop_7_484FF0()
{
    if (field_12C_brain_ret == 0)
    {
        field_130_timer = sGnFrame_5C1B84 + 60;
        return 1;
    }
    else if (field_12C_brain_ret == 1)
    {
        if (static_cast<int>(sGnFrame_5C1B84) < field_130_timer)
        {
            if (static_cast<int>(sGnFrame_5C1B84) == field_130_timer - 6)
            {
                SND_SEQ_Play_4CAB10(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
            }
            return field_12C_brain_ret;
        }

        Abe_SFX_2_457A40(15, 0, 0x7FFF, this);

        auto pScreenShake = alive_new<ScreenShake>();
        if (pScreenShake)
        {
            pScreenShake->ctor_4ACF70(0, 0);
        }
        field_130_timer = sGnFrame_5C1B84 + 30;
        return 2;
    }
    else
    {
        if (field_12C_brain_ret != 2 || static_cast<int>(sGnFrame_5C1B84) <= field_130_timer)
        {
            return field_12C_brain_ret;
        }

        if (sControlledCharacter_5C1B8C == this)
        {
            sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
            gMap_5C3030.SetActiveCam_480D30(field_14E_return_level, field_150_return_path, field_152_return_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
        }

        field_6_flags.Set(BaseGameObject::eDead);
        return field_12C_brain_ret;
    }
}

__int16 Paramite::AI_ControlledByGameSpeak_8_48DFC0()
{
    if (sControlledCharacter_5C1B8C->field_4_typeId != Types::eParamite_96 ||
        sControlledCharacter_5C1B8C->field_10C_health <= FP_FromInteger(0) ||
        sControlledCharacter_5C1B8C->field_CC_sprite_scale != field_CC_sprite_scale)
    {
        SetBrain(&Paramite::AI_Patrol_0_4835B0);
        return AI_Patrol::eState0_Inactive_0;
    }
    if (vOnSameYLevel_425520(sControlledCharacter_5C1B8C))
    {
        if (sControlledCharacter_5C1B8C->field_106_current_motion == eParamiteMotions::M_Attack_43_48DB70 ||
            sControlledCharacter_5C1B8C->field_106_current_motion == eParamiteMotions::M_RunningAttack_31_48C9E0)
        {
            if (static_cast<Paramite*>(sControlledCharacter_5C1B8C)->FindTarget_488C30())
            {
                field_120_obj_id = static_cast<Paramite*>(sControlledCharacter_5C1B8C)->field_120_obj_id;
                Sound_48F600(ParamiteSpeak::Howdy_5, 0);
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                SetBrain(&Paramite::AI_ChasingAbe_2_4859D0);
                field_148_timer = sGnFrame_5C1B84 + field_144_attack_duration;
                return AI_ChasingAbe::eState2_Inactive_0;
            }
        }
    }

    switch (field_12C_brain_ret)
    {
    case AI_ControlledByGameSpeak::eState8_Inactive_0:
    {
        field_178_flags.Clear(Flags_178::eBit7);
        __int16 result = vIsFacingMe_4254A0(sControlledCharacter_5C1B8C);
        if (result)
        {
            field_108_next_motion = eParamiteMotions::M_Empty_24_48C680;
            field_158 = 9;
            result = AI_ControlledByGameSpeak::eState8_Speaking_11;
        }
        else
        {
            if (field_106_current_motion == eParamiteMotions::M_Turn_4_48B180)
            {
                result = AI_ControlledByGameSpeak::eState8_Inactive_0;
            }
            else
            {
                field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            }
        }
        return result;
    }

    case AI_ControlledByGameSpeak::eState8_FollowingIdle_1:
        if (vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
        {
            FP gridBlock = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridBlock = field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                gridBlock = ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos;
            }

            auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(Types::eFleech_50, gridBlock, field_BC_ypos));
            if (pFleech && vIsFacingMe_4254A0(pFleech))
            {
                field_108_next_motion = eParamiteMotions::M_Attack_43_48DB70;
                field_158 = 1;
                return AI_ControlledByGameSpeak::eState8_Speaking_11;
            }

            const GameSpeakEvents lastSpeak = LastSpeak_489040();
            switch (lastSpeak)
            {
            case GameSpeakEvents::Paramite_Stay_49:
                field_15A = eParamiteMotions::M_Hiss1_22_48C3E0;
                field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                field_158 = 9;
                return AI_ControlledByGameSpeak::eState8_RecievedCommand_10;

            case GameSpeakEvents::Paramite_Howdy_48:
            case GameSpeakEvents::Paramite_AllYa_52:
                field_15A = eParamiteMotions::M_Empty_24_48C680;
                field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                field_158 = 1;
                return AI_ControlledByGameSpeak::eState8_RecievedCommand_10;

            case GameSpeakEvents::Paramite_DoIt_51:
                field_15A = eParamiteMotions::M_Hiss3_26_48C6F0;
                field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                field_158 = 7;
                return AI_ControlledByGameSpeak::eState8_RecievedCommand_10;

            default:
                if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), sControlledCharacter_5C1B8C))
                {
                    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -ScaleToGridSize_4498B0(field_CC_sprite_scale)))
                        {
                            if (!Check_IsOnEndOfLine_408E90(1, 1) && field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
                            {
                                field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                                return AI_ControlledByGameSpeak::eState8_FollowingIdleToWalking_4;
                            }
                            if (!Check_IsOnEndOfLine_408E90(1, 4) && vOnSameYLevel_425520(sControlledCharacter_5C1B8C))
                            {
                                field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                                return AI_ControlledByGameSpeak::eState8_Jumping_3;
                            }
                        }
                    }
                    else if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), ScaleToGridSize_4498B0(field_CC_sprite_scale)))
                        {
                            if (!Check_IsOnEndOfLine_408E90(0, 1) && field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
                            {
                                field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                                return AI_ControlledByGameSpeak::eState8_FollowingIdleToWalking_4;
                            }
                        }

                        if (!Check_IsOnEndOfLine_408E90(0, 4) && vOnSameYLevel_425520(sControlledCharacter_5C1B8C))
                        {
                            field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                            return AI_ControlledByGameSpeak::eState8_Jumping_3;
                        }
                    }
                }

                if (lastSpeak == GameSpeakEvents::Paramite_CMon_or_Attack_50)
                {
                    field_15A = eParamiteMotions::M_Hiss2_23_48C600;
                    field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                    field_158 = 2;
                    return AI_ControlledByGameSpeak::eState8_RecievedCommand_10;
                }
                else if (sControlledCharacter_5C1B8C->field_106_current_motion != eParamiteMotions::M_Attack_43_48DB70)
                {
                    return field_12C_brain_ret;
                }
                else
                {
                    field_15A = eParamiteMotions::M_Attack_43_48DB70;
                    field_158 = 1;
                    field_130_timer = sGnFrame_5C1B84 + 10;
                    return AI_ControlledByGameSpeak::eState8_RecievedCommand_10;
                }
                break;
            }
        }
        else
        {
            if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180)
            {
                field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            }
            return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
        }
        break;

    case AI_ControlledByGameSpeak::eState8_Falling_2:
        if (!vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
        }

        if (field_106_current_motion != eParamiteMotions::M_Walking_2_48A2D0)
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return AI_ControlledByGameSpeak::eState8_Falling_2;
        }

        if (field_C4_velx < FP_FromInteger(0) &&
            (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -ScaleToGridSize_4498B0(field_CC_sprite_scale)) ||
                Check_IsOnEndOfLine_408E90(1, 2)))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
        }
        else if (field_C4_velx > FP_FromInteger(0) &&
            (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), ScaleToGridSize_4498B0(field_CC_sprite_scale)) ||
                Check_IsOnEndOfLine_408E90(0, 2)))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
        }
        else if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), sControlledCharacter_5C1B8C))
        {
            return field_12C_brain_ret;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
        }
        break;

    case AI_ControlledByGameSpeak::eState8_Jumping_3:
        if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
        {
            return field_12C_brain_ret;
        }
        return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;

    case AI_ControlledByGameSpeak::eState8_FollowingIdleToWalking_4:
        if (field_130_timer > static_cast<int>(sGnFrame_5C1B84))
        {
            return field_12C_brain_ret;
        }
        field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
        return AI_ControlledByGameSpeak::eState8_FollowingWalking_5;

    case AI_ControlledByGameSpeak::eState8_FollowingWalking_5:
        if (!vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
        }

        if (field_C4_velx < FP_FromInteger(0))
        {
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -ScaleToGridSize_4498B0(field_CC_sprite_scale)) || Check_IsOnEndOfLine_408E90(1, 2))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
            }
        }
        else if (field_C4_velx > FP_FromInteger(0))
        {
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), ScaleToGridSize_4498B0(field_CC_sprite_scale)) || Check_IsOnEndOfLine_408E90(0, 2))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
            }
        }

        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), sControlledCharacter_5C1B8C))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
        }

        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(5), sControlledCharacter_5C1B8C))
        {
            return field_12C_brain_ret;
        }

        if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
        {
            if (Check_IsOnEndOfLine_408E90(0, 3))
            {
                return field_12C_brain_ret;
            }
        }
        else
        {
            if (Check_IsOnEndOfLine_408E90(1, 3))
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    return field_12C_brain_ret;
                }

                if (Check_IsOnEndOfLine_408E90(0, 3))
                {
                    return field_12C_brain_ret;
                }
            }
        }
        field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
        return AI_ControlledByGameSpeak::eState8_FollowingRunning_6;

    case AI_ControlledByGameSpeak::eState8_FollowingRunning_6:
        if (!vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
        }
        if (field_C4_velx < FP_FromInteger(0) && (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -ScaleToGridSize_4498B0(field_CC_sprite_scale)) || Check_IsOnEndOfLine_408E90(1, 3)))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
        }
        else
        {
            if (field_C4_velx > FP_FromInteger(0) && ((WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), ScaleToGridSize_4498B0(field_CC_sprite_scale))) || Check_IsOnEndOfLine_408E90(0, 3)))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
            }
            else
            {
                if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), sControlledCharacter_5C1B8C))
                {
                    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                    return AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
                }
                else
                {
                    if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(5), sControlledCharacter_5C1B8C))
                    {
                        return field_12C_brain_ret;
                    }
                    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
                    return AI_ControlledByGameSpeak::eState8_FollowingWalking_5;
                }
            }
        }
        break;

    case AI_ControlledByGameSpeak::eState8_IdleToWorking_7:
        if (field_106_current_motion == eParamiteMotions::M_JumpUpBegin_12_48BE40)
        {
            return AI_ControlledByGameSpeak::eState8_Working_8;
        }
        field_108_next_motion = eParamiteMotions::M_JumpUpBegin_12_48BE40;
        return AI_ControlledByGameSpeak::eState8_IdleToWorking_7;

    case AI_ControlledByGameSpeak::eState8_Working_8:
        if (field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
        {
            SetBrain(&Paramite::AI_Patrol_0_4835B0);
            return AI_Patrol::eState0_Inactive_0;
        }
        else
        {
            if (field_106_current_motion == eParamiteMotions::M_RopePull_15_48D930)
            {
                field_108_next_motion = eParamiteMotions::M_Falling_11_48B200;
            }
            return field_12C_brain_ret;
        }
        break;

    case AI_ControlledByGameSpeak::eState8_Idle_9:
        if (vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
        {
            const GameSpeakEvents lastSpeak = LastSpeak_489040();
            switch (lastSpeak)
            {
            case GameSpeakEvents::Paramite_CMon_or_Attack_50:
                field_15A = eParamiteMotions::M_Hiss2_23_48C600;
                field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                field_158 = 1;
                return AI_ControlledByGameSpeak::eState8_RecievedCommand_10;

            case GameSpeakEvents::Paramite_Stay_49:
                field_15A = eParamiteMotions::M_Hiss1_22_48C3E0;
                field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                field_158 = 9;
                return AI_ControlledByGameSpeak::eState8_RecievedCommand_10;

            case GameSpeakEvents::Paramite_Howdy_48:
            case GameSpeakEvents::Paramite_AllYa_52:
                field_15A = eParamiteMotions::M_Empty_24_48C680;
                field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                field_158 = 9;
                return AI_ControlledByGameSpeak::eState8_RecievedCommand_10;

            case GameSpeakEvents::Paramite_DoIt_51:
                field_15A = eParamiteMotions::M_Hiss3_26_48C6F0;
                field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                field_158 = 7;
                return AI_ControlledByGameSpeak::eState8_RecievedCommand_10;

            default:
                return field_12C_brain_ret;
            }
        }
        else
        {
            if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180)
            {
                field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            }
            return AI_ControlledByGameSpeak::eState8_Idle_9;
        }
        break;

    case AI_ControlledByGameSpeak::eState8_RecievedCommand_10:
        if (field_130_timer > static_cast<int>(sGnFrame_5C1B84))
        {
            return field_12C_brain_ret;
        }
        else
        {
            field_108_next_motion = field_15A;
            field_15A = -1;
        }
        return AI_ControlledByGameSpeak::eState8_Speaking_11;

    case AI_ControlledByGameSpeak::eState8_Speaking_11:
        if (field_106_current_motion == eParamiteMotions::M_Hiss2_23_48C600 ||
            field_106_current_motion == eParamiteMotions::M_Hiss1_22_48C3E0 ||
            field_106_current_motion == eParamiteMotions::M_Hiss3_26_48C6F0)
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        }

        if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
        {
            return AI_ControlledByGameSpeak::eState8_Speaking_11;
        }
        else
        {
            return field_158;
        }
        break;

    default:
        return field_12C_brain_ret;
    }
}

__int16 Paramite::AI_ParamiteSpawn_9_48ED80()
{
    auto pExistingWeb = static_cast<ParamiteWeb*>(sObjectIds_5C1B70.Find_449CF0(field_11C_web_id));
    switch (field_12C_brain_ret)
    {
    case AI_ParamiteSpawn::eState9_Inactive_0:
        if (field_14C_id)
        {
            field_12C_brain_ret = AI_ParamiteSpawn::eState9_PreSpawn_1;
        }
        else
        {
            if (field_100_pCollisionLine != nullptr)
            {
                field_12C_brain_ret = AI_ParamiteSpawn::eState9_SlowDescend_2;
            }
            else
            {
                field_12C_brain_ret = AI_ParamiteSpawn::eState9_SlowerDescend_3;
            }
        }
        break;

    case AI_ParamiteSpawn::eState9_PreSpawn_1:
        if (SwitchStates_Get_466020(field_14C_id))
        {
            field_178_flags.Set(Flags_178::eBit6);
            SFX_Play_46FA90(110u, 0);
            field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

            if (!field_100_pCollisionLine)
            {
                field_C8_vely = FP_FromInteger(0);
                field_106_current_motion = eParamiteMotions::M_SurpriseWeb_33_48D760;
                auto pWeb = alive_new<ParamiteWeb>();
                if (pWeb)
                {
                    field_11C_web_id = pWeb->ctor_4E1840(
                        field_B8_xpos, FP_GetExponent(field_BC_ypos) - 20,
                        FP_GetExponent(field_BC_ypos) - 10,
                        field_CC_sprite_scale)->field_8_object_id;
                }
                field_12C_brain_ret = AI_ParamiteSpawn::eState9_DescendLoop1_4;
            }
            else
            {
                SetBrain(&Paramite::AI_ControlledByGameSpeak_8_48DFC0);
                field_12C_brain_ret = AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
            }
        }
        else
        {
            field_12C_brain_ret = AI_ParamiteSpawn::eState9_PreSpawn_1;
        }
        break;

    case AI_ParamiteSpawn::eState9_SlowDescend_2:
        if (field_160 != pEventSystem_5BC11C->field_28_last_event_index)
        {
            field_160 = pEventSystem_5BC11C->field_28_last_event_index;
            if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Paramite_Howdy_48)
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
                SetBrain(&Paramite::AI_ControlledByGameSpeak_8_48DFC0);
                field_12C_brain_ret = AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
            }
        }
        break;

    case AI_ParamiteSpawn::eState9_SlowerDescend_3:
        if (field_160 != pEventSystem_5BC11C->field_28_last_event_index)
        {
            field_160 = pEventSystem_5BC11C->field_28_last_event_index;

            if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Paramite_Howdy_48)
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_C8_vely = FP_FromInteger(0);
                field_106_current_motion = eParamiteMotions::M_SurpriseWeb_33_48D760;
                auto pWeb = alive_new<ParamiteWeb>();
                if (pWeb)
                {
                    field_11C_web_id = pWeb->ctor_4E1840(
                        field_B8_xpos, FP_GetExponent(field_BC_ypos) - 20, 
                        FP_GetExponent(field_BC_ypos) - 10, 
                        field_CC_sprite_scale)->field_8_object_id;
                }
                field_12C_brain_ret = AI_ParamiteSpawn::eState9_DescendLoop1_4;
            }
        }
        break;

    case AI_ParamiteSpawn::eState9_DescendLoop1_4:
        pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(field_BC_ypos)) - 10;
        pExistingWeb->field_BC_ypos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
        if (field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
        {
            field_12C_brain_ret = AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
            pExistingWeb->field_104_bEnabled = 1;
            field_11C_web_id = -1;
            SetBrain(&Paramite::AI_ControlledByGameSpeak_8_48DFC0);
        }
        else
        {
            if (field_C8_vely < (field_CC_sprite_scale * FP_FromInteger(8)))
            {
                field_C8_vely = (field_CC_sprite_scale * FP_FromDouble(0.5)) + field_C8_vely;
                return field_12C_brain_ret;
            }
            field_12C_brain_ret = AI_ParamiteSpawn::eState9_DescendLoop2_5;
        }
        break;

    case AI_ParamiteSpawn::eState9_DescendLoop2_5:
        pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(field_BC_ypos)) - 10;
        pExistingWeb->field_BC_ypos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
        if (field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
        {
            field_12C_brain_ret = AI_ControlledByGameSpeak::eState8_FollowingIdle_1;
            pExistingWeb->field_104_bEnabled = 1;
            field_11C_web_id = -1;
            SetBrain(&Paramite::AI_ControlledByGameSpeak_8_48DFC0);
        }
        else
        {
            if (field_C8_vely > (field_CC_sprite_scale * FP_FromInteger(-1)))
            {
                field_C8_vely = field_C8_vely - (field_CC_sprite_scale * FP_FromInteger(1));
                return field_12C_brain_ret;
            }
            field_12C_brain_ret = AI_ParamiteSpawn::eState9_DescendLoop1_4;
        }
        break;

    default:
        break;
    }
    return field_12C_brain_ret;
}

void Paramite::M_Idle_0_489FB0()
{
    if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
    {
        ToNextMotion_4898A0();
        return;
    }

    if (Input_IsChanting_45F260() && !field_178_flags.Get(Flags_178::eBit5))
    {
        field_106_current_motion = eParamiteMotions::M_GetDepossessedBegin_29_48D9D0;
        field_138 = sGnFrame_5C1B84 + 30;
        SFX_Play_46FA90(17u, 0);
    }
    else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak1_555104))
    {
        field_106_current_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
        field_108_next_motion = eParamiteMotions::M_Empty_24_48C680;
    }
    else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak2_5550F8))
    {
        field_106_current_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
        field_108_next_motion = eParamiteMotions::M_Hiss2_23_48C600;
    }
    else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak3_555100))
    {
        field_106_current_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
        field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
    }
    else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak4_5550FC))
    {
        field_106_current_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
        field_108_next_motion = eParamiteMotions::M_Hiss3_26_48C6F0;
    }
    else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak6_555108))
    {
        field_106_current_motion = eParamiteMotions::M_AllOYaGameSpeakBegin_25_48C6A0;
    }
    else
    {
        ToNextMotion_4898A0();
    }
}

const int sWalkTable_5464BC[3] = // TODO: Convert
{
    0,
    91888,
    591491
};

void Paramite::M_WalkBegin_1_48A7B0()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = - FP_FromRaw(sWalkTable_5464BC[field_20_animation.field_92_current_frame]);
    }
    else
    {
        frameVelX = FP_FromRaw(sWalkTable_5464BC[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * frameVelX);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockBack_489BB0();
    }
    else
    {
        MoveOnLine_489CA0();
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = eParamiteMotions::M_Walking_2_48A2D0;
        }
    }
}

const int sWalkTable_546484[14] = // TODO: Convert
{
    133524,
    264052,
    292668,
    219373,
    208975,
    323594,
    204912,
    164380,
    290982,
    260097,
    190383,
    238565,
    308112,
    178444
};


void Paramite::M_Walking_2_48A2D0()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -FP_FromRaw(sWalkTable_546484[field_20_animation.field_92_current_frame]);
    }
    else
    {
        frameVelX = FP_FromRaw(sWalkTable_546484[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = field_CC_sprite_scale * frameVelX;

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockBack_489BB0();
        return;
    }
    
    MoveOnLine_489CA0();
    
    if (field_106_current_motion == eParamiteMotions::M_Walking_2_48A2D0)
    {
        field_154 |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
        switch (field_20_animation.field_92_current_frame)
        {
        case 0:
        case 7:
        {
            if (field_20_animation.field_92_current_frame == 0)
            {
                Sound_48F600(ParamiteSpeak::SlightStep_4, 0);
            }
            else
            {
                Sound_48F600(ParamiteSpeak::LoudStep_3, 0);
            }

            FP gridBlock = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridBlock = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                gridBlock = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }

            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), gridBlock * FP_FromInteger(1)))
            {
                field_106_current_motion = eParamiteMotions::M_WalkEnd_8_48A870;
                return;
            }

            if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
            {
                HandleStopWalking_48A720();
                field_154 = 0;
                return;
            }

            if (field_108_next_motion == eParamiteMotions::M_Idle_0_489FB0 ||
                field_108_next_motion == eParamiteMotions::M_GameSpeakBegin_20_48C010 ||
                field_108_next_motion == eParamiteMotions::M_Turn_4_48B180 ||
                field_108_next_motion == eParamiteMotions::M_UNKNOWN_42_48D900 ||
                field_108_next_motion == eParamiteMotions::M_Hiss1_22_48C3E0 ||
                field_108_next_motion == eParamiteMotions::M_Eating_40_48A0F0)
            {
                field_106_current_motion = eParamiteMotions::M_WalkEnd_8_48A870;
            }
            return;
        }

        case 3:
        case 10:
            if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
            {
                if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
                {
                    field_178_flags.Set(Flags_178::eBit2);
                    field_F4 = 3;
                    field_F6_anim_frame = 11;
                }

                field_154 = 0;

                if (!field_178_flags.Get(Flags_178::eBit3))
                {
                    UpdateSlurgWatchPoints_4890D0();
                    MapFollowMe_408D10(TRUE);
                    field_178_flags.Set(Flags_178::eBit3);
                }
                return;
            }

            if (field_108_next_motion == eParamiteMotions::M_Running_3_48AA00)
            {
                field_178_flags.Set(Flags_178::eBit2);
                field_F4 = 3;
                field_F6_anim_frame = 11;
                field_108_next_motion = -1;
                if (!field_178_flags.Get(Flags_178::eBit3))
                {
                    UpdateSlurgWatchPoints_4890D0();
                    MapFollowMe_408D10(TRUE);
                    field_178_flags.Set(Flags_178::eBit3);
                }
                return;
            }
            else if (field_108_next_motion == eParamiteMotions::M_RunningAttack_31_48C9E0)
            {
                field_106_current_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
                field_108_next_motion = -1;

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                }
                else
                {
                    field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                }
            }

            if (!field_178_flags.Get(Flags_178::eBit3))
            {
                UpdateSlurgWatchPoints_4890D0();
                MapFollowMe_408D10(TRUE);
                field_178_flags.Set(Flags_178::eBit3);
            }
            return;

        default:
            field_178_flags.Clear(Flags_178::eBit3);
            return;
        }
    }
}

const int sRunningTable_5464E8[14] = // TODO: Convert
{
    338545,
    368618,
    218134,
    231502,
    344828,
    685751,
    647332,
    534050,
    543610,
    470669,
    445321,
    585377,
    751093,
    388423
};



void Paramite::M_Running_3_48AA00()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -FP_FromRaw(sRunningTable_5464E8[field_20_animation.field_92_current_frame]);
    }
    else
    {
        frameVelX = FP_FromRaw(sRunningTable_5464E8[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * frameVelX);
    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockBack_489BB0();
        return;
    }
    
    MoveOnLine_489CA0();
    
    if (field_106_current_motion == eParamiteMotions::M_Running_3_48AA00)
    {
        field_154 = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held | field_154;

        if (field_20_animation.field_92_current_frame == 3 || field_20_animation.field_92_current_frame == 10)
        {
            if (field_20_animation.field_92_current_frame == 3)
            {
                SFX_Play_46FBA0(28u, 45, -600);
            }
            else
            {
                Sound_48F600(ParamiteSpeak::LoudStep_3, 0);
                Abe_SFX_2_457A40(6, 50, 600, 0);
            }

            if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
            {
                if (field_108_next_motion != eParamiteMotions::M_Walking_2_48A2D0)
                {
                    if (field_108_next_motion != eParamiteMotions::M_Idle_0_489FB0 &&
                        field_108_next_motion != eParamiteMotions::M_Turn_4_48B180 &&
                        field_108_next_motion != eParamiteMotions::M_Eating_40_48A0F0 &&
                        field_108_next_motion != eParamiteMotions::M_UNKNOWN_42_48D900 &&
                        field_108_next_motion != eParamiteMotions::M_Hiss1_22_48C3E0)
                    {
                        if (field_108_next_motion == eParamiteMotions::M_RunningAttack_31_48C9E0)
                        {
                            field_106_current_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
                            field_108_next_motion = -1;

                            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                            {
                                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                            }
                            else
                            {
                                field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
                            }
                        }
                        else
                        {
                            if (field_108_next_motion == eParamiteMotions::M_Hop_5_48B5B0)
                            {
                                ToHop_489C20();
                            }
                        }
                    }
                    else
                    {
                        field_106_current_motion = eParamiteMotions::M_RunEnd_10_48B000;
                    }
                }
                else
                {
                    field_106_current_motion = eParamiteMotions::M_WalkRunTransition_7_48B0C0;
                    field_108_next_motion = -1;
                }

                if (!field_178_flags.Get(Flags_178::eBit3))
                {
                    UpdateSlurgWatchPoints_4890D0();
                    MapFollowMe_408D10(TRUE);
                    field_178_flags.Set(Flags_178::eBit3);
                }
            }
            else
            {
                HandleInputRunning_48ADB0();
                field_154 = 0;
            }

            if (!field_178_flags.Get(Flags_178::eBit3))
            {
                UpdateSlurgWatchPoints_4890D0();
                MapFollowMe_408D10(TRUE);
                field_178_flags.Set(Flags_178::eBit3);
            }
            return;
        }

        if (field_20_animation.field_92_current_frame != 6 && field_20_animation.field_92_current_frame != 13)
        {
            field_178_flags.Clear(Flags_178::eBit3);
            return;
        }

        if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
        {
            if (field_154 & sInputKey_Hop_5550E0)
            {
                field_154 = 0;
                ToHop_489C20();
            }
        }

        if (!field_178_flags.Get(Flags_178::eBit3))
        {
            UpdateSlurgWatchPoints_4890D0();
            MapFollowMe_408D10(TRUE);
            field_178_flags.Set(Flags_178::eBit3);
            return;
        }
    }
}

void Paramite::M_Turn_4_48B180()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sound_48F600(ParamiteSpeak::SlightStep_4, 0);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        
        if (ToNextMotion_4898A0())
        {
            field_20_animation.Set_Animation_Data_409C80(field_20_animation.field_18_frame_table_offset, nullptr);
        }
        else
        {
            ToIdle_489B70();
        }
    }
}

const int sHopTable_546544[27] = // TODO: Convert
{
    424574,
    455634,
    300215,
    314017,
    431064,
    795042,
    731621,
    421866,
    423200,
    352426,
    387779,
    483797,
    439521,
    593985,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

void Paramite::M_Hop_5_48B5B0()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -FP_FromRaw(sHopTable_546544[field_20_animation.field_92_current_frame]);
    }
    else
    {
        frameVelX = FP_FromRaw(sHopTable_546544[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * frameVelX);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockBack_489BB0();
    }
    else
    {
        field_100_pCollisionLine = WebCollision_4888A0(FP_FromInteger(0), field_C4_velx);
        if (field_100_pCollisionLine)
        {
            field_C4_velx = FP_FromInteger(0);
            field_106_current_motion = eParamiteMotions::M_WebGrab_38_48D6C0;
            auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(Types::eWebLine_146, field_B8_xpos, field_BC_ypos));
            if (pWeb)
            {
                pWeb->Wobble_4E29D0(FP_GetExponent(field_BC_ypos));
            }
        }
        else
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            const __int16 bHit = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(0.9));
            
            if (sControlledCharacter_5C1B8C == this)
            {
                sub_408C40();
            }

            if (bHit)
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
                    ToIdle_489B70();
                    CheckForPlatform_489EA0();
                    UpdateSlurgWatchPoints_4890D0();
                    return;

                case 1u:
                case 2u:
                    field_B8_xpos = hitX - field_C4_velx;
                    MapFollowMe_408D10(TRUE);
                    field_C4_velx = FP_FromInteger(0);
                    field_BC_ypos = hitY;
                    return;

                default:
                    break;
                }
            }

            if (field_BC_ypos - field_F8_LastLineYPos > FP_FromInteger(5))
            {
                field_13C = FP_FromDouble(0.75);
                field_106_current_motion = eParamiteMotions::M_Falling_11_48B200;
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_C4_velx = (field_CC_sprite_scale * FP_FromInteger(-5));
                }
                else
                {
                    field_C4_velx = (field_CC_sprite_scale * FP_FromInteger(5));
                }
            }

            if (field_20_animation.field_92_current_frame == 3)
            {
                SFX_Play_46FBA0(28u, 50, -500);
            }
            else if (field_20_animation.field_92_current_frame == 12)
            {
                Sound_48F600(ParamiteSpeak::LoudStep_3, 0);
                Abe_SFX_2_457A40(6, 55, 500, 0);
            }
        }
    }
}

const int dword_5464D4[5] =
{
    147603,
    369009,
    479711,
    479711,
    369009
};

void Paramite::M_UNKNOWN_6_48A930()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -FP_FromRaw(dword_5464D4[field_20_animation.field_92_current_frame]);
    }
    else
    {
        frameVelX = FP_FromRaw(dword_5464D4[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * frameVelX);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_178_flags.Set(Flags_178::eBit2);
        field_F4 = 3;
        field_F6_anim_frame = 2;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockBack_489BB0();
    }
    else
    {
        MoveOnLine_489CA0();
    }
}

const int sWalkToRunTable_546538[3] = // TODO: Convert
{
    353663,
    353662,
    353664
};

void Paramite::M_WalkRunTransition_7_48B0C0()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -FP_FromRaw(sWalkToRunTable_546538[field_20_animation.field_92_current_frame]);
    }
    else
    {
        frameVelX = FP_FromRaw(sWalkToRunTable_546538[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * frameVelX);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eParamiteMotions::M_Walking_2_48A2D0;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockBack_489BB0();
    }
    else
    {
        MoveOnLine_489CA0();
    }
}

const int sWalkEndTable_5464C8[3] = // TODO: Convert
{ 
    153033,
    330008,
    439196 
};

void Paramite::M_WalkEnd_8_48A870()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -FP_FromRaw(sWalkEndTable_5464C8[field_20_animation.field_92_current_frame]);
    }
    else
    {
        frameVelX = FP_FromRaw(sWalkEndTable_5464C8[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * frameVelX);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockBack_489BB0();
    }
    else
    {
        MoveOnLine_489CA0();
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            ToIdle_489B70();
        }
    }
}

const int sRunBeginTable_546520[3] = // TODO: Convert
{
    123139,
    141508,
    218262
};

void Paramite::M_RunBegin_9_48AF10()
{
    field_154 |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = (field_CC_sprite_scale * -FP_FromRaw(sRunBeginTable_546520[field_20_animation.field_92_current_frame]));
    }
    else
    {
        field_C4_velx = (field_CC_sprite_scale * FP_FromRaw(sRunBeginTable_546520[field_20_animation.field_92_current_frame]));
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_178_flags.Set(Flags_178::eBit2);
        field_F4 = 3;
        field_F6_anim_frame = 2;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockBack_489BB0();
    }
    else
    {
        MoveOnLine_489CA0();
    }
}

const int sRunEndTable_54652C[3] = // TODO: Convert
{
    203900,
    200766,
    224956
};

void Paramite::M_RunEnd_10_48B000()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -FP_FromRaw(sRunEndTable_54652C[field_20_animation.field_92_current_frame]);
    }
    else
    {
        frameVelX = FP_FromRaw(sRunEndTable_54652C[field_20_animation.field_92_current_frame]);
    }

    field_C4_velx = (field_CC_sprite_scale * frameVelX);
    
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_489B70();
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockBack_489BB0();
    }
    else
    {
        MoveOnLine_489CA0();
    }
}

void Paramite::M_Falling_11_48B200()
{
    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx = field_C4_velx - (field_CC_sprite_scale * field_13C);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx = (field_CC_sprite_scale * field_13C) + field_C4_velx;;
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    field_100_pCollisionLine = WebCollision_4888A0(FP_FromInteger(0), field_C4_velx);
    if (field_100_pCollisionLine)
    {
        field_C4_velx = FP_FromInteger(0);
        field_106_current_motion = eParamiteMotions::M_WebGrab_38_48D6C0;
        auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(Types::eWebLine_146, field_B8_xpos, field_BC_ypos));
        if (pWeb)
        {
            pWeb->Wobble_4E29D0(FP_GetExponent(field_BC_ypos));
        }
    }
    else
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        const __int16 bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

        if (sControlledCharacter_5C1B8C == this)
        {
            sub_408C40();
        }

        if (bCollision)
        {
            switch (pLine->field_8_type)
            {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                field_100_pCollisionLine = pLine;
                field_BC_ypos = hitY;
                field_B8_xpos = hitX;
                field_106_current_motion = eParamiteMotions::M_Landing_17_48B590;

                CheckForPlatform_489EA0();
                UpdateSlurgWatchPoints_4890D0();

                if (field_BC_ypos - field_F8_LastLineYPos > (field_CC_sprite_scale * FP_FromInteger(180)))
                {
                    Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
                    field_10C_health = FP_FromInteger(0);
                    SetBrain(&Paramite::AI_Death_1_484CD0);
                    field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
                    field_130_timer = sGnFrame_5C1B84 + 90;

                    auto pBlood = alive_new<Blood>();
                    if (pBlood)
                    {
                        pBlood->ctor_40F0B0(field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 50);
                    }
                }
                break;

            case 1u:
            case 2u:
            case 5u:
            case 6u:
                field_B8_xpos = hitX - field_C4_velx;
                field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
                field_BC_ypos = hitY;
                field_C4_velx = FP_FromInteger(0);
                MapFollowMe_408D10(TRUE);
                break;
            default:
                return;
            }
        }
    }
}

void Paramite::M_JumpUpBegin_12_48BE40()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_F8_LastLineYPos = field_BC_ypos;
        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(-10) * field_CC_sprite_scale;
        field_BC_ypos += field_C8_vely;
        VOnTrapDoorOpen();
        field_106_current_motion = eParamiteMotions::M_JumpUpMidair_13_48BAF0;
        field_100_pCollisionLine = nullptr;
    }
    else if (field_20_animation.field_92_current_frame == 3)
    {
        SFX_Play_46FBA0(28u, 50, -500);
    }
}

void Paramite::M_JumpUpMidair_13_48BAF0()
{
    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        ToKnockBack_489BB0();
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* ppPathLine = nullptr;
    const __int16 bHit = InAirCollision_408810(&ppPathLine, &hitX, &hitY, FP_FromDouble(1.8));
    
    if (sControlledCharacter_5C1B8C == this)
    {
        sub_408C40();
    }

    if (bHit)
    {
        switch (ppPathLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            field_100_pCollisionLine = ppPathLine;
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            ToIdle_489B70();
            CheckForPlatform_489EA0();
            UpdateSlurgWatchPoints_4890D0();
            field_106_current_motion = eParamiteMotions::M_JumpUpLand_14_48BF00;
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
            return;

        case 3u:
        case 7u:
            if (field_C8_vely < FP_FromInteger(0))
            {
                field_C8_vely = FP_FromInteger(0);
                field_BC_ypos = hitY;
            }
            break;

        default:
            break;
        }
    }
    else
    {
        PullRingRope* pRope = FindPullRope_488F20();
        if (pRope)
        {
            if (pRope->VPull_49BBD0(this))
            {
                field_106_current_motion = eParamiteMotions::M_RopePull_15_48D930;
                field_124 = pRope->field_8_object_id;
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return;
            }
            field_124 = -1;
        }

        if (sControlledCharacter_5C1B8C == this)
        {
            FP gridBlock = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                gridBlock = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                gridBlock = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }


            if (field_C8_vely > FP_FromInteger(0))
            {
                field_100_pCollisionLine = WebCollision_4888A0(FP_FromInteger(0), gridBlock);
                if (field_100_pCollisionLine)
                {
                    field_C4_velx = FP_FromInteger(0);
                    field_106_current_motion = eParamiteMotions::M_WebGrab_38_48D6C0;

                    auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(Types::eWebLine_146, field_B8_xpos, field_BC_ypos));
                    if (pWeb)
                    {
                        pWeb->Wobble_4E29D0(FP_GetExponent(field_BC_ypos));
                    }
                    // Don't try to go to falling
                    return;
                }
            }
            else if (FP_Abs(field_C8_vely) < FP_FromInteger(3))
            {
                auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(Types::eFleech_50, field_B8_xpos, field_BC_ypos - FP_FromInteger(20)));
                if (pFleech)
                {
                    pFleech->VTakeDamage_408730(this);
                    pFleech->field_6_flags.Set(BaseGameObject::eDead);
                }
            }

        }
    }

    if ((field_BC_ypos - field_F8_LastLineYPos) > FP_FromInteger(5))
    {
        field_13C = FP_FromDouble(0.75);
        field_106_current_motion = eParamiteMotions::M_Falling_11_48B200;
    }
}

void Paramite::M_JumpUpLand_14_48BF00()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_489B70();
    }
    else if (field_20_animation.field_92_current_frame == 1)
    {
        Sound_48F600(ParamiteSpeak::LoudStep_3, 0);
        Abe_SFX_2_457A40(6, 55, 500, 0);
    }
}

void Paramite::M_RopePull_15_48D930()
{
    auto pPullRingRope = static_cast<PullRingRope*>(sObjectIds_5C1B70.Find_449CF0(field_124));
    if (!pPullRingRope || 
        ((pPullRingRope && pPullRingRope->Vsub_49BC90()) && 
        (sControlledCharacter_5C1B8C == this || field_108_next_motion == eParamiteMotions::M_Falling_11_48B200)))
    {
        if (pPullRingRope)
        {
            pPullRingRope->Vsub_49B610();
        }
        field_124 = -1;
        field_C8_vely = FP_FromInteger(0);
        field_106_current_motion = eParamiteMotions::M_JumpUpMidair_13_48BAF0;
        field_108_next_motion = -1;
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }

}

void Paramite::M_CloseAttack_16_48DDA0()
{

    if (field_20_animation.field_92_current_frame == 10)
    {
        Sound_48F600(ParamiteSpeak::CMon_or_Attack_0, 0);

        BaseAliveGameObject* pObj = nullptr;
        if (sControlledCharacter_5C1B8C == this)
        {
            if (FindTarget_488C30())
            {
                pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
                field_120_obj_id = -1;
            }
        }
        else
        {
            pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
        }

        if (pObj)
        {
            PSX_RECT otherRect = {};
            pObj->vGetBoundingRect_424FD0(&otherRect, 1);

            PSX_RECT ourRect = {};
            vGetBoundingRect_424FD0(&ourRect, 1);

            short right = 0;
            short left = 0;
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                right = ourRect.x;
                left = (ourRect.x + ourRect.w) / 2;
            }
            else
            {
                left = ourRect.w;
                right = (ourRect.x + ourRect.w) / 2;
            }

            if (otherRect.x <= left &&
                otherRect.w >= right &&
                otherRect.h >= ourRect.y &&
                otherRect.y <= ourRect.h)
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage_408730(this);
                    }
                }
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_489B70();
    }
}

void Paramite::M_Landing_17_48B590()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_489B70();
    }
}

void Paramite::M_UNKNOWN_18_48DF60()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_489B70();
    }
}

void Paramite::M_Knockback_19_48BF50()
{
    if (!field_100_pCollisionLine)
    {
        M_Falling_11_48B200();
        if (field_106_current_motion == eParamiteMotions::M_Death_41_48D8E0 || field_106_current_motion == eParamiteMotions::M_Landing_17_48B590)
        {
            field_106_current_motion = eParamiteMotions::M_Knockback_19_48BF50;
            Abe_SFX_2_457A40(6, 80, -200, this);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_100_pCollisionLine || !field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                ToIdle_489B70();
            }
            else
            {
                field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
                SetBrain(&Paramite::AI_Death_1_484CD0);
                field_130_timer = sGnFrame_5C1B84 + 90;
            }
        }
    }
}

void Paramite::M_GameSpeakBegin_20_48C010()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->vGetBoundingRect_424FD0(&otherRect, 1);

        PSX_RECT ourRect = {};
        vGetBoundingRect_424FD0(&ourRect, 1);

        if (otherRect.x <= ourRect.w &&
            otherRect.w >= ourRect.x &&
            otherRect.h >= ourRect.y &&
            otherRect.y <= ourRect.h)
        {
            if (vIsObj_GettingNear_425420(pObj))
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        if (pObj->field_10C_health > FP_FromInteger(0))
                        {
                            SFX_Play_46FA90(64u, 0);
                            Abe_SFX_457EC0(9u, 0, 0, pObj);
                        }
                        pObj->VTakeDamage_408730(this);
                    }
                }
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eParamiteMotions::M_PreHiss_21_48C180;
    }
}

void Paramite::M_PreHiss_21_48C180()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->vGetBoundingRect_424FD0(&otherRect, 1);

        PSX_RECT ourRect = {};
        vGetBoundingRect_424FD0(&ourRect, 1);

        if (otherRect.x <= ourRect.w &&
            otherRect.w >= ourRect.x &&
            otherRect.h >= ourRect.y &&
            otherRect.y <= ourRect.h)
        {
            if (vIsObj_GettingNear_425420(pObj))
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage_408730(this);
                    }
                }
            }
        }
    }

    switch (field_108_next_motion)
    {
    case eParamiteMotions::M_Hiss1_22_48C3E0:
    case eParamiteMotions::M_Hiss2_23_48C600:
    case eParamiteMotions::M_Hiss3_26_48C6F0:
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
        break;

    case eParamiteMotions::M_Empty_24_48C680:
        field_106_current_motion = eParamiteMotions::M_GameSpeakEnd_28_48C8B0;
        field_108_next_motion = -1;
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);

        if (sControlledCharacter_5C1B8C == this)
        {
            pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Paramite_Howdy_48);
        }
        break;

    case eParamiteMotions::M_RunningAttack_31_48C9E0:
        field_106_current_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
        field_108_next_motion = -1;

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
        }
        else
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
        }

        break;

    default:
        if (field_108_next_motion != -1 || sControlledCharacter_5C1B8C == this)
        {
            field_106_current_motion = eParamiteMotions::M_GameSpeakEnd_28_48C8B0;
        }
        break;
    }
}

void Paramite::M_Hiss1_22_48C3E0()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->vGetBoundingRect_424FD0(&otherRect, 1);

        PSX_RECT ourRect = {};
        vGetBoundingRect_424FD0(&ourRect, 1);

        if (otherRect.x <= ourRect.w &&
            otherRect.w >= ourRect.x &&
            otherRect.h >= ourRect.y &&
            otherRect.y <= ourRect.h)
        {
            if (vIsObj_GettingNear_425420(pObj))
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage_408730(this);
                    }
                }
            }
        }
    }

    if (field_20_animation.field_92_current_frame != 2 || field_178_flags.Get(Flags_178::eBit3))
    {
        field_178_flags.Clear(Flags_178::eBit3);
    }
    else
    {
        field_178_flags.Set(Flags_178::eBit3);
        Event_Broadcast_422BC0(kEventLoudNoise, this);
        Sound_48F600(ParamiteSpeak::Stay_1, 0);

        if (sControlledCharacter_5C1B8C == this)
        {
            pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Paramite_Stay_49);
        }
    }

    if (field_108_next_motion == eParamiteMotions::M_RunningAttack_31_48C9E0)
    {
        field_106_current_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
        field_108_next_motion = -1;

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
        }
        else
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eParamiteMotions::M_PostHiss_27_48C780;
    }
}

void Paramite::M_Hiss2_23_48C600()
{
    if (field_20_animation.field_92_current_frame != 2 || field_178_flags.Get(Flags_178::eBit3))
    {
        field_178_flags.Clear(Flags_178::eBit3);
    }
    else
    {
        field_178_flags.Set(Flags_178::eBit3);
        Sound_48F600(ParamiteSpeak::CMon_or_Attack_0, 0);
        if (sControlledCharacter_5C1B8C == this)
        {
            pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Paramite_CMon_or_Attack_50);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eParamiteMotions::M_PostHiss_27_48C780;
    }
}

void Paramite::M_Empty_24_48C680()
{
    // Empty
}

void Paramite::M_AllOYaGameSpeakBegin_25_48C6A0()
{
    if (field_20_animation.field_92_current_frame == 7)
    {
        Sound_48F600(ParamiteSpeak::AllYa_9, 0);
        if (sControlledCharacter_5C1B8C == this)
        {
            pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Paramite_AllYa_52);
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eParamiteMotions::M_GameSpeakEnd_28_48C8B0;
    }
}

void Paramite::M_Hiss3_26_48C6F0()
{
    if (field_20_animation.field_92_current_frame != 2 || field_178_flags.Get(Flags_178::eBit3))
    {
        field_178_flags.Clear(Flags_178::eBit3);
    }
    else
    {
        field_178_flags.Set(Flags_178::eBit3);
        Event_Broadcast_422BC0(kEventLoudNoise, this);
        Sound_48F600(ParamiteSpeak::DoIt_2, 0);
        if (sControlledCharacter_5C1B8C == this)
        {
            pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Paramite_DoIt_51);
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eParamiteMotions::M_PostHiss_27_48C780;
    }
}

void Paramite::M_PostHiss_27_48C780()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->vGetBoundingRect_424FD0(&otherRect, 1);

        PSX_RECT ourRect = {};
        vGetBoundingRect_424FD0(&ourRect, 1);

        if (otherRect.x <= ourRect.w &&
            otherRect.w >= ourRect.x &&
            otherRect.h >= ourRect.y &&
            otherRect.y <= ourRect.h)
        {
            if (vIsObj_GettingNear_425420(pObj))
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage_408730(this);
                    }
                }
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eParamiteMotions::M_PreHiss_21_48C180;
    }
}

void Paramite::M_GameSpeakEnd_28_48C8B0()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->vGetBoundingRect_424FD0(&otherRect, 1);

        PSX_RECT ourRect = {};
        vGetBoundingRect_424FD0(&ourRect, 1);

        if (otherRect.x <= ourRect.w &&
            otherRect.w >= ourRect.x &&
            otherRect.h >= ourRect.y &&
            otherRect.y <= ourRect.h)
        {
            if (vIsObj_GettingNear_425420(pObj))
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage_408730(this);
                    }
                }
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextMotion_4898A0())
        {
            ToIdle_489B70();
        }
    }
}

void Paramite::M_GetDepossessedBegin_29_48D9D0()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (!Input_IsChanting_45F260())
            {
                field_106_current_motion = eParamiteMotions::M_GetDepossessedEnd_30_48DB50;
            }
        }

        if (!(static_cast<int>(sGnFrame_5C1B84) % 4))
        {
            New_Chant_Particle_426BE0(
                (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange_496AB0(-20, 20))) + field_B8_xpos,
                field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange_496AB0(20, 50))), 
                field_CC_sprite_scale, 0);
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_138)
        {
            sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
            field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
            field_106_current_motion = eParamiteMotions::M_GetDepossessedEnd_30_48DB50;
            SetBrain(&Paramite::AI_Patrol_0_4835B0);
            field_12C_brain_ret = 0;
            gMap_5C3030.SetActiveCam_480D30(field_14E_return_level, field_150_return_path, field_152_return_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            if (field_152_return_camera != gMap_5C3030.sCurrentCamId_5C3034)
            {
                if (field_178_flags.Get(Flags_178::eBit6))
                {
                    field_6_flags.Set(BaseGameObject::eDead);
                }
            }
        }
    }
}

void Paramite::M_GetDepossessedEnd_30_48DB50()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_489B70();
    }
}

void Paramite::M_RunningAttack_31_48C9E0()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx))
    {
        field_100_pCollisionLine = nullptr;
        field_F8_LastLineYPos = field_BC_ypos;
        ToKnockBack_489BB0();
        return;
    }
    else if (sControlledCharacter_5C1B8C == this)
    {
        auto pWebLine = WebCollision_4888A0(FP_FromInteger(0), field_C4_velx);
        if (pWebLine)
        {
            field_C4_velx = FP_FromInteger(0);
            field_106_current_motion = eParamiteMotions::M_WebGrab_38_48D6C0;
            field_100_pCollisionLine = pWebLine;
            return;
        }
    }

    MoveOnLine_489CA0();

    if (field_106_current_motion == eParamiteMotions::M_RunningAttack_31_48C9E0)
    {
        if (sControlledCharacter_5C1B8C == this)
        {
            sub_408C40();
            if (FindTarget_488C30())
            {
                pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
                field_120_obj_id = -1;
            }
        }

        if (pObj)
        {
            PSX_RECT otherRect = {};
            pObj->vGetBoundingRect_424FD0(&otherRect, 1);

            PSX_RECT ourRect = {};
            vGetBoundingRect_424FD0(&ourRect, 1);

            short right = 0;
            short left = 0;
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                right = ourRect.x;
                left = (ourRect.x + ourRect.w) / 2;
            }
            else
            {
                left = ourRect.w;
                right = (ourRect.x + ourRect.w) / 2;
            }

            if (otherRect.x <= left &&
                otherRect.w >= right &&
                otherRect.h >= ourRect.y &&
                otherRect.y <= ourRect.h)
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage_408730(this);
                    }
                }
            }
        }

        if (field_20_animation.field_92_current_frame == 3)
        {
            Sound_48F600(ParamiteSpeak::CMon_or_Attack_0, 0);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            ToIdle_489B70();
        }
    }
}

void Paramite::M_Empty_32_48D740()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_SurpriseWeb_33_48D760()
{
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_C8_vely + field_BC_ypos,
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
        field_F8_LastLineYPos = hitY;
        field_106_current_motion = eParamiteMotions::M_WebLeaveDown_34_48D870;
        if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
        {
            CheckForPlatform_489EA0();
        }
    }
    else
    {
        field_BC_ypos += field_C8_vely;
    }

    if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 3)
    {
        Sound_48F600(ParamiteSpeak::ClimbingWeb_6, 0);
    }
}

void Paramite::M_WebLeaveDown_34_48D870()
{
    if (field_20_animation.field_92_current_frame == 2)
    {
        Abe_SFX_2_457A40(6, 50, 600, 0);
        field_DA_xOffset = field_15C;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_489B70();
    }
}

void Paramite::M_WebIdle_35_48D400()
{
    // Go down web
    if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
    {
        field_106_current_motion = eParamiteMotions::M_WebGoingDown_37_48CC60;
    }

    // Go up web
    if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
    {
        field_106_current_motion = eParamiteMotions::M_WebGoingUp_36_48D000;
    }

    // Try to leave to the left
    if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - (field_CC_sprite_scale * FP_FromDouble(0.5)),
            field_BC_ypos - FP_FromInteger(10),
            field_B8_xpos - (field_CC_sprite_scale * FP_FromDouble(0.5)),
            field_BC_ypos + FP_FromInteger(10),
            &pLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16))
        {
            auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(Types::eWebLine_146, field_B8_xpos, field_BC_ypos));
            if (pWeb)
            {
                pWeb->Wobble_4E29D0(FP_GetExponent(field_BC_ypos));
            }
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            field_100_pCollisionLine = pLine;
            field_106_current_motion = eParamiteMotions::M_WebLeaveDown_34_48D870;
        }
    }

    // Try to leave to the right
    if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos + (field_CC_sprite_scale * FP_FromDouble(0.5)),
            field_BC_ypos - FP_FromInteger(10),
            field_B8_xpos + (field_CC_sprite_scale * FP_FromDouble(0.5)),
            field_BC_ypos + FP_FromInteger(10),
            &pLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16))
        {
            auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(Types::eWebLine_146, field_B8_xpos, field_BC_ypos));
            if (pWeb)
            {
                pWeb->Wobble_4E29D0(FP_GetExponent(field_BC_ypos));
            }
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            field_100_pCollisionLine = pLine;
            field_106_current_motion = eParamiteMotions::M_WebLeaveDown_34_48D870;
        }
    }
}

void Paramite::M_WebGoingUp_36_48D000()
{
    if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
    {
        field_C8_vely = -(field_CC_sprite_scale * FP_FromInteger(4)); 
    }
    else
    {
        field_C8_vely = FP_FromInteger(0);
        field_106_current_motion = eParamiteMotions::M_WebIdle_35_48D400;
    }
    
    const auto oldYPos = field_BC_ypos;
    const auto pOldLine = field_100_pCollisionLine;

    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C8_vely);
    }

    if (sControlledCharacter_5C1B8C == this)
    {
        sub_408C40();
    }

    if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 3)
    {
        Sound_48F600(ParamiteSpeak::ClimbingWeb_6, 0);
    }

    if (!field_100_pCollisionLine ||
        !((1 << field_100_pCollisionLine->field_8_type) & 0x100) ||
        FP_GetExponent(field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(20))) < field_100_pCollisionLine->field_0_rect.y)
    {
        field_100_pCollisionLine = nullptr;

        const auto kHalfGrid = (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(0.5));

        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;

        if (!sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos - FP_FromInteger(10),
            field_BC_ypos - FP_FromInteger(30),
            field_B8_xpos,
            field_BC_ypos - FP_FromInteger(30),
            &pLine, &hitX, &hitY, field_D6_scale != 0 ? 6 : 0x60))
        {
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos - kHalfGrid,
                field_BC_ypos - FP_FromInteger(30),
                field_B8_xpos - kHalfGrid,
                field_BC_ypos + FP_FromInteger(10),
                &pLine, &hitX, &hitY, field_D6_scale != 0 ? 1 : 0x10))
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                field_100_pCollisionLine = pLine;
                field_106_current_motion = eParamiteMotions::M_WebLeaveUp_39_48D8C0;

                auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(Types::eWebLine_146, field_B8_xpos, field_BC_ypos));
                if (pWeb)
                {
                    pWeb->Wobble_4E29D0(FP_GetExponent(field_BC_ypos));
                }
                return;
            }

            if (!sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos + FP_FromInteger(10),
                field_BC_ypos - FP_FromInteger(30),
                field_B8_xpos, 
                field_BC_ypos - FP_FromInteger(30),
                &pLine, &hitX, &hitY, field_D6_scale != 0 ? 6 : 0x60))
            {
                if (sCollisions_DArray_5C1128->Raycast_417A60(
                    kHalfGrid + field_B8_xpos, 
                    field_BC_ypos - FP_FromInteger(30),
                    kHalfGrid + field_B8_xpos,
                    field_BC_ypos + FP_FromInteger(10),
                    &pLine, &hitX, &hitY, field_D6_scale != 0 ? 1 : 0x10))
                {

                    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                    field_B8_xpos = hitX;
                    field_BC_ypos = hitY;
                    field_100_pCollisionLine = pLine;
                    field_106_current_motion = eParamiteMotions::M_WebLeaveUp_39_48D8C0;
                }
                else
                {
                    field_106_current_motion = eParamiteMotions::M_Falling_11_48B200;
                    field_F8_LastLineYPos = field_BC_ypos;
                    field_DA_xOffset = field_15C;
                }

                auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(Types::eWebLine_146, field_B8_xpos, field_BC_ypos));
                if (pWeb)
                {
                    pWeb->Wobble_4E29D0(FP_GetExponent(field_BC_ypos));
                }
                return;
            }
        }

        field_BC_ypos = oldYPos;
        field_100_pCollisionLine = pOldLine;
    }
}

void Paramite::M_WebGoingDown_37_48CC60()
{
    if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
    {
        field_C8_vely = (field_CC_sprite_scale * FP_FromInteger(4)); 
    }
    else
    {
        field_C8_vely = FP_FromInteger(0);
        field_106_current_motion = eParamiteMotions::M_WebIdle_35_48D400;
    }

    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C8_vely);
    }

    if (sControlledCharacter_5C1B8C == this)
    {
        sub_408C40();
    }

    if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 3)
    {
        Sound_48F600(ParamiteSpeak::ClimbingWeb_6, 0);
    }

    if (!field_100_pCollisionLine || !((1 << field_100_pCollisionLine->field_8_type) & 0x100))
    {
        auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(Types::eWebLine_146, field_B8_xpos, field_BC_ypos));
        if (pWeb)
        {
            pWeb->Wobble_4E29D0(FP_GetExponent(field_BC_ypos));
        }
        field_100_pCollisionLine = nullptr;
        field_F8_LastLineYPos = field_BC_ypos;
        field_106_current_motion = eParamiteMotions::M_Falling_11_48B200;

        FP gridSize = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        if (sCollisions_DArray_5C1128->Raycast_417A60(
            gridSize + field_B8_xpos,
            field_BC_ypos,
            gridSize + field_B8_xpos,
            (field_CC_sprite_scale * FP_FromInteger(50)) + field_BC_ypos,
            &pLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16))
        {
            field_B8_xpos = (gridSize * FP_FromDouble(0.5)) + field_B8_xpos;
            field_100_pCollisionLine = pLine;
            field_BC_ypos = hitY;
            field_106_current_motion = eParamiteMotions::M_WebLeaveDown_34_48D870;
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }
        else
        {
            const FP invertedGridSize = -gridSize;
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                invertedGridSize + field_B8_xpos,
                field_BC_ypos,
                invertedGridSize + field_B8_xpos,
                (field_CC_sprite_scale * FP_FromInteger(50)) + field_BC_ypos,
                &pLine,
                &hitX,
                &hitY,
                field_D6_scale != 0 ? 1 : 16))
            {
                field_B8_xpos = (invertedGridSize * FP_FromDouble(0.5)) + field_B8_xpos;
                field_BC_ypos = hitY;
                field_100_pCollisionLine = pLine;
                field_106_current_motion = eParamiteMotions::M_WebLeaveDown_34_48D870;
            }
            else
            {
                field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
                field_B8_xpos -= (invertedGridSize * FP_FromDouble(0.5));
                field_DA_xOffset = field_15C;
            }
        }
    }
}

void Paramite::M_WebGrab_38_48D6C0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_DA_xOffset = 0;
        field_106_current_motion = eParamiteMotions::M_WebIdle_35_48D400;
        auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(Types::eWebLine_146, field_B8_xpos, field_BC_ypos));
        if (pWeb)
        {
            pWeb->Wobble_4E29D0(FP_GetExponent(field_BC_ypos));
        }
    }
}

void Paramite::M_WebLeaveUp_39_48D8C0()
{
    M_WebLeaveDown_34_48D870();
}

void Paramite::M_Eating_40_48A0F0()
{
    if (field_20_animation.field_92_current_frame == 5)
    {
        FP gridBlock = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridBlock = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            gridBlock = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        if (sControlledCharacter_5C1B8C == this)
        {
            auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(Types::eFleech_50, gridBlock + field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
            if (pFleech)
            {
                pFleech->VTakeDamage_408730(this);
                pFleech->field_6_flags.Set(BaseGameObject::eDead);
            }
            else
            {
                auto pSlurg = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(Types::eSlurg_129, gridBlock + field_B8_xpos, field_BC_ypos));
                if (pSlurg)
                {
                    auto pBlood = alive_new<Blood>();
                    if (pBlood)
                    {
                        pBlood->ctor_40F0B0(pSlurg->field_B8_xpos, pSlurg->field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 30);
                    }
                    pSlurg->field_6_flags.Set(BaseGameObject::eDead);
                }
                else
                {
                    SFX_Play_46FA90(static_cast<BYTE>(Math_RandomRange_496AB0(65, 66)), 0);
                }
            }
        }
        else
        {
            SFX_Play_46FA90(static_cast<BYTE>(Math_RandomRange_496AB0(65, 66)), 0);
        }
    }

    if (sControlledCharacter_5C1B8C == this && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            return;
        }
        else
        {
            ToIdle_489B70();
            return;
        }
    }

    if (field_108_next_motion != -1 && !ToNextMotion_4898A0())
    {
        ToIdle_489B70();
        return;
    }
}

void Paramite::M_Death_41_48D8E0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_UNKNOWN_42_48D900()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Sound_48F600(ParamiteSpeak::DoIt_2, 0);
    }
}

void Paramite::M_Attack_43_48DB70()
{
    BaseAliveGameObject* pObj = nullptr;

    if (field_20_animation.field_92_current_frame == 4)
    {
        pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
        Sound_48F600(ParamiteSpeak::CMon_or_Attack_0, 0);

        if (sControlledCharacter_5C1B8C == this)
        {
            if (FindTarget_488C30())
            {
                pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
                field_120_obj_id = -1;
            }
        }
        else if (field_178_flags.Get(Flags_178::eBit8))
        {
            if (!pObj)
            {
                FP gridBlock = {};
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    gridBlock = field_B8_xpos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
                }
                else
                {
                    gridBlock = ScaleToGridSize_4498B0(field_CC_sprite_scale) + field_B8_xpos;
                }
                pObj = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(Types::eFleech_50, gridBlock, field_BC_ypos));
            }
        }
    }

    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->vGetBoundingRect_424FD0(&otherRect, 1);

        PSX_RECT ourRect = {};
        vGetBoundingRect_424FD0(&ourRect, 1);

        short right = 0;
        short left = 0;
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            right = ourRect.x;
            left = (ourRect.x + ourRect.w) / 2;
        }
        else
        {
            left = ourRect.w;
            right = (ourRect.x + ourRect.w) / 2;
        }
        if (otherRect.x <= left &&
            otherRect.w >= right &&
            otherRect.h >= ourRect.y &&
            otherRect.y <= ourRect.h)
        {
            if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
            {
                if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                {
                    pObj->VTakeDamage_408730(this);
                }
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_489B70();
    }
}

void Paramite::dtor_487FC0()
{
    SetVTable(this, 0x54640C);

    BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_11C_web_id);
    if (pObj)
    {
        pObj->field_6_flags.Set(BaseGameObject::eDead);
        field_11C_web_id = -1;
    }

    field_118 = -1;
    VOnTrapDoorOpen();

    if (field_10C_health > FP_FromInteger(0) || field_178_flags.Get(Flags_178::eBit6))
    {
        Path::TLV_Reset_4DB8E0(field_140_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_140_tlvInfo, -1, 0, 1);
    }

    SND_SEQ_Stop_4CAE60(SeqId::ParamiteOffscreen_25);
    MusicController::sub_47FD60(0, this, 0, 0);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        if (gMap_5C3030.field_A_5C303A_levelId != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_14E_return_level,
                field_150_return_path,
                field_152_return_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }
    dtor_4080B0();
}

Paramite* Paramite::vdtor_487F90(signed int flags)
{
    dtor_487FC0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Paramite::vUpdate_4871B0()
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
        field_118 = BaseGameObject::Find_Flags_4DC170(field_118);
        field_120_obj_id = BaseGameObject::Find_Flags_4DC170(field_120_obj_id);
        field_124 = BaseGameObject::Find_Flags_4DC170(field_124);

        if (field_11C_web_id != -1)
        {
            auto pWeb = alive_new<ParamiteWeb>();
            if (pWeb)
            {
                pWeb->ctor_4E1840(
                    field_B8_xpos,
                    FP_GetExponent(field_BC_ypos) - 20,
                    FP_GetExponent(field_BC_ypos) - 10,
                    field_CC_sprite_scale);
            }
            field_11C_web_id = pWeb->field_8_object_id;
        }
    }

    auto pMeat = static_cast<BaseThrowable*>(sObjectIds_5C1B70.Find_449CF0(field_118));
    if (field_10C_health > FP_FromInteger(0)
        && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
    {
        SND_SEQ_PlaySeq_4CA960(SeqId::ParamiteOffscreen_25, 1, 0);
    }
    else if (!Find_Paramite_488810())
    {
        SND_SEQ_Stop_4CAE60(SeqId::ParamiteOffscreen_25);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    const FP xDelta = FP_Abs(field_B8_xpos- sControlledCharacter_5C1B8C->field_B8_xpos);
    const FP yDelta = FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos);

    if (xDelta > FP_FromInteger(750) || yDelta > FP_FromInteger(390))
    {
        if (field_178_flags.Get(Flags_178::eBit4) && field_178_flags.Get(Flags_178::eBit6))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        else if (pMeat)
        {
            if (pMeat->VIsFalling_49E330() || pMeat->field_6_flags.Get(BaseGameObject::eDead))
            {
                field_118 = -1;
                field_108_next_motion = 0;
                SetBrain(&Paramite::AI_Patrol_0_4835B0);
                field_12C_brain_ret = 0;
            }
        }
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        if (field_10C_health > FP_FromInteger(0))
        {
            if (!BrainIs(&Paramite::AI_ParamiteSpawn_9_48ED80) || field_178_flags.Get(Flags_178::eBit6))
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            }
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        }

        if (!Input_IsChanting_45F260())
        {
            field_178_flags.Clear(Flags_178::eBit5);
        }


        if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter_5C1B8C == this)
        {
            // Handle DDCheat mode

            VOnTrapDoorOpen();
            field_106_current_motion = 11;
            field_F8_LastLineYPos = field_BC_ypos;
            field_100_pCollisionLine = nullptr;

            static int sArray1_5C929C[8] = {};
            static int sArray2_5C92BC[8] = {};

            static char byte_5C9298 = 0;

            if (!(byte_5C9298 & 1))
            {
                byte_5C9298 |= 1u;
                sArray1_5C929C[0] = 4;
                sArray1_5C929C[1] = 4;
                sArray1_5C929C[2] = 0;
                sArray1_5C929C[3] = -4;
                sArray1_5C929C[4] = -4;
                sArray1_5C929C[5] = -4;
                sArray1_5C929C[6] = 0;
                sArray1_5C929C[7] = 4;
            }
            else if (!(byte_5C9298 & 2))
            {
                byte_5C9298 = byte_5C9298 | 2;
                sArray2_5C92BC[0] = 0;
                sArray2_5C92BC[1] = -4;
                sArray2_5C92BC[2] = -4;
                sArray2_5C92BC[3] = -4;
                sArray2_5C92BC[4] = 0;
                sArray2_5C92BC[5] = 4;
                sArray2_5C92BC[6] = 4;
                sArray2_5C92BC[7] = 4;
            }

            // TODO: InputCommand constants
            if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & 0xF)
            {
                field_C4_velx = FP_FromInteger(sArray1_5C929C[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5]);
                field_C8_vely = FP_FromInteger(sArray2_5C92BC[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5]);

                if (sInputKey_Run_5550E8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                {
                    field_C4_velx += FP_FromInteger(sArray1_5C929C[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5]);
                    field_C4_velx += FP_FromInteger(sArray1_5C929C[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5]);
                    field_C8_vely += FP_FromInteger(sArray2_5C92BC[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5]);
                }

                field_B8_xpos += field_C4_velx;
                field_BC_ypos += field_C8_vely;

                // Keep in map bounds
                PSX_Point mapBounds = {};
                gMap_5C3030.Get_map_size_480640(&mapBounds);
                if (field_B8_xpos < FP_FromInteger(0))
                {
                    field_B8_xpos = FP_FromInteger(0);
                }

                if (field_B8_xpos >= FP_FromInteger(mapBounds.field_0_x))
                {
                    field_B8_xpos = FP_FromInteger(mapBounds.field_0_x) - FP_FromInteger(1);
                }

                if (field_BC_ypos < FP_FromInteger(0))
                {
                    field_BC_ypos = FP_FromInteger(0);
                }

                if (field_BC_ypos >= FP_FromInteger(mapBounds.field_2_y))
                {
                    field_BC_ypos = FP_FromInteger(mapBounds.field_2_y) - FP_FromInteger(1);
                    sub_408C40();
                    return;
                }
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);
                field_C8_vely = FP_FromInteger(0);
            }
            sub_408C40();
        }
        else
        {
            const auto oldMotion = field_106_current_motion;
            const auto oldBrain = field_128_fn_brainState;
            const auto oldBrainRet = field_12C_brain_ret;
            field_12C_brain_ret = (this->*field_128_fn_brainState)();

            if (sDDCheat_ShowAI_Info_5C1BD8)
            {
                DDCheat::DebugStr_4F5560(
                    "Paramite %d %d %d %d\n",
                    field_12C_brain_ret,
                    field_130_timer,
                    field_106_current_motion,
                    field_108_next_motion);
            }

            const FP oldXPos = field_B8_xpos;
            const FP oldYPos = field_BC_ypos;
            (this->*sParamite_motion_table_55D5B0[field_106_current_motion])();

            // TODO: This is extra debug logging to figure out the motion names
            if (oldBrain != field_128_fn_brainState)
            {
                LOG_INFO("Paramite: Old brain = " << GetOriginalFn(oldBrain, sParamiteAITable).fnName << " new brain = " << GetOriginalFn(field_128_fn_brainState, sParamiteAITable).fnName);
                //LOG_INFO("Paramite: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
            }
            else
            {
                if (oldBrainRet != field_12C_brain_ret)
                {
                    LOG_INFO("Paramite: Brain state = " << GetOriginalFn(field_128_fn_brainState, sParamiteAITable).fnName << " State change from " << oldBrainRet << " to " << field_12C_brain_ret);
                }
            }

            if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
            {
                field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                    nullptr,
                    field_B8_xpos,
                    field_BC_ypos,
                    field_B8_xpos,
                    field_BC_ypos);
                VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
            }

            if (field_114_flags.Get(Flags_114::e114_Bit1_bShot))
            {
                ToKnockBack_489BB0();
                field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
                field_106_current_motion = field_108_next_motion;
                field_108_next_motion = -1;
            }

            if (oldMotion == field_106_current_motion)
            {
                if (field_178_flags.Get(Flags_178::eBit2))
                {
                    field_106_current_motion = field_F4;
                    vUpdateAnim_487170();
                    field_20_animation.SetFrame_409D50(field_F6_anim_frame);
                    field_178_flags.Clear(Flags_178::eBit2);
                }
            }
            else
            {
                vUpdateAnim_487170();
            }
        }
    }
}

__int16 Paramite::Find_Paramite_488810()
{
    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eParamite_96 &&
            pObj != this &&
            gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pObj->field_C2_lvl_number, pObj->field_C0_path_number, pObj->field_B8_xpos, pObj->field_BC_ypos, 0))
        {
            return 1;
        }
    }
    return 0;
}

void Paramite::vUpdateAnim_487170()
{
    field_20_animation.Set_Animation_Data_409C80(sParamiteFrameTableOffsets_55D660[field_106_current_motion], ResBlockForMotion_488130(field_106_current_motion));
}

Meat* Paramite::FindMeat_488930()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eMeat_84)
        {
            auto pMeat = static_cast<Meat*>(pObj);
            if (pMeat->VCanEatMe_4696A0())
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pMeat->field_C2_lvl_number, pMeat->field_C0_path_number, pMeat->field_B8_xpos, pMeat->field_BC_ypos, 0) &&
                    !WallHit_408750(field_BC_ypos, pMeat->field_B8_xpos - field_B8_xpos))
                {
                    if (!pMeat->field_130_pLine)
                    {
                        return pMeat;
                    }

                    if (FP_Abs(pMeat->field_BC_ypos - field_BC_ypos) <= FP_FromInteger(20))
                    {
                        return pMeat;
                    }
                }
            }
        }
    }
    return nullptr;
}

__int16 Paramite::IsNear_488B10(Paramite* pOther)
{
    return FP_Abs(pOther->field_BC_ypos - field_BC_ypos) < field_CC_sprite_scale * FP_FromInteger(100);
}

__int16 Paramite::vOnSameYLevel_488A40(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther)
    {
        PSX_RECT bRect = {};
        pOther->vGetBoundingRect_424FD0(&bRect, 1);

        if ((FP_Abs(field_BC_ypos - FP_FromInteger(bRect.h)) < field_CC_sprite_scale * FP_FromInteger(40)) ||
            (pOther->field_4_typeId == Types::eParamite_96 && static_cast<Paramite*>(pOther)->field_106_current_motion == eParamiteMotions::M_JumpUpMidair_13_48BAF0))
        {
            return 1;
        }
    }
    return 0;
}

void Paramite::vUnPosses_488BE0()
{
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
    field_130_timer = sGnFrame_5C1B84 + 180;
    SetBrain(&Paramite::AI_Patrol_0_4835B0);
    field_12C_brain_ret = 0;
}

void Paramite::vPossessed_488B60()
{
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_178_flags.Set(Flags_178::eBit5);
    SetBrain(&Paramite::AI_Possessed_6_484BC0);
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    field_12C_brain_ret = 0;
    field_130_timer = sGnFrame_5C1B84 + 30;
    field_14E_return_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_150_return_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_152_return_camera = gMap_5C3030.sCurrentCamId_5C3034;
}

__int16 Paramite::vTakeDamage_488250(BaseGameObject* pFrom)
{
    if (field_10C_health <= FP_FromInteger(0))
    {
        return 1;
    }

    field_118 = -1;

    switch (pFrom->field_4_typeId)
    {
    case Types::eGrinder_30:
    case Types::eBaseBomb_46:
    case Types::eExplosion_109:
    {
        Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
        auto pGibs = alive_new<Gibs>();
        if (pGibs)
        {
            pGibs->ctor_40FB40(2, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
        }
        field_10C_health = FP_FromInteger(0);
        field_6_flags.Set(BaseGameObject::eDead);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        if (sControlledCharacter_5C1B8C != this)
        {
            return 1;
        }
        SND_SEQ_Play_4CAB10(SeqId::DeathDrums_29, 1, 127, 127);
    }
        return 1;

    case Types::eFleech_50:
    {
        field_10C_health -= FP_FromDouble(0.15);
        if (field_10C_health < FP_FromInteger(0))
        {
            field_10C_health = FP_FromInteger(0);
        }
        if (sControlledCharacter_5C1B8C != this)
        {
            if (field_120_obj_id == -1 || field_120_obj_id == sActiveHero_5C1B68->field_8_object_id)
            {
                if (BrainIs(&Paramite::AI_Patrol_0_4835B0) || BrainIs(&Paramite::AI_ControlledByGameSpeak_8_48DFC0))
                {
                    SetBrain(&Paramite::AI_ChasingAbe_2_4859D0);
                    field_12C_brain_ret = 0;
                    field_148_timer = sGnFrame_5C1B84 + field_144_attack_duration;
                    field_120_obj_id = pFrom->field_8_object_id;
                }
            }
        }

        if (field_10C_health > FP_FromInteger(0))
        {
            return 1;
        }

        Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
        SetBrain(&Paramite::AI_Death_1_484CD0);
        field_130_timer = sGnFrame_5C1B84 + 90;
        field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
        vUpdateAnim_487170();

        auto pBlood = alive_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 50);
        }

        if (sControlledCharacter_5C1B8C == this)
        {
            SND_SEQ_Play_4CAB10(SeqId::DeathDrums_29, 1, 127, 127);
        }
    }
        return 0;

    case Types::eType_104:
        return 0;

    default:
    {
        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);
        Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
        field_10C_health = FP_FromInteger(0);
        SetBrain(&Paramite::AI_Death_1_484CD0);
        field_130_timer = sGnFrame_5C1B84 + 90;
        field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
        vUpdateAnim_487170();

        auto pBlood = alive_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 50);
        }

        if (sControlledCharacter_5C1B8C != this)
        {
            return 1;
        }

        SND_SEQ_Play_4CAB10(SeqId::DeathDrums_29, 1, 127, 127);
        return 1;
    }
    }
}

BYTE** Paramite::ResBlockForMotion_488130(__int16 motion)
{
    if (motion < eParamiteMotions::M_GameSpeakBegin_20_48C010)
    {
        return field_10_resources_array.ItemAt(0);
    }
    if (motion < eParamiteMotions::M_RunningAttack_31_48C9E0)
    {
        return field_10_resources_array.ItemAt(5);
    }
    if (motion < eParamiteMotions::M_Empty_32_48D740)
    {
        return field_10_resources_array.ItemAt(14);
    }
    if (motion < eParamiteMotions::M_Eating_40_48A0F0)
    {
        return field_10_resources_array.ItemAt(4);
    }
    if (motion < eParamiteMotions::M_Death_41_48D8E0)
    {
        return field_10_resources_array.ItemAt(1);
    }
    if (motion < eParamiteMotions::M_UNKNOWN_42_48D900)
    {
        return field_10_resources_array.ItemAt(15);
    }
    if (motion >= eParamiteMotions::M_Attack_43_48DB70)
    {
        return field_10_resources_array.ItemAt(motion >= 44 ? 0 : 9);
    }
    return field_10_resources_array.ItemAt(10);
}

void Paramite::vOn_TLV_Collision_488640(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_10C_health = FP_FromInteger(0);
                field_12C_brain_ret = 0;
                SetBrain(&Paramite::AI_DeathDrop_7_484FF0);
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
                Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
            }
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, field_B8_xpos, field_BC_ypos, field_B8_xpos, field_BC_ypos);
    }
}

__int16 Paramite::AnotherParamiteNear_4886E0()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eParamite_96 && pObj != this)
        {
            auto pOther = static_cast<Paramite*>(pObj);
            if (pOther->field_CC_sprite_scale == field_CC_sprite_scale && 
                gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pOther->field_C2_lvl_number, pOther->field_C0_path_number, pOther->field_B8_xpos, pOther->field_BC_ypos, 0)  &&
                gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) &&
                IsNear_488B10(pOther))
            {
                if (pOther->BrainIs(&Paramite::AI_Patrol_0_4835B0) || pOther->BrainIs(&Paramite::AI_ChasingAbe_2_4859D0))
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

PathLine* Paramite::WebCollision_4888A0(FP yOff, FP xOff)
{
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (!sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos + yOff,
        field_B8_xpos + xOff,
        field_BC_ypos + yOff, &pLine, &hitX, &hitY, 0x100))
    {
        return 0;
    }

    field_B8_xpos = hitX;
    field_BC_ypos = hitY;
    return pLine;
}

void Paramite::vRender_488220(int** ot)
{
    if (field_1C_update_delay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void Paramite::vOnTrapDoorOpen_489F60()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        field_106_current_motion = eParamiteMotions::M_Falling_11_48B200;
    }
}

void Paramite::ToHop_489C20()
{
    VOnTrapDoorOpen();
    field_106_current_motion = eParamiteMotions::M_Hop_5_48B5B0;
    field_108_next_motion = -1;
    field_F8_LastLineYPos = field_BC_ypos;
    field_C8_vely = FP_FromDouble(-6.3) * field_CC_sprite_scale;
    field_BC_ypos += field_C8_vely;
    field_100_pCollisionLine = nullptr;
}

__int16 Paramite::CanIAcceptAGameSpeakCommand_489160()
{
    if (sControlledCharacter_5C1B8C->field_4_typeId != Types::eParamite_96)
    {
        return 0;
    }

    const int distToPlayer = Math_Distance_496EB0(
        FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos),
        FP_GetExponent(sControlledCharacter_5C1B8C->field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos));

    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find another paramite on the same layer/scale
        if (pObj != this &&
            pObj != sControlledCharacter_5C1B8C && 
            pObj->field_CC_sprite_scale == sControlledCharacter_5C1B8C->field_CC_sprite_scale && 
            pObj->field_4_typeId == Types::eParamite_96)
        {
            auto pParamite = static_cast<Paramite*>(pObj);

            if (pParamite->field_178_flags.Get(Flags_178::eBit7))
            {
                return 0;
            }

            if (!pParamite->BrainIs(&Paramite::AI_ControlledByGameSpeak_8_48DFC0) &&
                gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pParamite->field_C2_lvl_number, pParamite->field_C0_path_number, pParamite->field_B8_xpos, pParamite->field_BC_ypos, 0))
            {
                if (sControlledCharacter_5C1B8C->vIsFacingMe_4254A0(pParamite) && !sControlledCharacter_5C1B8C->vIsFacingMe_4254A0(this))
                {
                    return 0;
                }

                if (distToPlayer > Math_Distance_496EB0(
                    FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos),
                    FP_GetExponent(sControlledCharacter_5C1B8C->field_BC_ypos),
                    FP_GetExponent(pParamite->field_B8_xpos),
                    FP_GetExponent(pParamite->field_BC_ypos)))
                {
                    if (sControlledCharacter_5C1B8C->vIsFacingMe_4254A0(pParamite) == sControlledCharacter_5C1B8C->vIsFacingMe_4254A0(this))
                    {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

__int16 Paramite::HandleEnemyStopper_4893B0(__int16 numGridBlocks)
{
    auto pEnemyStopper = static_cast<Path_EnemyStopper*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos + (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(numGridBlocks))),
        FP_GetExponent(field_BC_ypos),
        TlvTypes::EnemyStopper_47));

    // No stopper or its disabled
    if (!pEnemyStopper || !SwitchStates_Get_466020(pEnemyStopper->field_12_id))
    {
        return 0;
    }

    // We have a stopper that applies to any direction
    if (pEnemyStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Both_2)
    {
        return 1;
    }

    // Does the stopper direction match the animation direction?
    if ((pEnemyStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Left_0 &&  field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) || 
        (pEnemyStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Right_1 && !(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))))
    {
        return 1;
    }

    return 0;
}

PullRingRope* Paramite::FindPullRope_488F20()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::ePullRope_103)
        {
            auto pRope = static_cast<PullRingRope*>(pObj);

            if (pRope->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                PSX_RECT bRect = {};
                pRope->vGetBoundingRect_424FD0(&bRect, 1);
                if ((field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(40))) <= pRope->field_BC_ypos && field_BC_ypos > pRope->field_BC_ypos)
                {
                    if (field_B8_xpos > FP_FromInteger(bRect.x) && field_B8_xpos < FP_FromInteger(bRect.w))
                    {
                        return pRope;
                    }
                }
            }
        }
    }
    return nullptr;
}

__int16 Paramite::NextPlayerInputMotion_4894C0()
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_106_current_motion = eParamiteMotions::M_Turn_4_48B180;
            return 1;
        }
        else
        {
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), kGridSize))
            {
                return 0;
            }

            if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
            {
                field_C4_velx = (kGridSize / FP_FromDouble(3.5));
                field_106_current_motion = eParamiteMotions::M_RunBegin_9_48AF10;
            }
            else
            {
                field_C4_velx = (kGridSize / FP_FromInteger(7));
                field_106_current_motion = eParamiteMotions::M_WalkBegin_1_48A7B0;
            }
            return 1;

        }
    }
    else if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
        {
            field_106_current_motion = eParamiteMotions::M_Turn_4_48B180;
            return 1;
        }
        else
        {
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -kGridSize))
            {
                return 0;
            }

            if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
            {
                field_C4_velx = -(kGridSize / FP_FromDouble(3.5));
                field_106_current_motion = eParamiteMotions::M_RunBegin_9_48AF10;
            }
            else
            {
                field_C4_velx = -(kGridSize / FP_FromInteger(7));
                field_106_current_motion = eParamiteMotions::M_WalkBegin_1_48A7B0;
            }
            return 1;
        }
    }
    else
    {
        if (sInputObject_5BD4E0.isHeld(sInputKey_Up_5550D8))
        {
            field_106_current_motion = eParamiteMotions::M_JumpUpBegin_12_48BE40;
            return 1;
        }
        else if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            FP xCheck = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                xCheck = -kGridSize;
            }
            else
            {
                xCheck = kGridSize;
            }

            PathLine* pLine = WebCollision_4888A0(field_CC_sprite_scale * FP_FromInteger(20), xCheck);
            if (pLine)
            {
                field_106_current_motion = eParamiteMotions::M_WebGrab_38_48D6C0;
                field_100_pCollisionLine = pLine;
            }
            else
            {
                field_106_current_motion = eParamiteMotions::M_Eating_40_48A0F0;
            }
            return 1;
        }
        else
        {
            if (sInputObject_5BD4E0.isPressed(sInputKey_ThrowItem_5550F4 | sInputKey_DoAction_5550E4 | sInputKey_GameSpeak5_55510C))
            {
                if (FindTarget_488C30())
                {
                    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
                    if (pTarget)
                    {
                        field_120_obj_id = -1;
                        if (vIsObjNearby_4253B0(kGridSize * FP_FromDouble(0.5), pTarget))
                        {
                            field_106_current_motion = eParamiteMotions::M_CloseAttack_16_48DDA0;
                            return 1;
                        }
                    }
                }

                field_106_current_motion = eParamiteMotions::M_Attack_43_48DB70;
                return 1;
            }
            else
            {
                if (sInputObject_5BD4E0.isPressed(sInputKey_Hop_5550E0))
                {
                    ToHop_489C20();
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
}

__int16 Paramite::FindTarget_488C30()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        BaseAliveGameObject* pBestTarget = nullptr;
        for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if ((pObj->field_4_typeId == Types::eFleech_50 ||
                pObj->field_4_typeId == Types::eSlig_125 ||
                pObj->field_4_typeId == Types::eSlurg_129 ||
                pObj->field_4_typeId == Types::eMudokon_110) &&
                pObj->field_10C_health > FP_FromInteger(0))
            {
                if (vOnSameYLevel_488A40(pObj))
                {
                    if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                    {
                        if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                        {
                            if (FP_GetExponent(pObj->field_B8_xpos) / 375 == FP_GetExponent(field_B8_xpos) / 375 &&
                                FP_GetExponent(pObj->field_BC_ypos) / 260 == FP_GetExponent(field_BC_ypos) / 260)
                            {
                                if (!pBestTarget)
                                {
                                    pBestTarget = pObj;
                                }
                                else
                                {
                                    if (FP_Abs(field_B8_xpos - pObj->field_B8_xpos) < FP_Abs(field_B8_xpos - pBestTarget->field_B8_xpos))
                                    {
                                        pBestTarget = pObj;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (pBestTarget)
        {
            field_120_obj_id = pBestTarget->field_8_object_id;
            return 1;
        }
    }

    if (vOnSameYLevel_425520(sActiveHero_5C1B68) &&
        !sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible) && 
        field_CC_sprite_scale == sActiveHero_5C1B68->field_CC_sprite_scale &&
        !WallHit_408750((sActiveHero_5C1B68->field_CC_sprite_scale * FP_FromInteger(20)), sActiveHero_5C1B68->field_B8_xpos - field_B8_xpos))
    {
        field_120_obj_id = sActiveHero_5C1B68->field_8_object_id;
        return 1;
    }
    else
    {
        field_120_obj_id = -1;
        return 0;
    }
}

__int16 Paramite::ToNextMotion_4898A0()
{
    if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
    {
        return NextPlayerInputMotion_4894C0();
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    switch (field_108_next_motion)
    {
    case eParamiteMotions::M_Turn_4_48B180:
    case eParamiteMotions::M_Eating_40_48A0F0:
    case eParamiteMotions::M_JumpUpBegin_12_48BE40:
    case eParamiteMotions::M_UNKNOWN_42_48D900:
    case eParamiteMotions::M_Attack_43_48DB70:
    case eParamiteMotions::M_CloseAttack_16_48DDA0:
    case eParamiteMotions::M_GameSpeakBegin_20_48C010:
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
        return 1;

    case eParamiteMotions::M_Running_3_48AA00:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -(kGridSize / FP_FromDouble(3.5));
            field_106_current_motion = eParamiteMotions::M_RunBegin_9_48AF10;
        }
        else
        {
            field_C4_velx = (kGridSize / FP_FromDouble(3.5));
            field_106_current_motion = eParamiteMotions::M_RunBegin_9_48AF10;
        }
        field_108_next_motion = -1;
        return 1;

    case eParamiteMotions::M_Walking_2_48A2D0:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -(kGridSize / FP_FromInteger(7));
        }
        else
        {
            field_C4_velx = (kGridSize / FP_FromInteger(7));
        }
        field_106_current_motion = eParamiteMotions::M_WalkBegin_1_48A7B0;
        field_108_next_motion = -1;
        return 1;

    case eParamiteMotions::M_Hiss1_22_48C3E0:
    case eParamiteMotions::M_Empty_24_48C680:
    case eParamiteMotions::M_Hiss2_23_48C600:
    case eParamiteMotions::M_Hiss3_26_48C6F0:
        field_106_current_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
        return 1;

    case eParamiteMotions::M_AllOYaGameSpeakBegin_25_48C6A0:
        field_106_current_motion = eParamiteMotions::M_AllOYaGameSpeakBegin_25_48C6A0;
        field_108_next_motion = -1;
        return 1;

    case eParamiteMotions::M_RunningAttack_31_48C9E0:
        field_106_current_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
        field_108_next_motion = -1;
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -(kGridSize / FP_FromInteger(4));
        }
        else
        {
            field_C4_velx = (kGridSize / FP_FromInteger(4));
        }
        return 1;

    case eParamiteMotions::M_Hop_5_48B5B0:
        ToHop_489C20();
        return 1;

    default:
        return 0;
    }
}

void Paramite::ToIdle_489B70()
{
    field_13C = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = eParamiteMotions::M_Idle_0_489FB0;
    MapFollowMe_408D10(TRUE);
    field_154 = 0;
}

void Paramite::ToKnockBack_489BB0()
{
    field_B8_xpos -= field_C4_velx;
    field_154 = 0;

    MapFollowMe_408D10(TRUE);

    if (field_C8_vely < FP_FromInteger(0))
    {
        field_C8_vely = FP_FromInteger(0);
    }

    field_106_current_motion = eParamiteMotions::M_Knockback_19_48BF50;
}

void Paramite::MoveOnLine_489CA0()
{
    if (!field_100_pCollisionLine)
    {
        field_F8_LastLineYPos = field_BC_ypos;
        field_106_current_motion = eParamiteMotions::M_Falling_11_48B200;
        return;
    }

    BaseGameObject* pPlatform = sObjectIds_5C1B70.Find_449CF0(field_110_id);
    const FP oldXPos = field_B8_xpos;

    // As we move on the line it might change to another one
    field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
    if (field_100_pCollisionLine)
    {
        if (field_100_pCollisionLine->field_8_type == 8)
        {
            field_C4_velx = FP_FromInteger(0);
            field_B8_xpos = FP_FromInteger(field_100_pCollisionLine->field_0_rect.x);
            field_BC_ypos = FP_FromInteger(field_100_pCollisionLine->field_0_rect.y) + (field_CC_sprite_scale * FP_FromInteger(20));
            field_106_current_motion = eParamiteMotions::M_WebGrab_38_48D6C0;
        }
        else if (pPlatform)
        {
            if (field_100_pCollisionLine->field_8_type != 32 && field_100_pCollisionLine->field_8_type != 36)
            {
                const auto savedMotion = field_106_current_motion;
                VOnTrapDoorOpen();
                field_106_current_motion = savedMotion;
            }
        }
        else if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
        {
            CheckForPlatform_489EA0();
        }
    }
    else
    {
        // Not on a line so we are falling

        if (sControlledCharacter_5C1B8C == this)
        {
            // Check if we hit a web while falling and grab it if so
            field_100_pCollisionLine = WebCollision_4888A0(field_CC_sprite_scale * FP_FromInteger(20), field_C4_velx);
            if (field_100_pCollisionLine)
            {
                field_C4_velx = FP_FromInteger(0);
                field_106_current_motion = eParamiteMotions::M_WebGrab_38_48D6C0;
                return;
            }
        }

        // Otherwise just fall
        VOnTrapDoorOpen();
        field_106_current_motion = eParamiteMotions::M_Falling_11_48B200;
        field_F8_LastLineYPos = field_BC_ypos;
        field_13C = FP_FromDouble(0.3);
        field_B8_xpos = oldXPos + field_C4_velx;
    }
}

void Paramite::CheckForPlatform_489EA0()
{
    PSX_Point xy = {};
    PSX_Point wh = {};

    xy.field_0_x = FP_GetExponent(field_B8_xpos - FP_FromInteger(5));
    xy.field_2_y = FP_GetExponent(field_BC_ypos - FP_FromInteger(5));

    wh.field_0_x = FP_GetExponent(field_B8_xpos + FP_FromInteger(5));
    wh.field_2_y = FP_GetExponent(field_BC_ypos + FP_FromInteger(5));

    vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
}

void Paramite::HandleStopWalking_48A720()
{
    // Pressing opposite direction of movement or not pressing any direction
    if ((field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4)) ||
        (field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0)) ||
        !sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        field_106_current_motion = eParamiteMotions::M_WalkEnd_8_48A870;
    }
}

void Paramite::HandleInputRunning_48ADB0()
{
    if ((field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4)) ||
        (field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0)) ||
        !sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        field_106_current_motion = eParamiteMotions::M_RunEnd_10_48B000;
    }
    else if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
    {
        if (sInputKey_Hop_5550E0 & field_154)
        {
            ToHop_489C20();
        }
        else if ((sInputKey_ThrowItem_5550F4 | sInputKey_DoAction_5550E4) & field_154)
        {
            field_106_current_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
            }
            else
            {
                field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(4));
            }
        }
    }
    else
    {
        field_106_current_motion = eParamiteMotions::M_WalkRunTransition_7_48B0C0;
    }
}

ALIVE_VAR(1, 0x5C92EC, short, sParamiteDelayIdx_5C92EC, 0);

const __int16 sParamiteDelayTable_55D7B8[4] = { 0, 5, 10, 0 };

__int16 CC Paramite::StableDelay_48DF80()
{
    if (sParamiteDelayIdx_5C92EC >= 3u)
    {
        sParamiteDelayIdx_5C92EC = 0;
    }
    return sParamiteDelayTable_55D7B8[sParamiteDelayIdx_5C92EC++];
}

const SfxDefinition stru_55D7C0[12] =
{
    { 0u, 5u, 60u, 90u, -520, -520 },
    { 0u, 5u, 61u, 75u, -520, -520 },
    { 0u, 5u, 62u, 50u, -520, -520 },
    { 0u, 5u, 64u, 40u, -520, -520 },
    { 0u, 5u, 65u, 40u, -520, -520 },
    { 0u, 5u, 66u, 30u, -520, -520 },
    { 0u, 5u, 66u, 50u, 0, 0 },
    { 0u, 5u, 67u, 40u, -520, -520 },
    { 0u, 5u, 68u, 30u, -520, -520 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 }
};

void Paramite::Sound_48F600(ParamiteSpeak soundId, __int16 pitch_min)
{
    LOG_INFO("Paramite sound = " << static_cast<int>(soundId));

    const CameraPos direction = gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos);

    short volRight = 0;
    if (soundId == ParamiteSpeak::Howdy_5)
    {
        volRight = stru_55D7C0[5].field_3_default_volume;
        if (sControlledCharacter_5C1B8C == this)
        {
            volRight *= 3;
        }
    }
    else if (soundId == ParamiteSpeak::AllYa_9)
    {
        volRight = sSeqData_558D50.mData[20].field_9;
    }
    else
    {
        volRight = stru_55D7C0[static_cast<int>(soundId)].field_3_default_volume;
    }

    if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        volRight = (2 * volRight) / 3;
    }

    PSX_RECT pRect = {};
    gMap_5C3030.Get_Camera_World_Rect_481410(direction, &pRect);

    short volLeft = 0;
    switch (direction)
    {
    case CameraPos::eCamCurrent_0:
        volLeft = volRight;
        break;

    case CameraPos::eCamTop_1:
    case CameraPos::eCamBottom_2:
    {
        const short v12 = FP_GetExponent(FP_FromRaw(stru_55D7C0[static_cast<int>(soundId)].field_3_default_volume) / FP_FromInteger(3));
        volLeft = v12;
        volRight = v12;
        break;
    }

    case CameraPos::eCamLeft_3:
    {
        const FP v9 = (FP_FromInteger(pRect.w) - field_B8_xpos) / FP_FromInteger(368);
        volLeft = volRight - FP_GetExponent((v9 * FP_FromInteger((volRight -1) * (volRight / 3))));
        const FP v10 = (v9 * FP_FromInteger(volRight));
        volRight = volRight - FP_GetExponent(v10);
        break;
    }

    case CameraPos::eCamRight_4:
    {
        const FP v11 = (field_B8_xpos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
        volLeft = volRight - FP_GetExponent(v11 * FP_FromInteger(volRight));
        const FP v10 = (v11 * FP_FromInteger((volRight - 1) * (volRight / 3)));
        volRight = volRight - FP_GetExponent(v10);
        break;
    }

    default:
        return;
    }

    if (soundId == ParamiteSpeak::AllYa_9)
    {
        SND_SEQ_Play_4CAB10(SeqId::MainMenuParamiteAttack_20, 1, volLeft, volRight);
    }
    else if (pitch_min > 0)
    {
        SFX_SfxDefinition_Play_4CA700(&stru_55D7C0[static_cast<int>(soundId)], volLeft, volRight, pitch_min, pitch_min);
    }
    else
    {
        SFX_SfxDefinition_Play_4CA700(&stru_55D7C0[static_cast<int>(soundId)], volLeft, volRight, -520, -520);
    }
}

void Paramite::UpdateSlurgWatchPoints_4890D0()
{
    const char count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
    if (count < 5)
    {
        if (field_100_pCollisionLine)
        {
            Slurg_Step_Watch_Points* pPoints = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
            pPoints->field_0_points[count].field_0_xPos = FP_GetExponent(field_B8_xpos);
            pPoints->field_0_points[count].field_2_yPos = field_100_pCollisionLine->field_0_rect.y - 5;
            sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = count + 1;
        }
    }
}

GameSpeakEvents Paramite::LastSpeak_489040()
{
    GameSpeakEvents ret = GameSpeakEvents::eNone_m1;
    if (field_160 == pEventSystem_5BC11C->field_28_last_event_index)
    {
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            ret = GameSpeakEvents::eNone_m1;
        }
        else
        {
            ret = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_160 = pEventSystem_5BC11C->field_28_last_event_index;
        ret = pEventSystem_5BC11C->field_20_last_event;
    }

    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 1))
    {
        return ret;
    }

    return GameSpeakEvents::eNone_m1;
}
