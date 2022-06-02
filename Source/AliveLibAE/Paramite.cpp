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
#include "Sound/Midi.hpp"
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
#include "Grid.hpp"

const TintEntry kParamiteTints_55D73C[24] = {
    {LevelIds_s8::eMudomoVault_3, 105u, 105u, 105u},
    {LevelIds_s8::eMudomoVault_Ender_11, 105u, 105u, 105u},
    {LevelIds_s8::eNone, 105u, 105u, 105u}};


const TParamiteMotionFn sParamite_motion_table_55D5B0[44] = {
#define MAKE_FN(VAR) &Paramite::VAR,
    PARAMITE_MOTIONS_ENUM(MAKE_FN)
#undef MAKE_FN
};

const TParamiteBrainFn sParamite_brain_table_55D710[10] = {
    &Paramite::Brain_0_Patrol_4835B0,
    &Paramite::Brain_1_Death_484CD0,
    &Paramite::Brain_2_ChasingAbe_4859D0,
    &Paramite::Brain_3_SurpriseWeb_4851B0,
    &Paramite::Brain_4_Unused_48F8F0,
    &Paramite::Brain_5_SpottedMeat_486880,
    &Paramite::Brain_6_Possessed_484BC0,
    &Paramite::Brain_7_DeathDrop_484FF0,
    &Paramite::Brain_8_ControlledByGameSpeak_48DFC0,
    &Paramite::Brain_9_ParamiteSpawn_48ED80};


const static BrainFunctionData<TParamiteBrainFn> sParamiteBrainTable[10] = {
    {&Paramite::Brain_0_Patrol_4835B0, 0x402A7C, "Brain_0_Patrol"},
    {&Paramite::Brain_1_Death_484CD0, 0x404223, "Brain_1_Death"},
    {&Paramite::Brain_2_ChasingAbe_4859D0, 0x401799, "Brain_2_ChasingAbe"},
    {&Paramite::Brain_3_SurpriseWeb_4851B0, 0x401645, "Brain_3_SurpriseWeb"},
    {&Paramite::Brain_4_Unused_48F8F0, 0x48F8F0, "Brain_4_Unused_4"},
    {&Paramite::Brain_5_SpottedMeat_486880, 0x4012E4, "Brain_5_SpottedMeat"},
    {&Paramite::Brain_6_Possessed_484BC0, 0x40187F, "Brain_6_Possessed"},
    {&Paramite::Brain_7_DeathDrop_484FF0, 0x4021A3, "Brain_7_DeathDrop"},
    {&Paramite::Brain_8_ControlledByGameSpeak_48DFC0, 0x4010B4, "Brain_8_ControlledByGameSpeak_8"},
    {&Paramite::Brain_9_ParamiteSpawn_48ED80, 0x401EE7, "Brain_9_ParamiteSpawn"},
};

void Paramite::SetBrain(TParamiteBrainFn fn)
{
    return ::SetBrain(fn, field_128_fn_brainState, sParamiteBrainTable);
}

bool Paramite::BrainIs(TParamiteBrainFn fn)
{
    return ::BrainIs(fn, field_128_fn_brainState, sParamiteBrainTable);
}


Paramite::Paramite(Path_Paramite* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(16)
{
    field_160_last_event_index = -1;
    field_174_unused = 0;
    field_176_unused = -1;

    SetType(AETypes::eParamite_96);

    field_C_objectId = tlvInfo;

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArjbasicResID, 1, 0));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArjpumpResID, 1, 0));
    field_10_resources_array.SetAt(14, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArjponceResID, 1, 0));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArjclimbResID, 1, 0));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArjeatResID, 1, 0));
    field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArjfalrkResID, 1, 0));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArjwaspResID, 1, 0));
    field_10_resources_array.SetAt(9, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArjscrchResID, 1, 0));

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kWebResID);

    const AnimRecord& rec = AnimRec(AnimId::Paramite_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    SetTint(&kParamiteTints_55D73C[0], gMap.mCurrentLevel);

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_110_id = -1;
    field_158_next_brain_ret = -1;
    field_15A_paramite_next_motion = -1;
    field_11C_web_id = -1;
    field_118_meat_id = -1;
    field_120_obj_id = -1;
    field_124_pull_ring_rope_id = -1;
    field_130_timer = 0;
    field_12C_brain_ret = 0;
    field_108_next_motion = 0;
    field_106_current_motion = 0;
    field_154_input = 0;
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = Layer::eLayer_8;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = Layer::eLayer_27;
        field_D6_scale = 1;
    }

    if (!VIsFacingMe(sActiveHero_5C1B68))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }

    switch (pTlv->field_12_entrace_type)
    {
        case Path_Paramite::EntranceType::eNormalSurpriseWeb_1:
            field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
            SetBrain(&Paramite::Brain_3_SurpriseWeb_4851B0);
            break;

        case Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb_2:
            SetBrain(&Paramite::Brain_9_ParamiteSpawn_48ED80);
            field_BC_ypos -= FP_FromInteger(20);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
            break;

        case Path_Paramite::EntranceType::eUnused_ScaleToLeftGridSize_3:
            SetBrain(&Paramite::Brain_9_ParamiteSpawn_48ED80);
            field_B8_xpos -= ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
            break;

        case Path_Paramite::EntranceType::eUnused_ScaleToRightGridSize_4:
            SetBrain(&Paramite::Brain_9_ParamiteSpawn_48ED80);
            field_B8_xpos += ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
            break;

        default:
            SetBrain(&Paramite::Brain_0_Patrol_4835B0);
            break;
    }

    field_136_alone_chase_delay = pTlv->field_14_alone_chase_delay;
    field_12E_surprise_web_delay_timer = pTlv->field_16_surprise_web_delay_timer;
    field_134_meat_eating_time = pTlv->field_18_meat_eating_time;
    field_144_group_chase_delay = pTlv->field_1A_group_chase_delay;
    field_14C_id = pTlv->field_1E_id;

    field_178_flags.Set(Flags_178::eBit1_hiss_before_attack, pTlv->field_20_hiss_before_attack == Choice_short::eYes_1);
    field_178_flags.Clear(Flags_178::eBit2_running);
    field_178_flags.Set(Flags_178::eBit4_out_of_sight, pTlv->field_22_delete_when_out_of_sight == Choice_short::eYes_1);
    field_178_flags.Clear(Flags_178::eBit5_prevent_depossession);
    field_178_flags.Set(Flags_178::eBit8_bAttack_fleeches, pTlv->field_24_bAttack_fleeches == Choice_short::eYes_1);
    field_178_flags.Clear(Flags_178::eBit6_spawned);
    field_178_flags.Clear(Flags_178::eBit7_alerted);

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
            field_D6_scale != 0 ? 1 : 16)
        == 1)
    {
        field_F8_LastLineYPos = hitY;
        field_BC_ypos = hitY;
        field_106_current_motion = 0;
        CheckForPlatform_489EA0();
    }

    field_F8_LastLineYPos = field_BC_ypos;
    field_140_tlvInfo = tlvInfo;
    MapFollowMe(TRUE);

    VStackOnObjectsOfType(AETypes::eParamite_96);

    field_DC_bApplyShadows |= 2u;
    field_15C_paramite_xOffset = field_DA_xOffset;

    field_E0_pShadow = ae_new<Shadow>();
}

const AnimId sParamiteAnimIdTable_55D660[44] = {
    AnimId::Paramite_Idle,
    AnimId::Paramite_WalkBegin,
    AnimId::Paramite_Walking,
    AnimId::Paramite_Running,
    AnimId::Paramite_Turn,
    AnimId::Paramite_Hop,
    AnimId::Paramite_Unused,
    AnimId::Paramite_WalkRunTransition,
    AnimId::Paramite_WalkEnd,
    AnimId::Paramite_RunBegin,
    AnimId::Paramite_RunEnd,
    AnimId::Paramite_Falling,
    AnimId::Paramite_JumpUpBegin,
    AnimId::Paramite_JumpUpMidair,
    AnimId::Paramite_JumpUpLand,
    AnimId::Paramite_RopePull,
    AnimId::Paramite_CloseAttack,
    AnimId::Paramite_Landing,
    AnimId::Paramite_Unused2,
    AnimId::Paramite_Knockback,
    AnimId::Paramite_GameSpeakBegin,
    AnimId::Paramite_PreHiss,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_AllOYaGameSpeakBegin,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_PostHiss,
    AnimId::Paramite_GameSpeakEnd,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_PostHiss,
    AnimId::Paramite_RunningAttack,
    AnimId::Paramite_SurpriseWeb,
    AnimId::Paramite_SurpriseWeb,
    AnimId::Paramite_WebLeaveDown,
    AnimId::Paramite_WebIdle,
    AnimId::Paramite_WebGoingUp,
    AnimId::Paramite_WebGoingDown,
    AnimId::Paramite_WebGrab,
    AnimId::Paramite_WebLeaveUp,
    AnimId::Paramite_Eating,
    AnimId::Paramite_Death,
    AnimId::Paramite_Squawk,
    AnimId::Paramite_Attack};

s32 Paramite::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Paramite_State*>(pBuffer);
    auto pTlv = static_cast<Path_Paramite*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_3C_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArjbasicResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("PARAMITE.BND", nullptr);
    }

    auto pParamite = ae_new<Paramite>(pTlv, pState->field_3C_tlvInfo);

    if (pState->field_76_flags.Get(Paramite_State::eBit1_unused))
    {
        sControlledCharacter_5C1B8C = pParamite;
    }

    pParamite->field_FC_pPathTLV = nullptr;
    pParamite->field_100_pCollisionLine = nullptr;

    pParamite->field_B8_xpos = pState->field_4_xpos;
    pParamite->field_BC_ypos = pState->field_8_ypos;

    pParamite->field_C4_velx = pState->field_C_velx;
    pParamite->field_C8_vely = pState->field_10_vely;

    pParamite->field_13C_velx_offset = pState->field_64_velx_offset;
    pParamite->field_C0_path_number = pState->field_14_path_number;
    pParamite->field_C2_lvl_number = pState->field_16_lvl_number;
    pParamite->field_CC_sprite_scale = pState->field_18_sprite_scale;

    pParamite->field_D0_r = pState->field_1C_r;
    pParamite->field_D2_g = pState->field_1E_g;
    pParamite->field_D4_b = pState->field_20_b;

    pParamite->field_106_current_motion = pState->field_24_current_motion;
    const AnimRecord& animRec = AnimRec(sParamiteAnimIdTable_55D660[pParamite->field_106_current_motion]);
    u8** ppRes = pParamite->ResBlockForMotion_488130(pParamite->field_106_current_motion);
    pParamite->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, ppRes);

    pParamite->field_20_animation.field_92_current_frame = pState->field_26_anim_current_frame;
    pParamite->field_20_animation.field_E_frame_change_counter = pState->field_28_frame_change_counter;

    pParamite->mFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2B_drawable & 1);

    pParamite->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_22_flip_x & 1);
    pParamite->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2A_render & 1);

    if (IsLastFrame(&pParamite->field_20_animation))
    {
        pParamite->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pParamite->field_10C_health = pState->field_2C_health;
    pParamite->field_106_current_motion = pState->field_30_current_motion;
    pParamite->field_108_next_motion = pState->field_32_next_motion;
    pParamite->field_F8_LastLineYPos = FP_FromInteger(pState->field_34_last_line_ypos);
    pParamite->field_114_flags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
    pParamite->field_104_collision_line_type = pState->field_36_line_type;

    pParamite->field_118_meat_id = pState->field_40_meat_id;
    pParamite->field_11C_web_id = pState->field_44_web_id;
    pParamite->field_120_obj_id = pState->field_48_obj_id;
    pParamite->field_124_pull_ring_rope_id = pState->field_4C_pull_ring_rope_id;

    pParamite->SetBrain(sParamite_brain_table_55D710[pState->field_50_brain_idx]);

    pParamite->field_12C_brain_ret = pState->field_58_brain_ret;
    pParamite->field_130_timer = pState->field_5C_timer;
    pParamite->field_138_depossession_timer = pState->field_60_depossession_timer;
    pParamite->field_13C_velx_offset = pState->field_64_velx_offset;

    pParamite->field_140_tlvInfo = pState->field_3C_tlvInfo;
    pParamite->field_148_timer = pState->field_68_timer;

    pParamite->field_14E_return_level = pState->field_6C_return_level;
    pParamite->field_150_return_path = pState->field_6E_return_path;
    pParamite->field_152_return_camera = pState->field_70_return_camera;

    pParamite->field_154_input = InputObject::PsxButtonsToKeyboardInput_45EE40(pState->field_72_input);
    pParamite->field_158_next_brain_ret = pState->field_74_next_brain_ret;

    pParamite->field_178_flags.Set(Flags_178::eBit2_running, pState->field_76_flags.Get(Paramite_State::eBit2_running));
    pParamite->field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen, pState->field_76_flags.Get(Paramite_State::eBit3_hissed_or_left_screen));
    pParamite->field_178_flags.Set(Flags_178::eBit5_prevent_depossession, pState->field_76_flags.Get(Paramite_State::eBit4_prevent_depossession));
    pParamite->field_178_flags.Set(Flags_178::eBit6_spawned, pState->field_76_flags.Get(Paramite_State::eBit5_spawned));
    pParamite->field_178_flags.Set(Flags_178::eBit7_alerted, pState->field_76_flags.Get(Paramite_State::eBit6_alerted));
    pParamite->field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed, pState->field_76_flags.Get(Paramite_State::eBit7_can_be_possessed));

    return sizeof(Paramite_State);
}

static s32 ResolveId(s32 objId)
{
    if (objId != -1)
    {
        BaseGameObject* pObj = sObjectIds.Find_449CF0(objId);
        if (pObj)
        {
            return pObj->field_C_objectId;
        }
    }
    return -1;
}

s32 Paramite::VGetSaveState(u8* pSaveBuffer)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Paramite_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eParamite_96;
    pState->field_4_xpos = field_B8_xpos;
    pState->field_8_ypos = field_BC_ypos;
    pState->field_C_velx = field_C4_velx;
    pState->field_10_vely = field_C8_vely;

    pState->field_64_velx_offset = field_13C_velx_offset;

    pState->field_14_path_number = field_C0_path_number;
    pState->field_16_lvl_number = field_C2_lvl_number;
    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_1C_r = field_D0_r;
    pState->field_1E_g = field_D2_g;
    pState->field_20_b = field_D4_b;

    pState->field_22_flip_x = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_24_current_motion = field_106_current_motion;
    pState->field_26_anim_current_frame = field_20_animation.field_92_current_frame;
    pState->field_28_frame_change_counter = field_20_animation.field_E_frame_change_counter;
    pState->field_2B_drawable = mFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2A_render = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
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
    pState->field_76_flags.Set(Paramite_State::eBit1_unused, this == sControlledCharacter_5C1B8C);

    pState->field_3C_tlvInfo = field_140_tlvInfo;
    pState->field_40_meat_id = ResolveId(field_118_meat_id);
    pState->field_44_web_id = ResolveId(field_11C_web_id);
    pState->field_48_obj_id = ResolveId(field_120_obj_id);
    pState->field_4C_pull_ring_rope_id = ResolveId(field_124_pull_ring_rope_id);

    pState->field_50_brain_idx = 0;

    s32 idx = 0;
    for (auto& fn : sParamite_brain_table_55D710)
    {
        if (BrainIs(fn))
        {
            pState->field_50_brain_idx = idx;
            break;
        }
        idx++;
    }

    pState->field_58_brain_ret = field_12C_brain_ret;
    pState->field_5C_timer = field_130_timer;

    pState->field_60_depossession_timer = field_138_depossession_timer;
    pState->field_64_velx_offset = field_13C_velx_offset;

    pState->field_3C_tlvInfo = field_140_tlvInfo;
    pState->field_68_timer = field_148_timer;

    pState->field_6C_return_level = field_14E_return_level;
    pState->field_6E_return_path = field_150_return_path;
    pState->field_70_return_camera = field_152_return_camera;

    pState->field_72_input = InputObject::KeyboardInputToPsxButtons_45EF70(field_154_input);
    pState->field_74_next_brain_ret = field_158_next_brain_ret;

    pState->field_76_flags.Set(Paramite_State::eBit2_running, field_178_flags.Get(Flags_178::eBit2_running));
    pState->field_76_flags.Set(Paramite_State::eBit3_hissed_or_left_screen, field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen));
    pState->field_76_flags.Set(Paramite_State::eBit4_prevent_depossession, field_178_flags.Get(Flags_178::eBit5_prevent_depossession));
    pState->field_76_flags.Set(Paramite_State::eBit5_spawned, field_178_flags.Get(Flags_178::eBit6_spawned));
    pState->field_76_flags.Set(Paramite_State::eBit6_alerted, field_178_flags.Get(Flags_178::eBit7_alerted));
    pState->field_76_flags.Set(Paramite_State::eBit7_can_be_possessed, field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed));

    return sizeof(Paramite_State);
}

const s16 sParamite_patrol_motion_table_55D590[6] = {
    eParamiteMotions::M_Hiss1_22_48C3E0,
    eParamiteMotions::M_Turn_4_48B180,
    eParamiteMotions::M_Hiss1_22_48C3E0,
    eParamiteMotions::M_Hiss1_22_48C3E0,
    eParamiteMotions::M_Turn_4_48B180,
    eParamiteMotions::M_Idle_0_489FB0};

namespace ParamiteEnums {

enum Brain_0_Patrol
{
    eBrain0_Inactive_0 = 0,
    eBrain0_IdleForAbe_1 = 1,
    eBrain0_FearingAbe_2 = 2,
    eBrain0_RunningFromAbe_3 = 3,
    eBrain0_ApproachingAbe_4 = 4,
    eBrain0_StopApproachingAbe_5 = 5,
    eBrain0_TurningForAbe_6 = 6,
    eBrain0_HittingAbe_7 = 7,
    eBrain0_StuckToWall_8 = 8,
    eBrain0_Attacking_9 = 9,
    eBrain0_LostInvisibleAbeSight_10 = 10,
    eBrain0_TurningAfterLoosingSight_11 = 11,
    eBrain0_Idle_12 = 12,
    eBrain0_IdleAnimation_13 = 13,
    eBrain0_Turning_14 = 14,
    eBrain0_LookingForInvisibleAbe_15 = 15,
    eBrain0_Panic_16 = 16
};

enum Brain_2_ChasingAbe
{
    eBrain2_Inactive_0 = 0,
    eBrain2_Attacking_1 = 1,
    eBrain2_ToWarning_2 = 2,
    eBrain2_Warning_3 = 3,
    eBrain2_CloseAttack_4 = 4,
    eBrain2_ToChasing_5 = 5,
    eBrain2_QuickAttack_6 = 6,
    eBrain2_Chasing_7 = 7,
    eBrain2_Jumping_8 = 8,
    eBrain2_TurningWhileChasing_9 = 9,
    eBrain2_Turning_10 = 10,
    eBrain2_Walking_11 = 11,
    eBrain2_WalkingToHop_12 = 12,
    eBrain2_Eating_13 = 13,
    eBrain2_Idle_14 = 14,
    eBrain2_Panic_15 = 15
};

enum Brain_3_SurpriseWeb
{
    eBrain3_Loading_0 = 0,
    eBrain3_Appearing_1 = 1,
    eBrain3_StartAnimation_2 = 2,
    eBrain3_StateLoop1_3 = 3,
    eBrain3_StateLoop2_4 = 4
};

enum Brain_5_SpottedMeat
{
    eBrain5_Idle_0 = 0,
    eBrain5_Running_1 = 1,
    eBrain5_Walking_2 = 2,
    eBrain5_Jumping_3 = 3,
    eBrain5_Turning_4 = 4,
    eBrain5_AttentiveToMeat_5 = 5,
    eBrain5_Eating_6 = 6
};

enum Brain_8_ControlledByGameSpeak
{
    eBrain8_Inactive_0 = 0,
    eBrain8_FollowingIdle_1 = 1,
    eBrain8_Falling_2 = 2,
    eBrain8_Jumping_3 = 3,
    eBrain8_FollowingIdleToWalking_4 = 4,
    eBrain8_FollowingWalking_5 = 5,
    eBrain8_FollowingRunning_6 = 6,
    eBrain8_IdleToWorking_7 = 7,
    eBrain8_Working_8 = 8,
    eBrain8_Idle_9 = 9,
    eBrain8_RecievedCommand_10 = 10,
    eBrain8_Speaking_11 = 11
};

enum Brain_9_ParamiteSpawn
{
    eBrain9_Inactive_0 = 0,
    eBrain9_PreSpawn_1 = 1,
    eBrain9_SlowDescend_2 = 2,
    eBrain9_SlowerDescend_3 = 3,
    eBrain9_DescendLoop1_4 = 4,
    eBrain9_DescendLoop2_5 = 5
};

} // namespace ParamiteEnums

s16 Paramite::Brain_0_Patrol_4835B0()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (!pObj)
    {
        field_120_obj_id = -1;
    }

    if (gMap.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    switch (field_12C_brain_ret)
    {
        case ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0:
            if (sNum_CamSwappers_5C1B66 > 0)
            {
                return field_12C_brain_ret;
            }
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1:
            return Brain_Patrol_State_1_IdleForAbe(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_FearingAbe_2:
            return Brain_Patrol_State_2_FearingAbe(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_RunningFromAbe_3:
            return Brain_Patrol_State_3_RunningFromAbe(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_ApproachingAbe_4:
            return Brain_Patrol_State_4_ApproachingAbe(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_StopApproachingAbe_5:
            return Brain_Patrol_State_5_StopApproachingAbe(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_TurningForAbe_6:
            if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_12C_brain_ret;
            }
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7:
            if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_12C_brain_ret;
            }
            field_108_next_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_StuckToWall_8;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_StuckToWall_8:
            return Brain_Patrol_State_8_StuckToWall(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_Attacking_9:
            if (field_106_current_motion != eParamiteMotions::M_RunningAttack_31_48C9E0 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_12C_brain_ret;
            }
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            if (pObj->field_10C_health > FP_FromInteger(0))
            {
                return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }
            SetBrain(&Paramite::Brain_2_ChasingAbe_4859D0);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10:
            if (field_130_timer > static_cast<s32>(sGnFrame_5C1B84))
            {
                return field_12C_brain_ret;
            }
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_TurningAfterLoosingSight_11;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_TurningAfterLoosingSight_11:
            if (field_130_timer > static_cast<s32>(sGnFrame_5C1B84))
            {
                return field_12C_brain_ret;
            }
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12:
            return Brain_Patrol_State_12_Idle(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_IdleAnimation_13:
            if (field_106_current_motion != eParamiteMotions::M_PreHiss_21_48C180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_12C_brain_ret;
            }
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            field_138_depossession_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 135);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_Turning_14:
            if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_12C_brain_ret;
            }
            field_138_depossession_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 135);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_LookingForInvisibleAbe_15:
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
            {
                return field_12C_brain_ret;
            }
            return ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_Panic_16:
            if (!Event_Get_422C00(kEventAbeOhm))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12;
            }
            field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
            return field_12C_brain_ret;

        default:
            return field_12C_brain_ret;
    }
}

s16 Paramite::Brain_Patrol_State_12_Idle(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    auto pMeat = FindMeat_488930();
    if (pMeat)
    {
        SetBrain(&Paramite::Brain_5_SpottedMeat_486880);
        field_118_meat_id = pMeat->field_8_object_id;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
    }

    if (pObj || FindTarget_488C30())
    {
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (Event_Get_422C00(kEventAbeOhm))
    {
        field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Panic_16;
    }

    const auto pEventNoise = Event_Is_Event_In_Range_422C30(kEventNoise, field_B8_xpos, field_BC_ypos, field_D6_scale);
    const auto pEventSpeaking = Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, field_D6_scale);

    if (sActiveHero_5C1B68 == pEventNoise || sActiveHero_5C1B68 == pEventSpeaking)
    {
        if (sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
        {
            field_108_next_motion = sParamite_patrol_motion_table_55D590[Math_RandomRange_496AB0(0, 4)];
            return ParamiteEnums::Brain_0_Patrol::eBrain0_LookingForInvisibleAbe_15;
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

    auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(AETypes::eFleech_50, gridBlockAHead, field_BC_ypos));
    if (pFleech && VIsFacingMe(pFleech))
    {
        field_108_next_motion = eParamiteMotions::M_Attack_43_48DB70;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LookingForInvisibleAbe_15;
    }

    const GameSpeakEvents lastSpeak = LastSpeak_489040();
    if (lastSpeak == GameSpeakEvents::Paramite_Howdy_48)
    {
        if (sControlledCharacter_5C1B8C->Type() == AETypes::eParamite_96 && sControlledCharacter_5C1B8C->field_10C_health > FP_FromInteger(0) && sControlledCharacter_5C1B8C->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            if (CanIAcceptAGameSpeakCommand_489160())
            {
                field_178_flags.Set(Flags_178::eBit7_alerted);
                SetBrain(&Paramite::Brain_8_ControlledByGameSpeak_48DFC0);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Inactive_0;
            }
        }
    }
    else if (lastSpeak == GameSpeakEvents::Paramite_AllYa_52)
    {
        if (sControlledCharacter_5C1B8C->Type() == AETypes::eParamite_96 && sControlledCharacter_5C1B8C->field_10C_health > FP_FromInteger(0) && sControlledCharacter_5C1B8C->field_CC_sprite_scale == field_CC_sprite_scale)
        {
            SetBrain(&Paramite::Brain_8_ControlledByGameSpeak_48DFC0);
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Inactive_0;
        }
    }

    if (VOnSameYLevel(sControlledCharacter_5C1B8C))
    {
        if (sControlledCharacter_5C1B8C->Type() == AETypes::eParamite_96)
        {
            if (sControlledCharacter_5C1B8C->field_106_current_motion == eParamiteMotions::M_Attack_43_48DB70 || sControlledCharacter_5C1B8C->field_106_current_motion == eParamiteMotions::M_RunningAttack_31_48C9E0)
            {
                auto sControlledParamite = static_cast<Paramite*>(sControlledCharacter_5C1B8C);
                if (sControlledParamite->FindTarget_488C30())
                {
                    field_120_obj_id = sControlledParamite->field_120_obj_id;
                    Sound_48F600(ParamiteSpeak::Howdy_5, 0);
                    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                    SetBrain(&Paramite::Brain_2_ChasingAbe_4859D0);
                    field_148_timer = sGnFrame_5C1B84 + field_144_group_chase_delay;
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
                }
            }
        }
    }

    if (field_138_depossession_timer > static_cast<s32>(sGnFrame_5C1B84))
    {
        if (!field_178_flags.Get(Flags_178::eBit6_spawned) || gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            return field_12C_brain_ret;
        }
        else
        {
            mFlags.Set(BaseGameObject::eDead);
            return field_12C_brain_ret;
        }
    }

    if (Math_NextRandom() >= 6u)
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Turning_14;
    }
    else
    {
        field_108_next_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleAnimation_13;
    }
}

s16 Paramite::Brain_Patrol_State_5_StopApproachingAbe(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (!pObj || !VOnSameYLevel(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (AnotherParamiteNear_4886E0())
    {
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::Brain_2_ChasingAbe_4859D0);
        field_148_timer = sGnFrame_5C1B84 + field_144_group_chase_delay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (field_C4_velx < FP_FromInteger(0))
    {
        if (HandleEnemyStopper_4893B0(-2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }
        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine(1, 3))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (HandleEnemyStopper_4893B0(2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }

        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine(0, 3))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }
    }

    if (!VIsObj_GettingNear(pObj))
    {
        return field_12C_brain_ret;
    }

    if (VIsObjNearby(kGridSize * FP_FromInteger(8), pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (!VIsObjNearby(kGridSize * FP_FromInteger(10), pObj))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
    return ParamiteEnums::Brain_0_Patrol::eBrain0_ApproachingAbe_4;
}

s16 Paramite::Brain_Patrol_State_4_ApproachingAbe(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (!pObj || !VOnSameYLevel(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (AnotherParamiteNear_4886E0())
    {
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::Brain_2_ChasingAbe_4859D0);
        field_148_timer = sGnFrame_5C1B84 + field_144_group_chase_delay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (field_C4_velx < FP_FromInteger(0))
    {
        if (HandleEnemyStopper_4893B0(-2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }

        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine(1, 2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (HandleEnemyStopper_4893B0(2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }

        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine(0, 2))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }
    }

    if (VIsObj_GettingNear(pObj) || (VIsObjNearby(kGridSize * FP_FromInteger(10), pObj)))
    {
        if (!VIsObj_GettingNear(pObj))
        {
            return field_12C_brain_ret;
        }
        if (!VIsObjNearby(kGridSize * FP_FromInteger(8), pObj))
        {
            return field_12C_brain_ret;
        }
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
    return ParamiteEnums::Brain_0_Patrol::eBrain0_StopApproachingAbe_5;
}

s16 Paramite::Brain_Patrol_State_3_RunningFromAbe(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (!pObj || !VOnSameYLevel(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (field_C4_velx < FP_FromInteger(0))
    {
        if ((WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine(1, 3)))
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if ((WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine(0, 3)))
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
        }
    }

    if (AnotherParamiteNear_4886E0())
    {
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::Brain_2_ChasingAbe_4859D0);
        field_148_timer = sGnFrame_5C1B84 + field_144_group_chase_delay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (VIsObj_GettingNear(pObj))
    {
        return field_12C_brain_ret;
    }

    if (VIsObjNearby(kGridSize * FP_FromInteger(7), pObj))
    {
        return field_12C_brain_ret;
    }

    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
    return ParamiteEnums::Brain_0_Patrol::eBrain0_FearingAbe_2;
}

s16 Paramite::Brain_Patrol_State_8_StuckToWall(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    auto pMeat = FindMeat_488930();
    if (pMeat)
    {
        SetBrain(&Paramite::Brain_5_SpottedMeat_486880);
        field_118_meat_id = pMeat->field_8_object_id;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
    }

    if (!pObj || !VOnSameYLevel(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (VIsObjNearby(kGridSize * FP_FromInteger(3), pObj))
    {
        field_108_next_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Attacking_9;
    }

    if (VIsObj_GettingNear(pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
    }
    else
    {
        if (!VIsObjNearby(kGridSize * FP_FromInteger(8), pObj))
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_ApproachingAbe_4;
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

s16 Paramite::Brain_Patrol_State_1_IdleForAbe(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    auto pFoundMeat = FindMeat_488930();
    if (pFoundMeat)
    {
        SetBrain(&Paramite::Brain_5_SpottedMeat_486880);
        field_118_meat_id = pFoundMeat->field_8_object_id;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
    }

    if (!pObj || !VOnSameYLevel(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        if (!FindTarget_488C30())
        {
            field_138_depossession_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 135);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12;
        }
        pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
    }

    if (pObj && pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (Event_Get_422C00(kEventAbeOhm) && VIsFacingMe(pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Panic_16;
    }

    if (HandleEnemyStopper_4893B0(0))
    {
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if ((AnotherParamiteNear_4886E0() || pObj->field_10C_health <= FP_FromInteger(0)) && field_CC_sprite_scale == pObj->field_CC_sprite_scale)
    {
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::Brain_2_ChasingAbe_4859D0);
        field_148_timer = sGnFrame_5C1B84 + field_144_group_chase_delay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    // mid range - warn abe if cornered or wait for new movement
    if (VIsObjNearby(kGridSize * FP_FromInteger(6), pObj))
    {
        if (VIsFacingMe(pObj))
        {
            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine(1, 2))
                {
                    field_108_next_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
                    return ParamiteEnums::Brain_0_Patrol::eBrain0_StuckToWall_8;
                }
            }
            else if (Check_IsOnEndOfLine(0, 2))
            {
                field_108_next_motion = eParamiteMotions::M_GameSpeakBegin_20_48C010;
                return ParamiteEnums::Brain_0_Patrol::eBrain0_StuckToWall_8;
            }
            Sound_48F600(ParamiteSpeak::Howdy_5, 0);
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_TurningForAbe_6;
        }

        // close range - run away or attack if cornered
        if (VIsObjNearby(kGridSize * FP_FromInteger(4), pObj))
        {
            if ((!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) || !Check_IsOnEndOfLine(1, 2)) && (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) || !Check_IsOnEndOfLine(0, 2)))
            {
                field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
                return ParamiteEnums::Brain_0_Patrol::eBrain0_RunningFromAbe_3;
            }
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
        }
        else
        {
            if ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine(1, 2)) || (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine(0, 2)))
            {
                field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
                return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
            }
            else
            {
                field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
                return ParamiteEnums::Brain_0_Patrol::eBrain0_FearingAbe_2;
            }
        }
    }

    // far range - follow abe
    if (!VIsObjNearby(kGridSize * FP_FromInteger(8), pObj))
    {
        if (!VIsFacingMe(pObj))
        {
            if ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine(0, 2)) || (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine(1, 2)))
            {
                field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
                return ParamiteEnums::Brain_0_Patrol::eBrain0_TurningForAbe_6;
            }
        }
        else if ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine(1, 2)) || (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && !Check_IsOnEndOfLine(0, 2)))
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_ApproachingAbe_4;
        }
    }

    if (VIsFacingMe(pObj))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
    return ParamiteEnums::Brain_0_Patrol::eBrain0_TurningForAbe_6;
}

s16 Paramite::Brain_Patrol_State_2_FearingAbe(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (!pObj || !VOnSameYLevel(pObj) || field_CC_sprite_scale != pObj->field_CC_sprite_scale)
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (field_C4_velx < FP_FromInteger(0))
    {
        if ((WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine(1, 2)))
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if ((WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine(0, 2)))
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
        }
    }

    if (AnotherParamiteNear_4886E0())
    {
        Sound_48F600(ParamiteSpeak::Howdy_5, 0);
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::Brain_2_ChasingAbe_4859D0);
        field_148_timer = sGnFrame_5C1B84 + field_144_group_chase_delay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (VIsObj_GettingNear(pObj) && VIsObjNearby(kGridSize * FP_FromInteger(4), pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
        return ParamiteEnums::Brain_0_Patrol::eBrain0_RunningFromAbe_3;
    }

    if (VIsObj_GettingNear(pObj))
    {
        return field_12C_brain_ret;
    }

    if (VIsObjNearby(kGridSize * FP_FromInteger(7), pObj))
    {
        return field_12C_brain_ret;
    }

    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
}

s16 Paramite::Brain_1_Death_484CD0()
{
    if (!field_100_pCollisionLine || field_100_pCollisionLine->field_8_type == eLineTypes::eTrackLine_8)
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
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                field_100_pCollisionLine = ppPathLine;
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                CheckForPlatform_489EA0();
                break;
            default:
                break;
        }
    }

    if (field_130_timer <= static_cast<s32>(sGnFrame_5C1B84) || field_130_timer >= static_cast<s32>(sGnFrame_5C1B84 + 80))
    {
        if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            mFlags.Set(BaseGameObject::eDead);
        }
    }
    else
    {
        field_CC_sprite_scale -= FP_FromDouble(0.01);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;

        if (field_CC_sprite_scale >= FP_FromDouble(0.3))
        {
            DeathSmokeEffect(true);
        }
    }

    if (sControlledCharacter_5C1B8C == this)
    {
        if (field_130_timer < static_cast<s32>(sGnFrame_5C1B84))
        {
            sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
            gMap.SetActiveCam_480D30(field_14E_return_level, field_150_return_path, field_152_return_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
    }

    if (sControlledCharacter_5C1B8C != this)
    {
        if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            mFlags.Set(BaseGameObject::eDead);
        }
    }

    if (field_CC_sprite_scale <= FP_FromInteger(0) || field_130_timer < static_cast<s32>(sGnFrame_5C1B84))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    return 100;
}

s16 Paramite::Brain_2_ChasingAbe_4859D0()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));

    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (pObj && !pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible) && (pObj->Type() != AETypes::eFleech_50 || pObj->field_10C_health > FP_FromInteger(0)))
    {
        if (field_148_timer > static_cast<s32>(sGnFrame_5C1B84) || (VOnSameYLevel(pObj) && field_CC_sprite_scale == pObj->field_CC_sprite_scale))
        {
            if (gMap.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
            {
                MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
            }

            if (Event_Get_422C00(kEventAbeOhm))
            {
                field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Panic_15;
            }

            switch (field_12C_brain_ret)
            {
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0:
                    return Brain_ChasingAbe_State_0_Inactive(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Attacking_1:
                    return Brain_ChasingAbe_State_1_Attacking(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToWarning_2:
                    return Brain_ChasingAbe_State_2_ToWarning();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Warning_3:
                    return Brain_ChasingAbe_State_3_Warning();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_CloseAttack_4:
                    return Brain_ChasingAbe_State_4_CloseAttack();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5:
                    return Brain_ChasingAbe_State_5_ToChasing(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_QuickAttack_6:
                    return Brain_ChasingAbe_State_6_QuickAttack();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Chasing_7:
                    return Brain_ChasingAbe_State_7_Chasing(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8:
                    return Brain_ChasingAbe_State_8_Jumping();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_TurningWhileChasing_9:
                    return Brain_ChasingAbe_State_9_TurningWhileChasing();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Turning_10:
                    return Brain_ChasingAbe_State_10_Turning(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11:
                    return Brain_ChasingAbe_State_11_Walking(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_WalkingToHop_12:
                    return Brain_ChasingAbe_State_12_WalkingToHop(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13:
                    return Brain_ChasingAbe_State_13_Eating();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Idle_14:
                    return Brain_ChasingAbe_State_14_Idle();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Panic_15:
                    return Brain_ChasingAbe_State_15_Panic();
                default:
                    return field_12C_brain_ret;
            }
        }
    }
    else
    {
        field_120_obj_id = -1;
    }

    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    SetBrain(&Paramite::Brain_0_Patrol_4835B0);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
}

s16 Paramite::Brain_ChasingAbe_State_15_Panic()
{
    if (Event_Get_422C00(kEventAbeOhm))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
}

s16 Paramite::Brain_ChasingAbe_State_14_Idle()
{
    if (field_130_timer > static_cast<s32>(sGnFrame_5C1B84))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
    field_130_timer = sGnFrame_5C1B84 + 28;
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
}

s16 Paramite::Brain_ChasingAbe_State_9_TurningWhileChasing()
{
    if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
}

s16 Paramite::Brain_ChasingAbe_State_13_Eating()
{
    if (field_130_timer > static_cast<s32>(sGnFrame_5C1B84))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    field_130_timer = sGnFrame_5C1B84 + 15;
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Idle_14;
}

s16 Paramite::Brain_ChasingAbe_State_8_Jumping()
{
    if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
    {
        return field_12C_brain_ret;
    }
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
}

s16 Paramite::Brain_ChasingAbe_State_6_QuickAttack()
{
    if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        return field_12C_brain_ret;
    }

    if (field_178_flags.Get(Flags_178::eBit1_hiss_before_attack))
    {
        field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
    }
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
}

s16 Paramite::Brain_ChasingAbe_State_3_Warning()
{
    if (field_106_current_motion != eParamiteMotions::M_Hiss1_22_48C3E0)
    {
        return field_12C_brain_ret;
    }
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
}

s16 Paramite::Brain_ChasingAbe_State_4_CloseAttack()
{
    if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        return field_12C_brain_ret;
    }

    if (field_178_flags.Get(Flags_178::eBit1_hiss_before_attack))
    {
        field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(0, 6);
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToWarning_2;
    }
    else
    {
        field_130_timer = sGnFrame_5C1B84 + field_136_alone_chase_delay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
    }
}

s16 Paramite::Brain_ChasingAbe_State_2_ToWarning()
{
    if (field_130_timer > static_cast<s32>(sGnFrame_5C1B84))
    {
        return field_12C_brain_ret;
    }
    field_108_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
    field_130_timer = sGnFrame_5C1B84 + field_136_alone_chase_delay;
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Warning_3;
}

s16 Paramite::Brain_ChasingAbe_State_11_Walking(BaseAliveGameObject* pObj)
{
    if (!VIsFacingMe(pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Turning_10;
    }

    if (!VOnSameYLevel(pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (VIsObjNearby(kGridSize, pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
        field_130_timer = sGnFrame_5C1B84 + 28;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
    }

    const FP xSnapped = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (FP_Abs(field_B8_xpos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine(0, 1))
        {
            ToHop_489C20();
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_WalkingToHop_12;
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        if (FP_Abs(field_B8_xpos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine(1, 1))
        {
            ToHop_489C20();
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_WalkingToHop_12;
        }
    }
    return field_12C_brain_ret;
}

s16 Paramite::Brain_ChasingAbe_State_7_Chasing(BaseAliveGameObject* pObj)
{
    if (!VOnSameYLevel(pObj) || WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }
    else
    {
        if (!VIsFacingMe(pObj))
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_TurningWhileChasing_9;
        }

        const FP xSnapped = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
        if (field_C4_velx < FP_FromInteger(0))
        {
            if (HandleEnemyStopper_4893B0(-2))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
            }

            if (FP_Abs(field_B8_xpos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine(1, 1))
            {
                ToHop_489C20();
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
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
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
            }

            if (FP_Abs(field_B8_xpos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine(0, 1))
            {
                ToHop_489C20();
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
            }
            else
            {
                // Drop through to idle/attack case
            }
        }

        const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

        if (!VIsObjNearby(kGridSize * FP_FromInteger(4), pObj))
        {
            return field_12C_brain_ret;
        }

        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Attacking_1;
        }
    }
}

s16 Paramite::Brain_ChasingAbe_State_12_WalkingToHop(BaseAliveGameObject* pObj)
{
    if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
    {
        return field_12C_brain_ret;
    }

    if (VIsFacingMe(pObj))
    {
        const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

        if (VIsObjNearby(kGridSize, pObj))
        {
            field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
            field_130_timer = sGnFrame_5C1B84 + 28;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
        }
    }
    else
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Turning_10;
    }
}

s16 Paramite::Brain_ChasingAbe_State_10_Turning(BaseAliveGameObject* pObj)
{
    if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        return field_12C_brain_ret;
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (VIsObjNearby(kGridSize, pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
        field_130_timer = sGnFrame_5C1B84 + 28;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
    }

    if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (!Check_IsOnEndOfLine(0, 1))
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (!Check_IsOnEndOfLine(1, 1))
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
        }
    }

    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
}

s16 Paramite::Brain_ChasingAbe_State_5_ToChasing(BaseAliveGameObject* pObj)
{
    if (!VOnSameYLevel(pObj))
    {
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (!VIsFacingMe(pObj))
    {
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_QuickAttack_6;
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (HandleEnemyStopper_4893B0(-2))
        {
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
        }
    }
    else if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (HandleEnemyStopper_4893B0(2))
        {
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
        }
    }

    if (VIsObjNearby(kGridSize, pObj))
    {
        field_108_next_motion = eParamiteMotions::M_CloseAttack_16_48DDA0;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (VIsObjNearby(kGridSize * FP_FromInteger(4), pObj))
    {
        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Attacking_1;
        }
    }

    if (field_130_timer > static_cast<s32>(sGnFrame_5C1B84) && field_178_flags.Get(Flags_178::eBit1_hiss_before_attack))
    {
        return field_12C_brain_ret;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine(1, 1))
        {
            field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
        }
    }
    else if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine(0, 1))
        {
            field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
        }
    }

    field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Chasing_7;
}

s16 Paramite::Brain_ChasingAbe_State_1_Attacking(BaseAliveGameObject* pObj)
{
    if (field_106_current_motion != eParamiteMotions::M_RunningAttack_31_48C9E0)
    {
        if (field_108_next_motion != eParamiteMotions::M_RunningAttack_31_48C9E0)
        {
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
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
        if (VIsFacingMe(pObj))
        {
            if (VIsObjNearby(kGridSize * FP_FromInteger(4), pObj))
            {
                if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                {
                    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
                }
                else
                {
                    field_108_next_motion = eParamiteMotions::M_RunningAttack_31_48C9E0;
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Attacking_1;
                }
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }

            field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Chasing_7;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_QuickAttack_6;
        }
    }
    else
    {
        if (VIsFacingMe(pObj))
        {
            if (VIsObjNearby(kGridSize, pObj))
            {
                field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
                field_130_timer = sGnFrame_5C1B84 + 28;
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Turning_10;
        }
    }
}

s16 Paramite::Brain_ChasingAbe_State_0_Inactive(BaseAliveGameObject* pObj)
{
    if (!VOnSameYLevel(pObj))
    {
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
    {
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (pObj->field_10C_health > FP_FromInteger(0))
    {
        if (VIsFacingMe(pObj))
        {
            if (field_178_flags.Get(Flags_178::eBit1_hiss_before_attack))
            {
                field_130_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(0, 6);
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToWarning_2;
            }
            else
            {
                field_130_timer = sGnFrame_5C1B84 + field_136_alone_chase_delay;
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
            }
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_CloseAttack_4;
        }
    }
    else
    {
        if (VIsFacingMe(pObj))
        {
            const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

            if (VIsObjNearby(kGridSize, pObj))
            {
                field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
                field_130_timer = sGnFrame_5C1B84 + 28;
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
        }
        else
        {
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Turning_10;
        }
    }
}

s16 Paramite::Brain_3_SurpriseWeb_4851B0()
{
    auto pExistingWeb = static_cast<ParamiteWeb*>(sObjectIds.Find_449CF0(field_11C_web_id));
    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    switch (field_12C_brain_ret)
    {
        case ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_Loading_0:
            if (sNum_CamSwappers_5C1B66 <= 0)
            {
                field_106_current_motion = eParamiteMotions::M_Idle_0_489FB0;
                MapFollowMe(TRUE);
            }
            return ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_Appearing_1;

        case ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_Appearing_1:
            if (field_14C_id != 0 && SwitchStates_Get(field_14C_id))
            {
                field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
                field_130_timer = sGnFrame_5C1B84 + field_12E_surprise_web_delay_timer;
                auto pNewWeb = ae_new<ParamiteWeb>(field_B8_xpos, FP_GetExponent(field_BC_ypos) - 20, FP_GetExponent(field_BC_ypos) - 10, field_CC_sprite_scale);
                if (pNewWeb)
                {
                    field_11C_web_id = pNewWeb->field_8_object_id;
                }
                return ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StartAnimation_2;
            }
            break;

        case ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StartAnimation_2:
            if (field_130_timer <= static_cast<s32>(sGnFrame_5C1B84))
            {
                field_C8_vely = FP_FromInteger(0);
                field_106_current_motion = eParamiteMotions::M_SurpriseWeb_33_48D760;
                return ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StateLoop1_3;
            }
            break;

        case ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StateLoop1_3:
            pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(field_BC_ypos)) - 10;
            pExistingWeb->field_BC_ypos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
            if (field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
            {
                auto pWeb = static_cast<ParamiteWeb*>(sObjectIds.Find_449CF0(field_11C_web_id));
                pWeb->field_104_bEnabled = TRUE;
                field_11C_web_id = -1;
                SetBrain(&Paramite::Brain_0_Patrol_4835B0);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
            }

            if (field_C8_vely < (field_CC_sprite_scale * FP_FromInteger(8)))
            {
                field_C8_vely = (field_CC_sprite_scale * FP_FromDouble(0.5)) + field_C8_vely;
                return field_12C_brain_ret;
            }
            return ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StateLoop2_4;

        case ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StateLoop2_4:
            pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(field_BC_ypos)) - 10;
            pExistingWeb->field_BC_ypos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
            if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
            {
                if (field_C8_vely <= (field_CC_sprite_scale * FP_FromInteger(-1)))
                {
                    return ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StateLoop1_3;
                }
                else
                {
                    field_C8_vely = field_C8_vely - (field_CC_sprite_scale * FP_FromInteger(1));
                    return field_12C_brain_ret;
                }
            }
            else
            {
                auto pWeb = static_cast<ParamiteWeb*>(sObjectIds.Find_449CF0(field_11C_web_id));
                pWeb->field_104_bEnabled = TRUE;
                field_11C_web_id = -1;
                SetBrain(&Paramite::Brain_0_Patrol_4835B0);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
            }
            break;

        default:
            break;
    }

    return field_12C_brain_ret;
}

s16 Paramite::Brain_4_Unused_48F8F0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
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

    SetBrain(&Paramite::Brain_0_Patrol_4835B0);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
}

s16 Paramite::Brain_5_SpottedMeat_486880()
{
    auto pMeat = static_cast<Meat*>(sObjectIds.Find_449CF0(field_118_meat_id));

    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (!pMeat || pMeat->VIsFalling())
    {
        Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
        field_118_meat_id = -1;
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        SetBrain(&Paramite::Brain_0_Patrol_4835B0);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
    }

    if (pMeat->field_130_pLine)
    {
        if (FP_Abs(pMeat->field_BC_ypos - field_BC_ypos) > FP_FromInteger(20))
        {
            field_118_meat_id = -1;
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            SetBrain(&Paramite::Brain_0_Patrol_4835B0);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
        }
    }

    if (gMap.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    switch (field_12C_brain_ret)
    {
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0:
            return Brain_SpottedMeat_State_0_Idle(pMeat);
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Running_1:
            return Brain_SpottedMeat_State_1_Running(pMeat);
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2:
            return Brain_SpottedMeat_State_2_Walking(pMeat);
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3:
            return Brain_SpottedMeat_State_3_Jumping();
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4:
            return Brain_SpottedMeat_State_4_Turning(pMeat);
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5:
            return Brain_SpottedMeat_State_5_AttentiveToMeat(pMeat);
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Eating_6:
            return Brain_SpottedMeat_State_6_Eating(pMeat);
        default:
            return field_12C_brain_ret;
    }
}

s16 Paramite::Brain_SpottedMeat_State_6_Eating(Meat* pMeat)
{
    if (!VIsObjNearby(field_CC_sprite_scale * FP_FromInteger(40), pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
    }

    if (!VIsFacingMe(pMeat))
    {
        if (FP_Abs(pMeat->field_B8_xpos - field_B8_xpos) > FP_FromInteger(5))
        {
            Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
        }
    }

    if (field_130_timer > static_cast<s32>(sGnFrame_5C1B84))
    {
        return field_12C_brain_ret;
    }

    pMeat->mFlags.Set(BaseGameObject::eDead);
    field_118_meat_id = -1;
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    SetBrain(&Paramite::Brain_0_Patrol_4835B0);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
}

s16 Paramite::Brain_SpottedMeat_State_5_AttentiveToMeat(Meat* pMeat)
{
    if (!VIsFacingMe(pMeat))
    {
        Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
    }

    if (VIsObj_GettingNear(pMeat))
    {
        return field_12C_brain_ret;
    }


    if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine(0, 1))
        {
            ToHop_489C20();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine(1, 1))
        {
            ToHop_489C20();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (!VIsObjNearby(kGridSize * FP_FromInteger(3), pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Running_1;
    }

    if (!VIsObjNearby(field_CC_sprite_scale * FP_FromInteger(40), pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
    }

    if (!pMeat->field_130_pLine || !field_100_pCollisionLine)
    {
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
    }

    field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
    field_130_timer = sGnFrame_5C1B84 + field_134_meat_eating_time;
    return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Eating_6;
}

s16 Paramite::Brain_SpottedMeat_State_4_Turning(Meat* pMeat)
{
    if (field_106_current_motion != eParamiteMotions::M_Turn_4_48B180 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        return field_12C_brain_ret;
    }

    if (!VIsFacingMe(pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
    }

    if (FP_Abs(pMeat->field_B8_xpos - field_B8_xpos) > FP_FromInteger(5))
    {
        Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
        field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
    }
    else
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
    }
}

s16 Paramite::Brain_SpottedMeat_State_3_Jumping()
{
    if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
    {
        return field_12C_brain_ret;
    }
    return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
}

s16 Paramite::Brain_SpottedMeat_State_2_Walking(Meat* pMeat)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const s32 xPos_int = FP_GetExponent(field_B8_xpos);
    const s32 xSnapped = SnapToXGrid_449930(field_CC_sprite_scale, xPos_int);

    if (field_C4_velx < FP_FromInteger(0))
    {
        if (std::abs(xPos_int - xSnapped) < 6 && Check_IsOnEndOfLine(1, 1))
        {
            ToHop_489C20();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (std::abs(xPos_int - xSnapped) < 6 && Check_IsOnEndOfLine(0, 1))
        {
            ToHop_489C20();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    if (!VIsFacingMe(pMeat))
    {
        if (FP_Abs(pMeat->field_B8_xpos - field_B8_xpos) > FP_FromInteger(5))
        {
            Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
        }
    }

    if (!VIsObjNearby(field_CC_sprite_scale * FP_FromInteger(40), pMeat))
    {
        return field_12C_brain_ret;
    }

    if (pMeat->field_130_pLine)
    {
        field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
        field_130_timer = sGnFrame_5C1B84 + field_134_meat_eating_time;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Eating_6;
    }
    return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
}

s16 Paramite::Brain_SpottedMeat_State_1_Running(Meat* pMeat)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    const s32 xAsInt = FP_GetExponent(field_B8_xpos);
    const s32 xSnapped = SnapToXGrid_449930(field_CC_sprite_scale, xAsInt);

    if (field_C4_velx < FP_FromInteger(0))
    {
        if (std::abs(xAsInt - xSnapped) < 6 && Check_IsOnEndOfLine(1, 1))
        {
            ToHop_489C20();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    if (field_C4_velx > FP_FromInteger(0))
    {
        if (std::abs(xAsInt - xSnapped) < 6 && Check_IsOnEndOfLine(0, 1))
        {
            ToHop_489C20();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    if (!VIsFacingMe(pMeat))
    {
        if (FP_Abs(pMeat->field_B8_xpos - field_B8_xpos) > FP_FromInteger(5))
        {
            Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
        }
    }

    if (VIsObjNearby(field_CC_sprite_scale * FP_FromInteger(40), pMeat))
    {
        if (pMeat->field_130_pLine)
        {
            field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
            field_130_timer = sGnFrame_5C1B84 + field_134_meat_eating_time;
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Eating_6;
        }
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Running_1;
    }

    if (!VIsObjNearby(kGridSize * FP_FromInteger(3), pMeat))
    {
        return field_12C_brain_ret;
    }

    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
    return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
}

s16 Paramite::Brain_SpottedMeat_State_0_Idle(Meat* pMeat)
{
    if (!VIsFacingMe(pMeat))
    {
        if (FP_Abs(pMeat->field_B8_xpos - field_B8_xpos) > FP_FromInteger(5))
        {
            Sound_48F600(ParamiteSpeak::DetectedMeat_7, 0);
            field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
        }
    }

    if (VIsObj_GettingNear(pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine(0, 1))
        {
            ToHop_489C20();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (Check_IsOnEndOfLine(1, 1))
        {
            ToHop_489C20();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    if (!VIsObjNearby(kGridSize * FP_FromInteger(3), pMeat))
    {
        field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Running_1;
    }

    if (VIsObjNearby(field_CC_sprite_scale * FP_FromInteger(40), pMeat))
    {
        if (field_B8_xpos == pMeat->field_B8_xpos)
        {
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
        }

        if (!pMeat->field_130_pLine || !field_100_pCollisionLine)
        {
            field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
        }

        field_108_next_motion = eParamiteMotions::M_Eating_40_48A0F0;
        field_130_timer = sGnFrame_5C1B84 + field_134_meat_eating_time;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Eating_6;
    }

    field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
    return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
}

s16 Paramite::Brain_6_Possessed_484BC0()
{
    if (field_12C_brain_ret == 1)
    {
        if (gMap.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
        {
            MusicController::PlayMusic_47FD60(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
        }

        if (Event_Get_422C00(kEventDeathReset))
        {
            if (sControlledCharacter_5C1B8C != this)
            {
                mFlags.Set(BaseGameObject::eDead);
            }
        }

        if (field_10C_health <= FP_FromInteger(0))
        {
            SetBrain(&Paramite::Brain_1_Death_484CD0);
            field_130_timer = sGnFrame_5C1B84 + 90;
            return field_12C_brain_ret;
        }
    }
    else if (field_12C_brain_ret == 0)
    {
        if (static_cast<s32>(sGnFrame_5C1B84) >= field_130_timer)
        {
            field_12C_brain_ret = 1;
            field_10C_health = FP_FromInteger(1);
        }
    }

    return field_12C_brain_ret;
}

s16 Paramite::Brain_7_DeathDrop_484FF0()
{
    if (field_12C_brain_ret == 0)
    {
        field_130_timer = sGnFrame_5C1B84 + 60;
        return 1;
    }
    else if (field_12C_brain_ret == 1)
    {
        if (static_cast<s32>(sGnFrame_5C1B84) < field_130_timer)
        {
            if (static_cast<s32>(sGnFrame_5C1B84) == field_130_timer - 6)
            {
                SND_SEQ_Play_4CAB10(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
            }
            return field_12C_brain_ret;
        }

        Environment_SFX_457A40(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);

        ae_new<ScreenShake>(0, 0);
        field_130_timer = sGnFrame_5C1B84 + 30;
        return 2;
    }
    else
    {
        if (field_12C_brain_ret != 2 || static_cast<s32>(sGnFrame_5C1B84) <= field_130_timer)
        {
            return field_12C_brain_ret;
        }

        if (sControlledCharacter_5C1B8C == this)
        {
            sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
            gMap.SetActiveCam_480D30(field_14E_return_level, field_150_return_path, field_152_return_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }

        mFlags.Set(BaseGameObject::eDead);
        return field_12C_brain_ret;
    }
}

s16 Paramite::Brain_8_ControlledByGameSpeak_48DFC0()
{
    if (sControlledCharacter_5C1B8C->Type() != AETypes::eParamite_96 || sControlledCharacter_5C1B8C->field_10C_health <= FP_FromInteger(0) || sControlledCharacter_5C1B8C->field_CC_sprite_scale != field_CC_sprite_scale)
    {
        SetBrain(&Paramite::Brain_0_Patrol_4835B0);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
    }
    if (VOnSameYLevel(sControlledCharacter_5C1B8C))
    {
        if (sControlledCharacter_5C1B8C->field_106_current_motion == eParamiteMotions::M_Attack_43_48DB70 || sControlledCharacter_5C1B8C->field_106_current_motion == eParamiteMotions::M_RunningAttack_31_48C9E0)
        {
            if (static_cast<Paramite*>(sControlledCharacter_5C1B8C)->FindTarget_488C30())
            {
                field_120_obj_id = static_cast<Paramite*>(sControlledCharacter_5C1B8C)->field_120_obj_id;
                Sound_48F600(ParamiteSpeak::Howdy_5, 0);
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                SetBrain(&Paramite::Brain_2_ChasingAbe_4859D0);
                field_148_timer = sGnFrame_5C1B84 + field_144_group_chase_delay;
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
            }
        }
    }

    switch (field_12C_brain_ret)
    {
        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Inactive_0:
        {
            field_178_flags.Clear(Flags_178::eBit7_alerted);
            s16 result = VIsFacingMe(sControlledCharacter_5C1B8C);
            if (result)
            {
                field_108_next_motion = eParamiteMotions::M_Empty_24_48C680;
                field_158_next_brain_ret = 9;
                result = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Speaking_11;
            }
            else
            {
                if (field_106_current_motion == eParamiteMotions::M_Turn_4_48B180)
                {
                    result = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Inactive_0;
                }
                else
                {
                    field_108_next_motion = eParamiteMotions::M_Turn_4_48B180;
                }
            }
            return result;
        }

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1:
            if (VIsFacingMe(sControlledCharacter_5C1B8C))
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

                auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(AETypes::eFleech_50, gridBlock, field_BC_ypos));
                if (pFleech && VIsFacingMe(pFleech))
                {
                    field_108_next_motion = eParamiteMotions::M_Attack_43_48DB70;
                    field_158_next_brain_ret = 1;
                    return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Speaking_11;
                }

                const GameSpeakEvents lastSpeak = LastSpeak_489040();
                switch (lastSpeak)
                {
                    case GameSpeakEvents::Paramite_Stay_49:
                        field_15A_paramite_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
                        field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                        field_158_next_brain_ret = 9;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    case GameSpeakEvents::Paramite_Howdy_48:
                    case GameSpeakEvents::Paramite_AllYa_52:
                        field_15A_paramite_next_motion = eParamiteMotions::M_Empty_24_48C680;
                        field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                        field_158_next_brain_ret = 1;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    case GameSpeakEvents::Paramite_DoIt_51:
                        field_15A_paramite_next_motion = eParamiteMotions::M_Hiss3_26_48C6F0;
                        field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                        field_158_next_brain_ret = 7;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    default:
                        if (!VIsObjNearby(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), sControlledCharacter_5C1B8C))
                        {
                            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                            {
                                if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -ScaleToGridSize_4498B0(field_CC_sprite_scale)))
                                {
                                    if (!Check_IsOnEndOfLine(1, 1) && field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
                                    {
                                        field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdleToWalking_4;
                                    }
                                    if (!Check_IsOnEndOfLine(1, 4) && VOnSameYLevel(sControlledCharacter_5C1B8C))
                                    {
                                        field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Jumping_3;
                                    }
                                }
                            }
                            else if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                            {
                                if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), ScaleToGridSize_4498B0(field_CC_sprite_scale)))
                                {
                                    if (!Check_IsOnEndOfLine(0, 1) && field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
                                    {
                                        field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdleToWalking_4;
                                    }
                                }

                                if (!Check_IsOnEndOfLine(0, 4) && VOnSameYLevel(sControlledCharacter_5C1B8C))
                                {
                                    field_108_next_motion = eParamiteMotions::M_Hop_5_48B5B0;
                                    return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Jumping_3;
                                }
                            }
                        }

                        if (lastSpeak == GameSpeakEvents::Paramite_CMon_or_Attack_50)
                        {
                            field_15A_paramite_next_motion = eParamiteMotions::M_Hiss2_23_48C600;
                            field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                            field_158_next_brain_ret = 2;
                            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;
                        }
                        else if (sControlledCharacter_5C1B8C->field_106_current_motion != eParamiteMotions::M_Attack_43_48DB70)
                        {
                            return field_12C_brain_ret;
                        }
                        else
                        {
                            field_15A_paramite_next_motion = eParamiteMotions::M_Attack_43_48DB70;
                            field_158_next_brain_ret = 1;
                            field_130_timer = sGnFrame_5C1B84 + 10;
                            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;
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
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            break;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Falling_2:
            if (!VIsFacingMe(sControlledCharacter_5C1B8C))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }

            if (field_106_current_motion != eParamiteMotions::M_Walking_2_48A2D0)
            {
                field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Falling_2;
            }

            if (field_C4_velx < FP_FromInteger(0) && (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -ScaleToGridSize_4498B0(field_CC_sprite_scale)) || Check_IsOnEndOfLine(1, 2)))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            else if (field_C4_velx > FP_FromInteger(0) && (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), ScaleToGridSize_4498B0(field_CC_sprite_scale)) || Check_IsOnEndOfLine(0, 2)))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            else if (!VIsObjNearby(ScaleToGridSize_4498B0(field_CC_sprite_scale), sControlledCharacter_5C1B8C))
            {
                return field_12C_brain_ret;
            }
            else
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            break;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Jumping_3:
            if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
            {
                return field_12C_brain_ret;
            }
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdleToWalking_4:
            if (field_130_timer > static_cast<s32>(sGnFrame_5C1B84))
            {
                return field_12C_brain_ret;
            }
            field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingWalking_5;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingWalking_5:
            if (!VIsFacingMe(sControlledCharacter_5C1B8C))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }

            if (field_C4_velx < FP_FromInteger(0))
            {
                if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -ScaleToGridSize_4498B0(field_CC_sprite_scale)) || Check_IsOnEndOfLine(1, 2))
                {
                    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                    return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                }
            }
            else if (field_C4_velx > FP_FromInteger(0))
            {
                if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), ScaleToGridSize_4498B0(field_CC_sprite_scale)) || Check_IsOnEndOfLine(0, 2))
                {
                    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                    return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                }
            }

            if (VIsObjNearby(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), sControlledCharacter_5C1B8C))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }

            if (VIsObjNearby(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(5), sControlledCharacter_5C1B8C))
            {
                return field_12C_brain_ret;
            }

            if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
            {
                if (Check_IsOnEndOfLine(0, 3))
                {
                    return field_12C_brain_ret;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(1, 3))
                {
                    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        return field_12C_brain_ret;
                    }

                    if (Check_IsOnEndOfLine(0, 3))
                    {
                        return field_12C_brain_ret;
                    }
                }
            }
            field_108_next_motion = eParamiteMotions::M_Running_3_48AA00;
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingRunning_6;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingRunning_6:
            if (!VIsFacingMe(sControlledCharacter_5C1B8C))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            if (field_C4_velx < FP_FromInteger(0) && (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), -ScaleToGridSize_4498B0(field_CC_sprite_scale)) || Check_IsOnEndOfLine(1, 3)))
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            else
            {
                if (field_C4_velx > FP_FromInteger(0) && ((WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), ScaleToGridSize_4498B0(field_CC_sprite_scale))) || Check_IsOnEndOfLine(0, 3)))
                {
                    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                    return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                }
                else
                {
                    if (VIsObjNearby(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), sControlledCharacter_5C1B8C))
                    {
                        field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                    }
                    else
                    {
                        if (!VIsObjNearby(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(5), sControlledCharacter_5C1B8C))
                        {
                            return field_12C_brain_ret;
                        }
                        field_108_next_motion = eParamiteMotions::M_Walking_2_48A2D0;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingWalking_5;
                    }
                }
            }
            break;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_IdleToWorking_7:
            if (field_106_current_motion == eParamiteMotions::M_JumpUpBegin_12_48BE40)
            {
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Working_8;
            }
            field_108_next_motion = eParamiteMotions::M_JumpUpBegin_12_48BE40;
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_IdleToWorking_7;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Working_8:
            if (field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
            {
                SetBrain(&Paramite::Brain_0_Patrol_4835B0);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
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

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Idle_9:
            if (VIsFacingMe(sControlledCharacter_5C1B8C))
            {
                const GameSpeakEvents lastSpeak = LastSpeak_489040();
                switch (lastSpeak)
                {
                    case GameSpeakEvents::Paramite_CMon_or_Attack_50:
                        field_15A_paramite_next_motion = eParamiteMotions::M_Hiss2_23_48C600;
                        field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                        field_158_next_brain_ret = 1;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    case GameSpeakEvents::Paramite_Stay_49:
                        field_15A_paramite_next_motion = eParamiteMotions::M_Hiss1_22_48C3E0;
                        field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                        field_158_next_brain_ret = 9;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    case GameSpeakEvents::Paramite_Howdy_48:
                    case GameSpeakEvents::Paramite_AllYa_52:
                        field_15A_paramite_next_motion = eParamiteMotions::M_Empty_24_48C680;
                        field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                        field_158_next_brain_ret = 9;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    case GameSpeakEvents::Paramite_DoIt_51:
                        field_15A_paramite_next_motion = eParamiteMotions::M_Hiss3_26_48C6F0;
                        field_130_timer = sGnFrame_5C1B84 + StableDelay_48DF80();
                        field_158_next_brain_ret = 7;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

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
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Idle_9;
            }
            break;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10:
            if (field_130_timer > static_cast<s32>(sGnFrame_5C1B84))
            {
                return field_12C_brain_ret;
            }
            else
            {
                field_108_next_motion = field_15A_paramite_next_motion;
                field_15A_paramite_next_motion = -1;
            }
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Speaking_11;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Speaking_11:
            if (field_106_current_motion == eParamiteMotions::M_Hiss2_23_48C600 || field_106_current_motion == eParamiteMotions::M_Hiss1_22_48C3E0 || field_106_current_motion == eParamiteMotions::M_Hiss3_26_48C6F0)
            {
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
            }

            if (field_106_current_motion != eParamiteMotions::M_Idle_0_489FB0)
            {
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Speaking_11;
            }
            else
            {
                return field_158_next_brain_ret;
            }
            break;

        default:
            return field_12C_brain_ret;
    }
}

s16 Paramite::Brain_9_ParamiteSpawn_48ED80()
{
    auto pExistingWeb = static_cast<ParamiteWeb*>(sObjectIds.Find_449CF0(field_11C_web_id));
    switch (field_12C_brain_ret)
    {
        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_Inactive_0:
            if (field_14C_id)
            {
                field_12C_brain_ret = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_PreSpawn_1;
            }
            else
            {
                if (field_100_pCollisionLine != nullptr)
                {
                    field_12C_brain_ret = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_SlowDescend_2;
                }
                else
                {
                    field_12C_brain_ret = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_SlowerDescend_3;
                }
            }
            break;

        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_PreSpawn_1:
            if (SwitchStates_Get(field_14C_id))
            {
                field_178_flags.Set(Flags_178::eBit6_spawned);
                SFX_Play_Mono(SoundEffect::ParamiteSpawn_110, 0);
                field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

                if (!field_100_pCollisionLine)
                {
                    field_C8_vely = FP_FromInteger(0);
                    field_106_current_motion = eParamiteMotions::M_SurpriseWeb_33_48D760;
                    auto pWeb = ae_new<ParamiteWeb>(field_B8_xpos, FP_GetExponent(field_BC_ypos) - 20,
                                                    FP_GetExponent(field_BC_ypos) - 10,
                                                    field_CC_sprite_scale);
                    if (pWeb)
                    {
                        field_11C_web_id = pWeb->field_8_object_id;
                    }
                    field_12C_brain_ret = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop1_4;
                }
                else
                {
                    SetBrain(&Paramite::Brain_8_ControlledByGameSpeak_48DFC0);
                    field_12C_brain_ret = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                }
            }
            else
            {
                field_12C_brain_ret = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_PreSpawn_1;
            }
            break;

        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_SlowDescend_2:
            if (field_160_last_event_index != pEventSystem_5BC11C->field_28_last_event_index)
            {
                field_160_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
                if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Paramite_Howdy_48)
                {
                    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
                    SetBrain(&Paramite::Brain_8_ControlledByGameSpeak_48DFC0);
                    field_12C_brain_ret = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                }
            }
            break;

        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_SlowerDescend_3:
            if (field_160_last_event_index != pEventSystem_5BC11C->field_28_last_event_index)
            {
                field_160_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;

                if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Paramite_Howdy_48)
                {
                    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
                    field_C8_vely = FP_FromInteger(0);
                    field_106_current_motion = eParamiteMotions::M_SurpriseWeb_33_48D760;
                    auto pWeb = ae_new<ParamiteWeb>(field_B8_xpos, FP_GetExponent(field_BC_ypos) - 20,
                                                    FP_GetExponent(field_BC_ypos) - 10,
                                                    field_CC_sprite_scale);
                    if (pWeb)
                    {
                        field_11C_web_id = pWeb->field_8_object_id;
                    }
                    field_12C_brain_ret = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop1_4;
                }
            }
            break;

        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop1_4:
            pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(field_BC_ypos)) - 10;
            pExistingWeb->field_BC_ypos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
            if (field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
            {
                field_12C_brain_ret = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                pExistingWeb->field_104_bEnabled = 1;
                field_11C_web_id = -1;
                SetBrain(&Paramite::Brain_8_ControlledByGameSpeak_48DFC0);
            }
            else
            {
                if (field_C8_vely < (field_CC_sprite_scale * FP_FromInteger(8)))
                {
                    field_C8_vely = (field_CC_sprite_scale * FP_FromDouble(0.5)) + field_C8_vely;
                    return field_12C_brain_ret;
                }
                field_12C_brain_ret = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop2_5;
            }
            break;

        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop2_5:
            pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(field_BC_ypos)) - 10;
            pExistingWeb->field_BC_ypos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
            if (field_106_current_motion == eParamiteMotions::M_Idle_0_489FB0)
            {
                field_12C_brain_ret = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                pExistingWeb->field_104_bEnabled = 1;
                field_11C_web_id = -1;
                SetBrain(&Paramite::Brain_8_ControlledByGameSpeak_48DFC0);
            }
            else
            {
                if (field_C8_vely > (field_CC_sprite_scale * FP_FromInteger(-1)))
                {
                    field_C8_vely = field_C8_vely - (field_CC_sprite_scale * FP_FromInteger(1));
                    return field_12C_brain_ret;
                }
                field_12C_brain_ret = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop1_4;
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

    if (Input_IsChanting_45F260() && !field_178_flags.Get(Flags_178::eBit5_prevent_depossession))
    {
        field_106_current_motion = eParamiteMotions::M_GetDepossessedBegin_29_48D9D0;
        field_138_depossession_timer = sGnFrame_5C1B84 + 30;
        SFX_Play_Mono(SoundEffect::PossessEffect_17, 0);
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

const FP sWalkTable_5464BC[3] = {
    FP_FromInteger(0),
    FP_FromDouble(1.402099609375),
    FP_FromDouble(9.025436401367188)};

void Paramite::M_WalkBegin_1_48A7B0()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -sWalkTable_5464BC[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = sWalkTable_5464BC[field_20_animation.field_92_current_frame];
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

const FP sWalkTable_546484[14] = {
    FP_FromDouble(2.03741455078125),
    FP_FromDouble(4.02911376953125),
    FP_FromDouble(4.46575927734375),
    FP_FromDouble(3.3473663330078125),
    FP_FromDouble(3.1887054443359375),
    FP_FromDouble(4.937652587890625),
    FP_FromDouble(3.126708984375),
    FP_FromDouble(2.50823974609375),
    FP_FromDouble(4.440032958984375),
    FP_FromDouble(3.9687652587890625),
    FP_FromDouble(2.9050140380859375),
    FP_FromDouble(3.6402130126953125),
    FP_FromDouble(4.701416015625),
    FP_FromDouble(2.72283935546875)};


void Paramite::M_Walking_2_48A2D0()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -sWalkTable_546484[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = sWalkTable_546484[field_20_animation.field_92_current_frame];
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
        field_154_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
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
                    field_154_input = 0;
                    return;
                }

                if (field_108_next_motion == eParamiteMotions::M_Idle_0_489FB0 || field_108_next_motion == eParamiteMotions::M_GameSpeakBegin_20_48C010 || field_108_next_motion == eParamiteMotions::M_Turn_4_48B180 || field_108_next_motion == eParamiteMotions::M_Squawk_42_48D900 || field_108_next_motion == eParamiteMotions::M_Hiss1_22_48C3E0 || field_108_next_motion == eParamiteMotions::M_Eating_40_48A0F0)
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
                        field_178_flags.Set(Flags_178::eBit2_running);
                        field_F4_previous_motion = eParamiteMotions::M_Running_3_48AA00;
                        field_F6_anim_frame = 11;
                    }

                    field_154_input = 0;

                    if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
                    {
                        UpdateSlurgWatchPoints_4890D0();
                        MapFollowMe(TRUE);
                        field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
                    }
                    return;
                }

                if (field_108_next_motion == eParamiteMotions::M_Running_3_48AA00)
                {
                    field_178_flags.Set(Flags_178::eBit2_running);
                    field_F4_previous_motion = eParamiteMotions::M_Running_3_48AA00;
                    field_F6_anim_frame = 11;
                    field_108_next_motion = -1;
                    if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
                    {
                        UpdateSlurgWatchPoints_4890D0();
                        MapFollowMe(TRUE);
                        field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
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

                if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
                {
                    UpdateSlurgWatchPoints_4890D0();
                    MapFollowMe(TRUE);
                    field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
                }
                return;

            default:
                field_178_flags.Clear(Flags_178::eBit3_hissed_or_left_screen);
                return;
        }
    }
}

const FP sRunningTable_5464E8[14] = {
    FP_FromDouble(5.1657867431640625),
    FP_FromDouble(5.624664306640625),
    FP_FromDouble(3.328460693359375),
    FP_FromDouble(3.532440185546875),
    FP_FromDouble(5.26165771484375),
    FP_FromDouble(10.463729858398438),
    FP_FromDouble(9.87750244140625),
    FP_FromDouble(8.148956298828125),
    FP_FromDouble(8.294830322265625),
    FP_FromDouble(7.1818389892578125),
    FP_FromDouble(6.7950592041015625),
    FP_FromDouble(8.932144165039062),
    FP_FromDouble(11.460769653320312),
    FP_FromDouble(5.9268646240234375)};

void Paramite::M_Running_3_48AA00()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -sRunningTable_5464E8[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = sRunningTable_5464E8[field_20_animation.field_92_current_frame];
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
        field_154_input = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held | field_154_input;

        if (field_20_animation.field_92_current_frame == 3 || field_20_animation.field_92_current_frame == 10)
        {
            if (field_20_animation.field_92_current_frame == 3)
            {
                SFX_Play_Pitch(SoundEffect::PickupItem_28, 45, -600);
            }
            else
            {
                Sound_48F600(ParamiteSpeak::LoudStep_3, 0);
                Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 50, 600, 0);
            }

            if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
            {
                if (field_108_next_motion != eParamiteMotions::M_Walking_2_48A2D0)
                {
                    if (field_108_next_motion != eParamiteMotions::M_Idle_0_489FB0 && field_108_next_motion != eParamiteMotions::M_Turn_4_48B180 && field_108_next_motion != eParamiteMotions::M_Eating_40_48A0F0 && field_108_next_motion != eParamiteMotions::M_Squawk_42_48D900 && field_108_next_motion != eParamiteMotions::M_Hiss1_22_48C3E0)
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

                if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
                {
                    UpdateSlurgWatchPoints_4890D0();
                    MapFollowMe(TRUE);
                    field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
                }
            }
            else
            {
                HandleInputRunning_48ADB0();
                field_154_input = 0;
            }

            if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
            {
                UpdateSlurgWatchPoints_4890D0();
                MapFollowMe(TRUE);
                field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
            }
            return;
        }

        if (field_20_animation.field_92_current_frame != 6 && field_20_animation.field_92_current_frame != 13)
        {
            field_178_flags.Clear(Flags_178::eBit3_hissed_or_left_screen);
            return;
        }

        if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
        {
            if (field_154_input & sInputKey_Hop_5550E0)
            {
                field_154_input = 0;
                ToHop_489C20();
            }
        }

        if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
        {
            UpdateSlurgWatchPoints_4890D0();
            MapFollowMe(TRUE);
            field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
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

const FP sHopTable_546544[14] = {
    FP_FromDouble(6.478485107421875),
    FP_FromDouble(6.952423095703125),
    FP_FromDouble(4.5809173583984375),
    FP_FromDouble(4.7915191650390625),
    FP_FromDouble(6.5775146484375),
    FP_FromDouble(12.131378173828125),
    FP_FromDouble(11.163650512695312),
    FP_FromDouble(6.437164306640625),
    FP_FromDouble(6.45751953125),
    FP_FromDouble(5.377593994140625),
    FP_FromDouble(5.9170379638671875),
    FP_FromDouble(7.3821563720703125),
    FP_FromDouble(6.7065582275390625),
    FP_FromDouble(9.063491821289062)};

void Paramite::M_Hop_5_48B5B0()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -sHopTable_546544[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = sHopTable_546544[field_20_animation.field_92_current_frame];
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
            auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(AETypes::eWebLine_146, field_B8_xpos, field_BC_ypos));
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
            const auto bHit = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(0.9));

            if (sControlledCharacter_5C1B8C == this)
            {
                SetActiveCameraDelayedFromDir();
            }

            if (bHit)
            {
                switch (pLine->field_8_type)
                {
                    case eLineTypes::eFloor_0:
                    case eLineTypes::eBackgroundFloor_4:
                    case eLineTypes::eUnknown_32:
                    case eLineTypes::eUnknown_36:
                        field_100_pCollisionLine = pLine;
                        field_B8_xpos = hitX;
                        field_BC_ypos = hitY;
                        ToIdle_489B70();
                        CheckForPlatform_489EA0();
                        UpdateSlurgWatchPoints_4890D0();
                        return;

                    case eLineTypes::eWallLeft_1:
                    case eLineTypes::eWallRight_2:
                        field_B8_xpos = hitX - field_C4_velx;
                        MapFollowMe(TRUE);
                        field_C4_velx = FP_FromInteger(0);
                        field_BC_ypos = hitY;
                        return;

                    default:
                        break;
                }
            }

            if (field_BC_ypos - field_F8_LastLineYPos > FP_FromInteger(5))
            {
                field_13C_velx_offset = FP_FromDouble(0.75);
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
                SFX_Play_Pitch(SoundEffect::PickupItem_28, 50, -500);
            }
            else if (field_20_animation.field_92_current_frame == 12)
            {
                Sound_48F600(ParamiteSpeak::LoudStep_3, 0);
                Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 55, 500, 0);
            }
        }
    }
}

const FP M_Unused_6_VelTable_5464D4[5] = {
    FP_FromDouble(2.2522430419921875),
    FP_FromDouble(5.6306304931640625),
    FP_FromDouble(7.3198089599609375),
    FP_FromDouble(7.3198089599609375),
    FP_FromDouble(5.6306304931640625)};

void Paramite::M_Unused_6_48A930()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -M_Unused_6_VelTable_5464D4[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = M_Unused_6_VelTable_5464D4[field_20_animation.field_92_current_frame];
    }

    field_C4_velx = (field_CC_sprite_scale * frameVelX);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_178_flags.Set(Flags_178::eBit2_running);
        field_F4_previous_motion = eParamiteMotions::M_Running_3_48AA00;
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

const FP sWalkToRunTable_546538[3] = {
    FP_FromDouble(5.3964691162109375),
    FP_FromDouble(5.396453857421875),
    FP_FromDouble(5.396484375)};

void Paramite::M_WalkRunTransition_7_48B0C0()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -sWalkToRunTable_546538[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = sWalkToRunTable_546538[field_20_animation.field_92_current_frame];
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

const FP sWalkEndTable_5464C8[3] = {
    FP_FromDouble(2.3350982666015625),
    FP_FromDouble(5.0355224609375),
    FP_FromDouble(6.70159912109375)};

void Paramite::M_WalkEnd_8_48A870()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -sWalkEndTable_5464C8[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = sWalkEndTable_5464C8[field_20_animation.field_92_current_frame];
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

const FP sRunBeginTable_546520[3] = {
    FP_FromDouble(1.8789520263671875),
    FP_FromDouble(2.15924072265625),
    FP_FromDouble(3.330413818359375)};

void Paramite::M_RunBegin_9_48AF10()
{
    field_154_input |= sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = (field_CC_sprite_scale * -sRunBeginTable_546520[field_20_animation.field_92_current_frame]);
    }
    else
    {
        field_C4_velx = (field_CC_sprite_scale * sRunBeginTable_546520[field_20_animation.field_92_current_frame]);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_178_flags.Set(Flags_178::eBit2_running);
        field_F4_previous_motion = eParamiteMotions::M_Running_3_48AA00;
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

const FP sRunEndTable_54652C[3] = {
    FP_FromDouble(3.11126708984375),
    FP_FromDouble(3.063446044921875),
    FP_FromDouble(3.43255615234375)};

void Paramite::M_RunEnd_10_48B000()
{
    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -sRunEndTable_54652C[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = sRunEndTable_54652C[field_20_animation.field_92_current_frame];
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
        field_C4_velx = field_C4_velx - (field_CC_sprite_scale * field_13C_velx_offset);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx = (field_CC_sprite_scale * field_13C_velx_offset) + field_C4_velx;
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
        auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(AETypes::eWebLine_146, field_B8_xpos, field_BC_ypos));
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
        const auto bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

        if (sControlledCharacter_5C1B8C == this)
        {
            SetActiveCameraDelayedFromDir();
        }

        if (bCollision)
        {
            switch (pLine->field_8_type)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eUnknown_32:
                case eLineTypes::eUnknown_36:
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
                        SetBrain(&Paramite::Brain_1_Death_484CD0);
                        field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
                        field_130_timer = sGnFrame_5C1B84 + 90;

                        ae_new<Blood>(field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 50);
                    }
                    break;

                case eLineTypes::eWallLeft_1:
                case eLineTypes::eWallRight_2:
                case eLineTypes::eBackgroundWallLeft_5:
                case eLineTypes::eBackgroundWallRight_6:
                    field_B8_xpos = hitX - field_C4_velx;
                    field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
                    field_BC_ypos = hitY;
                    field_C4_velx = FP_FromInteger(0);
                    MapFollowMe(TRUE);
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
        SFX_Play_Pitch(SoundEffect::PickupItem_28, 50, -500);
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
    const auto bHit = InAirCollision_408810(&ppPathLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (sControlledCharacter_5C1B8C == this)
    {
        SetActiveCameraDelayedFromDir();
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
                field_124_pull_ring_rope_id = pRope->field_8_object_id;
                field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
                return;
            }
            field_124_pull_ring_rope_id = -1;
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

                    auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(AETypes::eWebLine_146, field_B8_xpos, field_BC_ypos));
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
                auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(AETypes::eFleech_50, field_B8_xpos, field_BC_ypos - FP_FromInteger(20)));
                if (pFleech)
                {
                    pFleech->VTakeDamage(this);
                    pFleech->mFlags.Set(BaseGameObject::eDead);
                }
            }
        }
    }

    if ((field_BC_ypos - field_F8_LastLineYPos) > FP_FromInteger(5))
    {
        field_13C_velx_offset = FP_FromDouble(0.75);
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
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 55, 500, 0);
    }
}

void Paramite::M_RopePull_15_48D930()
{
    auto pPullRingRope = static_cast<PullRingRope*>(sObjectIds.Find_449CF0(field_124_pull_ring_rope_id));
    if (!pPullRingRope || ((pPullRingRope && pPullRingRope->VIsNotBeingPulled_49BC90()) && (sControlledCharacter_5C1B8C == this || field_108_next_motion == eParamiteMotions::M_Falling_11_48B200)))
    {
        if (pPullRingRope)
        {
            pPullRingRope->VMarkAsPulled_49B610();
        }
        field_124_pull_ring_rope_id = -1;
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
                pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
                field_120_obj_id = -1;
            }
        }
        else
        {
            pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
        }

        if (pObj)
        {
            PSX_RECT otherRect = {};
            pObj->VGetBoundingRect(&otherRect, 1);

            PSX_RECT ourRect = {};
            VGetBoundingRect(&ourRect, 1);

            s16 right = 0;
            s16 left = 0;
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

            if (otherRect.x <= left && otherRect.w >= right && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage(this);
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

void Paramite::M_Unused_18_48DF60()
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
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
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
                SetBrain(&Paramite::Brain_1_Death_484CD0);
                field_130_timer = sGnFrame_5C1B84 + 90;
            }
        }
    }
}

void Paramite::M_GameSpeakBegin_20_48C010()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->VGetBoundingRect(&otherRect, 1);

        PSX_RECT ourRect = {};
        VGetBoundingRect(&ourRect, 1);

        if (otherRect.x <= ourRect.w && otherRect.w >= ourRect.x && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (VIsObj_GettingNear(pObj))
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        if (pObj->field_10C_health > FP_FromInteger(0))
                        {
                            SFX_Play_Mono(SoundEffect::KillEffect_64, 0);
                            Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 0, 0, pObj);
                        }
                        pObj->VTakeDamage(this);
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
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->VGetBoundingRect(&otherRect, 1);

        PSX_RECT ourRect = {};
        VGetBoundingRect(&ourRect, 1);

        if (otherRect.x <= ourRect.w && otherRect.w >= ourRect.x && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (VIsObj_GettingNear(pObj))
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage(this);
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
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->VGetBoundingRect(&otherRect, 1);

        PSX_RECT ourRect = {};
        VGetBoundingRect(&ourRect, 1);

        if (otherRect.x <= ourRect.w && otherRect.w >= ourRect.x && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (VIsObj_GettingNear(pObj))
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }

    if (field_20_animation.field_92_current_frame != 2 || field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
    {
        field_178_flags.Clear(Flags_178::eBit3_hissed_or_left_screen);
    }
    else
    {
        field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
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
    if (field_20_animation.field_92_current_frame != 2 || field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
    {
        field_178_flags.Clear(Flags_178::eBit3_hissed_or_left_screen);
    }
    else
    {
        field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
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
    if (field_20_animation.field_92_current_frame != 2 || field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
    {
        field_178_flags.Clear(Flags_178::eBit3_hissed_or_left_screen);
    }
    else
    {
        field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
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
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->VGetBoundingRect(&otherRect, 1);

        PSX_RECT ourRect = {};
        VGetBoundingRect(&ourRect, 1);

        if (otherRect.x <= ourRect.w && otherRect.w >= ourRect.x && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (VIsObj_GettingNear(pObj))
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage(this);
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
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->VGetBoundingRect(&otherRect, 1);

        PSX_RECT ourRect = {};
        VGetBoundingRect(&ourRect, 1);

        if (otherRect.x <= ourRect.w && otherRect.w >= ourRect.x && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (VIsObj_GettingNear(pObj))
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage(this);
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

        if (!(static_cast<s32>(sGnFrame_5C1B84) % 4))
        {
            New_TintChant_Particle_426BE0(
                (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange_496AB0(-20, 20))) + field_B8_xpos,
                field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange_496AB0(20, 50))),
                field_CC_sprite_scale,
                Layer::eLayer_0);
        }

        if (static_cast<s32>(sGnFrame_5C1B84) > field_138_depossession_timer)
        {
            sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
            field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
            field_106_current_motion = eParamiteMotions::M_GetDepossessedEnd_30_48DB50;
            SetBrain(&Paramite::Brain_0_Patrol_4835B0);
            field_12C_brain_ret = 0;
            gMap.SetActiveCam_480D30(field_14E_return_level, field_150_return_path, field_152_return_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
            if (field_152_return_camera != gMap.mCurrentCamera)
            {
                if (field_178_flags.Get(Flags_178::eBit6_spawned))
                {
                    mFlags.Set(BaseGameObject::eDead);
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
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));

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
            SetActiveCameraDelayedFromDir();
            if (FindTarget_488C30())
            {
                pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
                field_120_obj_id = -1;
            }
        }

        if (pObj)
        {
            PSX_RECT otherRect = {};
            pObj->VGetBoundingRect(&otherRect, 1);

            PSX_RECT ourRect = {};
            VGetBoundingRect(&ourRect, 1);

            s16 right = 0;
            s16 left = 0;
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

            if (otherRect.x <= left && otherRect.w >= right && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
            {
                if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                    {
                        pObj->VTakeDamage(this);
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
            field_D6_scale != 0 ? 1 : 16)
        == 1)
    {
        field_BC_ypos = hitY;
        field_F8_LastLineYPos = hitY;
        field_106_current_motion = eParamiteMotions::M_WebLeaveDown_34_48D870;
        if (field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_32 || field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_36)
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
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 50, 600, 0);
        field_DA_xOffset = field_15C_paramite_xOffset;
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
            auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(AETypes::eWebLine_146, field_B8_xpos, field_BC_ypos));
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
            auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(AETypes::eWebLine_146, field_B8_xpos, field_BC_ypos));
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
        SetActiveCameraDelayedFromDir();
    }

    if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 3)
    {
        Sound_48F600(ParamiteSpeak::ClimbingWeb_6, 0);
    }

    if (!field_100_pCollisionLine || !((1 << field_100_pCollisionLine->field_8_type) & 0x100) || FP_GetExponent(field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(20))) < field_100_pCollisionLine->field_0_rect.y)
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

                auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(AETypes::eWebLine_146, field_B8_xpos, field_BC_ypos));
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
                    field_DA_xOffset = field_15C_paramite_xOffset;
                }

                auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(AETypes::eWebLine_146, field_B8_xpos, field_BC_ypos));
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
        SetActiveCameraDelayedFromDir();
    }

    if (field_20_animation.field_92_current_frame == 0 || field_20_animation.field_92_current_frame == 3)
    {
        Sound_48F600(ParamiteSpeak::ClimbingWeb_6, 0);
    }

    if (!field_100_pCollisionLine || !((1 << field_100_pCollisionLine->field_8_type) & 0x100))
    {
        auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(AETypes::eWebLine_146, field_B8_xpos, field_BC_ypos));
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
                field_DA_xOffset = field_15C_paramite_xOffset;
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
        auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType_425180(AETypes::eWebLine_146, field_B8_xpos, field_BC_ypos));
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
            auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(AETypes::eFleech_50, gridBlock + field_B8_xpos, field_BC_ypos - FP_FromInteger(5)));
            if (pFleech)
            {
                pFleech->VTakeDamage(this);
                pFleech->mFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                auto pSlurg = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(AETypes::eSlurg_129, gridBlock + field_B8_xpos, field_BC_ypos));
                if (pSlurg)
                {
                    ae_new<Blood>(pSlurg->field_B8_xpos, pSlurg->field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 30);
                    pSlurg->mFlags.Set(BaseGameObject::eDead);
                }
                else
                {
                    SFX_Play_Mono(static_cast<SoundEffect>(Math_RandomRange_496AB0(SoundEffect::Eating1_65, SoundEffect::Eating2_66)), 0);
                }
            }
        }
        else
        {
            SFX_Play_Mono(static_cast<SoundEffect>(Math_RandomRange_496AB0(SoundEffect::Eating1_65, SoundEffect::Eating2_66)), 0);
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
    
}

void Paramite::M_Squawk_42_48D900()
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
        pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
        Sound_48F600(ParamiteSpeak::CMon_or_Attack_0, 0);

        if (sControlledCharacter_5C1B8C == this)
        {
            if (FindTarget_488C30())
            {
                pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
                field_120_obj_id = -1;
            }
        }
        else if (field_178_flags.Get(Flags_178::eBit8_bAttack_fleeches))
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
                pObj = static_cast<BaseAliveGameObject*>(FindObjectOfType_425180(AETypes::eFleech_50, gridBlock, field_BC_ypos));
            }
        }
    }

    if (pObj)
    {
        PSX_RECT otherRect = {};
        pObj->VGetBoundingRect(&otherRect, 1);

        PSX_RECT ourRect = {};
        VGetBoundingRect(&ourRect, 1);

        s16 right = 0;
        s16 left = 0;
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
        if (otherRect.x <= left && otherRect.w >= right && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
            {
                if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                {
                    pObj->VTakeDamage(this);
                }
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_489B70();
    }
}

Paramite::~Paramite()
{
    BaseGameObject* pObj = sObjectIds.Find_449CF0(field_11C_web_id);
    if (pObj)
    {
        pObj->mFlags.Set(BaseGameObject::eDead);
        field_11C_web_id = -1;
    }

    field_118_meat_id = -1;
    VOnTrapDoorOpen();

    if (field_10C_health > FP_FromInteger(0) || field_178_flags.Get(Flags_178::eBit6_spawned))
    {
        Path::TLV_Reset_4DB8E0(field_140_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_140_tlvInfo, -1, 0, 1);
    }

    SND_SEQ_Stop_4CAE60(SeqId::ParamiteNearby_25);
    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        if (gMap.mLevel != LevelIds::eMenu_0)
        {
            gMap.SetActiveCam_480D30(
                field_14E_return_level,
                field_150_return_path,
                field_152_return_camera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }
}

void Paramite::VUpdate()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
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
        field_118_meat_id = BaseGameObject::RefreshId(field_118_meat_id);
        field_120_obj_id = BaseGameObject::RefreshId(field_120_obj_id);
        field_124_pull_ring_rope_id = BaseGameObject::RefreshId(field_124_pull_ring_rope_id);

        if (field_11C_web_id != -1)
        {
            auto pWeb = ae_new<ParamiteWeb>(field_B8_xpos,
                                            FP_GetExponent(field_BC_ypos) - 20,
                                            FP_GetExponent(field_BC_ypos) - 10,
                                            field_CC_sprite_scale);
            if (pWeb)
            {
                field_11C_web_id = pWeb->field_8_object_id;
            }
        }
    }

    auto pMeat = static_cast<BaseThrowable*>(sObjectIds.Find_449CF0(field_118_meat_id));
    if (field_10C_health > FP_FromInteger(0)
        && gMap.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
    {
        SND_SEQ_PlaySeq_4CA960(SeqId::ParamiteNearby_25, 1, 0);
    }
    else if (!Find_Paramite_488810())
    {
        SND_SEQ_Stop_4CAE60(SeqId::ParamiteNearby_25);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    const FP xDelta = FP_Abs(field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos);
    const FP yDelta = FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos);

    if (xDelta > FP_FromInteger(750) || yDelta > FP_FromInteger(390))
    {
        if (field_178_flags.Get(Flags_178::eBit4_out_of_sight) && field_178_flags.Get(Flags_178::eBit6_spawned))
        {
            mFlags.Set(BaseGameObject::eDead);
        }
        else if (pMeat)
        {
            if (pMeat->VIsFalling() || pMeat->mFlags.Get(BaseGameObject::eDead))
            {
                field_118_meat_id = -1;
                field_108_next_motion = 0;
                SetBrain(&Paramite::Brain_0_Patrol_4835B0);
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
            if (!BrainIs(&Paramite::Brain_9_ParamiteSpawn_48ED80) || field_178_flags.Get(Flags_178::eBit6_spawned))
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            }
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        }

        if (!Input_IsChanting_45F260())
        {
            field_178_flags.Clear(Flags_178::eBit5_prevent_depossession);
        }


        if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter_5C1B8C == this)
        {
            // Handle DDCheat mode

            VOnTrapDoorOpen();
            field_106_current_motion = 11;
            field_F8_LastLineYPos = field_BC_ypos;
            field_100_pCollisionLine = nullptr;

            static s32 sArray1_5C929C[8] = {};
            static s32 sArray2_5C92BC[8] = {};

            static s8 byte_5C9298 = 0;

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
                gMap.Get_map_size_480640(&mapBounds);
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
                    SetActiveCameraDelayedFromDir();
                    return;
                }
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);
                field_C8_vely = FP_FromInteger(0);
            }
            SetActiveCameraDelayedFromDir();
        }
        else
        {
            const auto oldMotion = field_106_current_motion;
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

            if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
            {
                field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                    nullptr,
                    field_B8_xpos,
                    field_BC_ypos,
                    field_B8_xpos,
                    field_BC_ypos);
                VOn_TLV_Collision(field_FC_pPathTLV);
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
                if (field_178_flags.Get(Flags_178::eBit2_running))
                {
                    field_106_current_motion = field_F4_previous_motion;
                    vUpdateAnim_487170();
                    field_20_animation.SetFrame_409D50(field_F6_anim_frame);
                    field_178_flags.Clear(Flags_178::eBit2_running);
                }
            }
            else
            {
                vUpdateAnim_487170();
            }
        }
    }
}

s16 Paramite::Find_Paramite_488810()
{
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eParamite_96 && pObj != this && gMap.Is_Point_In_Current_Camera_4810D0(pObj->field_C2_lvl_number, pObj->field_C0_path_number, pObj->field_B8_xpos, pObj->field_BC_ypos, 0))
        {
            return 1;
        }
    }
    return 0;
}

void Paramite::vUpdateAnim_487170()
{
    const AnimRecord& animRec = AnimRec(sParamiteAnimIdTable_55D660[field_106_current_motion]);
    u8** ppRes = ResBlockForMotion_488130(field_106_current_motion);
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, ppRes);
}

Meat* Paramite::FindMeat_488930()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eMeat_84)
        {
            auto pMeat = static_cast<Meat*>(pObj);
            if (pMeat->VCanEatMe_4696A0())
            {
                if (gMap.Is_Point_In_Current_Camera_4810D0(pMeat->field_C2_lvl_number, pMeat->field_C0_path_number, pMeat->field_B8_xpos, pMeat->field_BC_ypos, 0) && !WallHit_408750(field_BC_ypos, pMeat->field_B8_xpos - field_B8_xpos))
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

s16 Paramite::IsNear_488B10(Paramite* pOther)
{
    return FP_Abs(pOther->field_BC_ypos - field_BC_ypos) < field_CC_sprite_scale * FP_FromInteger(100);
}

s16 Paramite::vOnSameYLevel_488A40(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther)
    {
        PSX_RECT bRect = {};
        pOther->VGetBoundingRect(&bRect, 1);

        if ((FP_Abs(field_BC_ypos - FP_FromInteger(bRect.h)) < field_CC_sprite_scale * FP_FromInteger(40)) || (pOther->Type() == AETypes::eParamite_96 && static_cast<Paramite*>(pOther)->field_106_current_motion == eParamiteMotions::M_JumpUpMidair_13_48BAF0))
        {
            return 1;
        }
    }
    return 0;
}

void Paramite::VUnPosses()
{
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
    field_130_timer = sGnFrame_5C1B84 + 180;
    SetBrain(&Paramite::Brain_0_Patrol_4835B0);
    field_12C_brain_ret = 0;
}

void Paramite::VPossessed()
{
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_178_flags.Set(Flags_178::eBit5_prevent_depossession);
    SetBrain(&Paramite::Brain_6_Possessed_484BC0);
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    field_12C_brain_ret = 0;
    field_130_timer = sGnFrame_5C1B84 + 30;
    field_14E_return_level = gMap.mCurrentLevel;
    field_150_return_path = gMap.mCurrentPath;
    field_152_return_camera = gMap.mCurrentCamera;
}

s16 Paramite::VTakeDamage(BaseGameObject* pFrom)
{
    if (field_10C_health <= FP_FromInteger(0))
    {
        return 1;
    }

    field_118_meat_id = -1;

    switch (pFrom->Type())
    {
        case AETypes::eDrill_30:
        case AETypes::eBaseBomb_46:
        case AETypes::eExplosion_109:
        {
            Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
            ae_new<Gibs>(GibType::Slog_2, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
            field_10C_health = FP_FromInteger(0);
            mFlags.Set(BaseGameObject::eDead);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            if (sControlledCharacter_5C1B8C != this)
            {
                return 1;
            }
            SND_SEQ_Play_4CAB10(SeqId::DeathDrums_29, 1, 127, 127);
        }
            return 1;

        case AETypes::eFleech_50:
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
                    if (BrainIs(&Paramite::Brain_0_Patrol_4835B0) || BrainIs(&Paramite::Brain_8_ControlledByGameSpeak_48DFC0))
                    {
                        SetBrain(&Paramite::Brain_2_ChasingAbe_4859D0);
                        field_12C_brain_ret = 0;
                        field_148_timer = sGnFrame_5C1B84 + field_144_group_chase_delay;
                        field_120_obj_id = pFrom->field_8_object_id;
                    }
                }
            }

            if (field_10C_health > FP_FromInteger(0))
            {
                return 1;
            }

            Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
            SetBrain(&Paramite::Brain_1_Death_484CD0);
            field_130_timer = sGnFrame_5C1B84 + 90;
            field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
            vUpdateAnim_487170();

            ae_new<Blood>(field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 50);

            if (sControlledCharacter_5C1B8C == this)
            {
                SND_SEQ_Play_4CAB10(SeqId::DeathDrums_29, 1, 127, 127);
            }
        }
            return 0;

        case AETypes::eAbilityRing_104:
            return 0;

        default:
        {
            field_C8_vely = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
            field_10C_health = FP_FromInteger(0);
            SetBrain(&Paramite::Brain_1_Death_484CD0);
            field_130_timer = sGnFrame_5C1B84 + 90;
            field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
            vUpdateAnim_487170();

            ae_new<Blood>(field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 50);

            if (sControlledCharacter_5C1B8C != this)
            {
                return 1;
            }

            SND_SEQ_Play_4CAB10(SeqId::DeathDrums_29, 1, 127, 127);
            return 1;
        }
    }
}

u8** Paramite::ResBlockForMotion_488130(s16 motion)
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
    if (motion < eParamiteMotions::M_Squawk_42_48D900)
    {
        return field_10_resources_array.ItemAt(15);
    }
    if (motion >= eParamiteMotions::M_Attack_43_48DB70)
    {
        return field_10_resources_array.ItemAt(motion >= 44 ? 0 : 9);
    }
    return field_10_resources_array.ItemAt(10);
}

void Paramite::VOn_TLV_Collision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_10C_health = FP_FromInteger(0);
                field_12C_brain_ret = 0;
                SetBrain(&Paramite::Brain_7_DeathDrop_484FF0);
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
                Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
            }
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, field_B8_xpos, field_BC_ypos, field_B8_xpos, field_BC_ypos);
    }
}

s16 Paramite::AnotherParamiteNear_4886E0()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eParamite_96 && pObj != this)
        {
            auto pOther = static_cast<Paramite*>(pObj);
            if (pOther->field_CC_sprite_scale == field_CC_sprite_scale && gMap.Is_Point_In_Current_Camera_4810D0(pOther->field_C2_lvl_number, pOther->field_C0_path_number, pOther->field_B8_xpos, pOther->field_BC_ypos, 0) && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) && IsNear_488B10(pOther))
            {
                if (pOther->BrainIs(&Paramite::Brain_0_Patrol_4835B0) || pOther->BrainIs(&Paramite::Brain_2_ChasingAbe_4859D0))
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

void Paramite::VRender(PrimHeader** ppOt)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Paramite::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_449CF0(field_110_id));
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

s16 Paramite::CanIAcceptAGameSpeakCommand_489160()
{
    if (sControlledCharacter_5C1B8C->Type() != AETypes::eParamite_96)
    {
        return 0;
    }

    const s32 distToPlayer = Math_Distance_496EB0(
        FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos),
        FP_GetExponent(sControlledCharacter_5C1B8C->field_BC_ypos),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos));

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find another paramite on the same layer/scale
        if (pObj != this && pObj != sControlledCharacter_5C1B8C && pObj->field_CC_sprite_scale == sControlledCharacter_5C1B8C->field_CC_sprite_scale && pObj->Type() == AETypes::eParamite_96)
        {
            auto pParamite = static_cast<Paramite*>(pObj);

            if (pParamite->field_178_flags.Get(Flags_178::eBit7_alerted))
            {
                return 0;
            }

            if (!pParamite->BrainIs(&Paramite::Brain_8_ControlledByGameSpeak_48DFC0) && gMap.Is_Point_In_Current_Camera_4810D0(pParamite->field_C2_lvl_number, pParamite->field_C0_path_number, pParamite->field_B8_xpos, pParamite->field_BC_ypos, 0))
            {
                if (sControlledCharacter_5C1B8C->VIsFacingMe(pParamite) && !sControlledCharacter_5C1B8C->VIsFacingMe(this))
                {
                    return 0;
                }

                if (distToPlayer > Math_Distance_496EB0(
                        FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos),
                        FP_GetExponent(sControlledCharacter_5C1B8C->field_BC_ypos),
                        FP_GetExponent(pParamite->field_B8_xpos),
                        FP_GetExponent(pParamite->field_BC_ypos)))
                {
                    if (sControlledCharacter_5C1B8C->VIsFacingMe(pParamite) == sControlledCharacter_5C1B8C->VIsFacingMe(this))
                    {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

s16 Paramite::HandleEnemyStopper_4893B0(s16 numGridBlocks)
{
    auto pEnemyStopper = static_cast<Path_EnemyStopper*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos + (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(numGridBlocks))),
        FP_GetExponent(field_BC_ypos),
        TlvTypes::EnemyStopper_47));

    // No stopper or its disabled
    if (!pEnemyStopper || !SwitchStates_Get(pEnemyStopper->field_12_switch_id))
    {
        return 0;
    }

    // We have a stopper that applies to any direction
    if (pEnemyStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Both_2)
    {
        return 1;
    }

    // Does the stopper direction match the animation direction?
    if ((pEnemyStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Left_0 && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) || (pEnemyStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Right_1 && !(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))))
    {
        return 1;
    }

    return 0;
}

PullRingRope* Paramite::FindPullRope_488F20()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::ePullRope_103)
        {
            auto pRope = static_cast<PullRingRope*>(pObj);

            if (pRope->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                PSX_RECT bRect = {};
                pRope->VGetBoundingRect(&bRect, 1);
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

s16 Paramite::NextPlayerInputMotion_4894C0()
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
                    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_449CF0(field_120_obj_id));
                    if (pTarget)
                    {
                        field_120_obj_id = -1;
                        if (VIsObjNearby(kGridSize * FP_FromDouble(0.5), pTarget))
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

s16 Paramite::FindTarget_488C30()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        BaseAliveGameObject* pBestTarget = nullptr;
        for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if ((pObj->Type() == AETypes::eFleech_50 || pObj->Type() == AETypes::eSlig_125 || pObj->Type() == AETypes::eSlurg_129 || pObj->Type() == AETypes::eMudokon_110) && pObj->field_10C_health > FP_FromInteger(0))
            {
                if (vOnSameYLevel_488A40(pObj))
                {
                    if (field_CC_sprite_scale == pObj->field_CC_sprite_scale)
                    {
                        if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(20), pObj->field_B8_xpos - field_B8_xpos))
                        {
                            if (FP_GetExponent(pObj->field_B8_xpos) / 375 == FP_GetExponent(field_B8_xpos) / 375 && FP_GetExponent(pObj->field_BC_ypos) / 260 == FP_GetExponent(field_BC_ypos) / 260)
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

    if (VOnSameYLevel(sActiveHero_5C1B68) && !sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible) && field_CC_sprite_scale == sActiveHero_5C1B68->field_CC_sprite_scale && !WallHit_408750((sActiveHero_5C1B68->field_CC_sprite_scale * FP_FromInteger(20)), sActiveHero_5C1B68->field_B8_xpos - field_B8_xpos))
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

s16 Paramite::ToNextMotion_4898A0()
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
        case eParamiteMotions::M_Squawk_42_48D900:
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
    field_13C_velx_offset = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = eParamiteMotions::M_Idle_0_489FB0;
    MapFollowMe(TRUE);
    field_154_input = 0;
}

void Paramite::ToKnockBack_489BB0()
{
    field_B8_xpos -= field_C4_velx;
    field_154_input = 0;

    MapFollowMe(TRUE);

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

    BaseGameObject* pPlatform = sObjectIds.Find_449CF0(field_110_id);
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
        else if (field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_32 || field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_36)
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
        field_13C_velx_offset = FP_FromDouble(0.3);
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

    VOnCollisionWith(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
}

void Paramite::HandleStopWalking_48A720()
{
    // Pressing opposite direction of movement or not pressing any direction
    if ((field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4)) || (field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0)) || !sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        field_106_current_motion = eParamiteMotions::M_WalkEnd_8_48A870;
    }
}

void Paramite::HandleInputRunning_48ADB0()
{
    if ((field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4)) || (field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0)) || !sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
    {
        field_106_current_motion = eParamiteMotions::M_RunEnd_10_48B000;
    }
    else if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
    {
        if (sInputKey_Hop_5550E0 & field_154_input)
        {
            ToHop_489C20();
        }
        else if ((sInputKey_ThrowItem_5550F4 | sInputKey_DoAction_5550E4) & field_154_input)
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

ALIVE_VAR(1, 0x5C92EC, s16, sParamiteDelayIdx_5C92EC, 0);

const s16 sParamiteDelayTable_55D7B8[4] = {0, 5, 10, 0};

s16 Paramite::StableDelay_48DF80()
{
    if (sParamiteDelayIdx_5C92EC >= 3u)
    {
        sParamiteDelayIdx_5C92EC = 0;
    }
    return sParamiteDelayTable_55D7B8[sParamiteDelayIdx_5C92EC++];
}

// TODO: repetition with `MainMenu.cpp`
const SfxDefinition paramite_stru_55D7C0[12] = {
    {0u, 5u, 60u, 90u, -520, -520},
    {0u, 5u, 61u, 75u, -520, -520},
    {0u, 5u, 62u, 50u, -520, -520},
    {0u, 5u, 64u, 40u, -520, -520},
    {0u, 5u, 65u, 40u, -520, -520},
    {0u, 5u, 66u, 30u, -520, -520},
    {0u, 5u, 66u, 50u, 0, 0},
    {0u, 5u, 67u, 40u, -520, -520},
    {0u, 5u, 68u, 30u, -520, -520},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};

void Paramite::Sound_48F600(ParamiteSpeak soundId, s16 pitch_min)
{
    const CameraPos direction = gMap.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos);

    s16 volRight = 0;
    if (soundId == ParamiteSpeak::Howdy_5)
    {
        volRight = paramite_stru_55D7C0[5].field_3_default_volume;
        if (sControlledCharacter_5C1B8C == this)
        {
            volRight *= 3;
        }
    }
    else if (soundId == ParamiteSpeak::AllYa_9)
    {
        volRight = sSeqData_558D50.mSeqs[20].field_9_volume;
    }
    else
    {
        volRight = paramite_stru_55D7C0[static_cast<s32>(soundId)].field_3_default_volume;
    }

    if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        volRight = (2 * volRight) / 3;
    }

    PSX_RECT pRect = {};
    gMap.Get_Camera_World_Rect_481410(direction, &pRect);

    s16 volLeft = 0;
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
            volLeft = volRight;
            break;

        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            const s16 v12 = FP_GetExponent(FP_FromRaw(paramite_stru_55D7C0[static_cast<s32>(soundId)].field_3_default_volume) / FP_FromInteger(3));
            volLeft = v12;
            volRight = v12;
            break;
        }

        case CameraPos::eCamLeft_3:
        {
            const FP v9 = (FP_FromInteger(pRect.w) - field_B8_xpos) / FP_FromInteger(368);
            volLeft = volRight - FP_GetExponent((v9 * FP_FromInteger((volRight - 1) * (volRight / 3))));
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
        SFX_SfxDefinition_Play_4CA700(&paramite_stru_55D7C0[static_cast<s32>(soundId)], volLeft, volRight, pitch_min, pitch_min);
    }
    else
    {
        SFX_SfxDefinition_Play_4CA700(&paramite_stru_55D7C0[static_cast<s32>(soundId)], volLeft, volRight, -520, -520);
    }
}

void Paramite::UpdateSlurgWatchPoints_4890D0()
{
    const s8 count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
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
    if (field_160_last_event_index == pEventSystem_5BC11C->field_28_last_event_index)
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
        field_160_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
        ret = pEventSystem_5BC11C->field_20_last_event;
    }

    if (gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 1))
    {
        return ret;
    }

    return GameSpeakEvents::eNone_m1;
}
