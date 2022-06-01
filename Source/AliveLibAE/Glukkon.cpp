#include "stdafx.h"
#include "Glukkon.hpp"
#include "Function.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Shadow.hpp"
#include "MusicController.hpp"
#include "SwitchStates.hpp"
#include "Spark.hpp"
#include "Slig.hpp" //used for renderWithGlowingEyes() only - TODO refactor
#include "ParticleBurst.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "ObjectIds.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "Slurg.hpp"
#include "SlamDoor.hpp"
#include "DeathFadeOut.hpp"
#include "ScreenManager.hpp"
#include "Movie.hpp"
#include "DDCheat.hpp"
#include "PsxDisplay.hpp"
#include "Sound/Sound.hpp"
#include "MainMenu.hpp"
#include "GameSpeak.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "Bullet.hpp"
#include "Sound/Midi.hpp"
#include "Sys_common.hpp"
#include "Grid.hpp"

#define MAKE_STRINGS(VAR) #VAR,
const char_type* const sGlukkonMotionNames[25] = {
    GLUKKON_MOTIONS_ENUM(MAKE_STRINGS)};

const TGlukkonMotionFn sGlukkon_motion_table_5544C0[25] = {
    &Glukkon::M_Idle_0_442D10,
    &Glukkon::M_Walk_1_442D30,
    &Glukkon::M_Turn_2_442F10,
    &Glukkon::M_KnockBack_3_442F40,
    &Glukkon::M_Jump_4_443030,
    &Glukkon::M_JumpToFall_5_4434C0,
    &Glukkon::M_WalkToFall_6_4434E0,
    &Glukkon::M_Fall_7_443510,
    &Glukkon::M_DeathFall_8_443760,
    &Glukkon::M_Land_9_443790,
    &Glukkon::M_ChantShake_10_443B50,
    &Glukkon::M_Speak1_11_4437D0,
    &Glukkon::M_Speak2_12_4438F0,
    &Glukkon::M_LongLaugh_13_443930,
    &Glukkon::M_BeginWalk_14_443950,
    &Glukkon::M_EndWalk_15_443970,
    &Glukkon::M_StandToJump_16_4439B0,
    &Glukkon::M_JumpToStand_17_4439D0,
    &Glukkon::M_WalkToJump_18_443A00,
    &Glukkon::M_JumpToWalk_19_443A30,
    &Glukkon::M_KnockBackStandBegin_20_442FC0,
    &Glukkon::M_GetShot_21_443A60,
    &Glukkon::M_KnockBackStandEnd_22_443010,
    &Glukkon::M_Speak3_23_443910,
    &Glukkon::M_EndSingleStep_24_443990};

const TGlukkonBrainFn sGlukkon_brain_table_5544A0[6] = {
    &Glukkon::Brain_0_Calm_WalkAround_440B40,
    &Glukkon::Brain_1_Panic_4412F0,
    &Glukkon::Brain_2_Slapped_441720,
    &Glukkon::Brain_3_PlayerControlled_441A30,
    &Glukkon::Brain_4_Death_442010,
    &Glukkon::Brain_5_WaitToSpawn_442490};

const AnimId sGlukkonsFrameTableOffsetTable_554524[4][25] = {
    {AnimId::Glukkon_Normal_Idle, AnimId::Glukkon_Normal_Walk, AnimId::Glukkon_Normal_Turn, AnimId::Glukkon_Normal_KnockBack, AnimId::Glukkon_Normal_Jump,
     AnimId::Glukkon_Normal_JumpToFall, AnimId::Glukkon_Normal_WalkToFall, AnimId::Glukkon_Normal_Fall, AnimId::Glukkon_Normal_DeathFall, AnimId::Glukkon_Normal_Land,
     AnimId::Glukkon_Normal_ChantShake, AnimId::Glukkon_Normal_Speak1, AnimId::Glukkon_Normal_Speak2, AnimId::Glukkon_Normal_LongLaugh, AnimId::Glukkon_Normal_BeginWalk,
     AnimId::Glukkon_Normal_EndWalk, AnimId::Glukkon_Normal_StandToJump, AnimId::Glukkon_Normal_JumpToStand, AnimId::Glukkon_Normal_WalkToJump, AnimId::Glukkon_Normal_JumpToWalk,
     AnimId::Glukkon_Normal_KnockBackStandBegin, AnimId::Glukkon_Normal_GetShot, AnimId::Glukkon_Normal_KnockBackStandEnd, AnimId::Glukkon_Normal_Speak3, AnimId::Glukkon_Normal_EndSingleStep},

    {AnimId::Glukkon_Aslik_Idle, AnimId::Glukkon_Aslik_Walk, AnimId::Glukkon_Aslik_Turn, AnimId::Glukkon_Aslik_KnockBack, AnimId::Glukkon_Aslik_Jump,
     AnimId::Glukkon_Aslik_JumpToFall, AnimId::Glukkon_Aslik_WalkToFall, AnimId::Glukkon_Aslik_Fall, AnimId::Glukkon_Aslik_DeathFall, AnimId::Glukkon_Aslik_Land,
     AnimId::Glukkon_Aslik_ChantShake, AnimId::Glukkon_Aslik_Speak1, AnimId::Glukkon_Aslik_Speak2, AnimId::Glukkon_Aslik_LongLaugh, AnimId::Glukkon_Aslik_BeginWalk,
     AnimId::Glukkon_Aslik_EndWalk, AnimId::Glukkon_Aslik_StandToJump, AnimId::Glukkon_Aslik_JumpToStand, AnimId::Glukkon_Aslik_WalkToJump, AnimId::Glukkon_Aslik_JumpToWalk,
     AnimId::Glukkon_Aslik_KnockBackStandBegin, AnimId::Glukkon_Aslik_GetShot, AnimId::Glukkon_Aslik_KnockBackStandEnd, AnimId::Glukkon_Aslik_Speak3, AnimId::Glukkon_Aslik_EndSingleStep},

    {AnimId::Glukkon_Dripik_Idle, AnimId::Glukkon_Dripik_Walk, AnimId::Glukkon_Dripik_Turn, AnimId::Glukkon_Dripik_KnockBack, AnimId::Glukkon_Dripik_Jump,
     AnimId::Glukkon_Dripik_JumpToFall, AnimId::Glukkon_Dripik_WalkToFall, AnimId::Glukkon_Dripik_Fall, AnimId::Glukkon_Dripik_DeathFall, AnimId::Glukkon_Dripik_Land,
     AnimId::Glukkon_Dripik_ChantShake, AnimId::Glukkon_Dripik_Speak1, AnimId::Glukkon_Dripik_Speak2, AnimId::Glukkon_Dripik_LongLaugh, AnimId::Glukkon_Dripik_BeginWalk,
     AnimId::Glukkon_Dripik_EndWalk, AnimId::Glukkon_Dripik_StandToJump, AnimId::Glukkon_Dripik_JumpToStand, AnimId::Glukkon_Dripik_WalkToJump, AnimId::Glukkon_Dripik_JumpToWalk,
     AnimId::Glukkon_Dripik_KnockBackStandBegin, AnimId::Glukkon_Dripik_GetShot, AnimId::Glukkon_Dripik_KnockBackStandEnd, AnimId::Glukkon_Dripik_Speak3, AnimId::Glukkon_Dripik_EndSingleStep},

    {AnimId::Glukkon_Phleg_Idle, AnimId::Glukkon_Phleg_Walk, AnimId::Glukkon_Phleg_Turn, AnimId::Glukkon_Phleg_KnockBack, AnimId::Glukkon_Phleg_Jump,
     AnimId::Glukkon_Phleg_JumpToFall, AnimId::Glukkon_Phleg_WalkToFall, AnimId::Glukkon_Phleg_Fall, AnimId::Glukkon_Phleg_DeathFall, AnimId::Glukkon_Phleg_Land,
     AnimId::Glukkon_Phleg_ChantShake, AnimId::Glukkon_Phleg_Speak1, AnimId::Glukkon_Phleg_Speak2, AnimId::Glukkon_Phleg_LongLaugh, AnimId::Glukkon_Phleg_BeginWalk,
     AnimId::Glukkon_Phleg_EndWalk, AnimId::Glukkon_Phleg_StandToJump, AnimId::Glukkon_Phleg_JumpToStand, AnimId::Glukkon_Phleg_WalkToJump, AnimId::Glukkon_Phleg_JumpToWalk,
     AnimId::Glukkon_Phleg_KnockBackStandBegin, AnimId::Glukkon_Phleg_GetShot, AnimId::Glukkon_Phleg_KnockBackStandEnd, AnimId::Glukkon_Phleg_Speak3, AnimId::Glukkon_Phleg_EndSingleStep}};

const TintEntry kGlukkonTints_5546B4[18] = {
    {LevelIds_s8::eMines_1, 137u, 137u, 137u},
    {LevelIds_s8::eNecrum_2, 137u, 137u, 137u},
    {LevelIds_s8::eMudomoVault_3, 137u, 137u, 137u},
    {LevelIds_s8::eMudancheeVault_4, 137u, 137u, 137u},
    {LevelIds_s8::eFeeCoDepot_5, 137u, 137u, 137u},
    {LevelIds_s8::eBarracks_6, 137u, 137u, 137u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 137u, 137u, 137u},
    {LevelIds_s8::eBonewerkz_8, 137u, 137u, 137u},
    {LevelIds_s8::eBrewery_9, 137u, 137u, 137u},
    {LevelIds_s8::eBrewery_Ender_10, 137u, 137u, 137u},
    {LevelIds_s8::eMudomoVault_Ender_11, 137u, 137u, 137u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 137u, 137u, 137u},
    {LevelIds_s8::eBarracks_Ender_13, 137u, 137u, 137u},
    {LevelIds_s8::eBonewerkz_Ender_14, 137u, 137u, 137u},
    {LevelIds_s8::eNone, 137u, 137u, 137u}};

s32 Glukkon::CreateFromSaveState(const u8* pData)
{
    const Glukkon_SaveState* pSaveState = reinterpret_cast<const Glukkon_SaveState*>(pData);
    auto pTlv = static_cast<Path_Glukkon*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pSaveState->field_44_tlvInfo));

    switch (gMap.mCurrentLevel)
    {
        case LevelIds::eFeeCoDepot_5:
        case LevelIds::eFeeCoDepot_Ender_12:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kGlukAslikResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("ASLIK.BND", 0);
            }
            break;
        case LevelIds::eBarracks_6:
        case LevelIds::eBarracks_Ender_13:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kGlukDripikResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("DRIPIK.BND", 0);
            }
            break;
        case LevelIds::eBonewerkz_8:
        case LevelIds::eBonewerkz_Ender_14:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kGlukPhlegResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("PHLEG.BND", 0);
            }
            break;
        default:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kGlkbasicResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("GLUKKON.BND", 0);
            }
            break;
    }

    auto pGlukkon = ae_new<Glukkon>(pTlv, pSaveState->field_44_tlvInfo);
    if (pGlukkon)
    {
        pGlukkon->SetType(pSaveState->field_8E_type_id);
        pGlukkon->field_C_objectId = pSaveState->field_4_object_id;
        if (pSaveState->field_40_bIsActiveChar)
        {
            sControlledCharacter_5C1B8C = pGlukkon;
        }

        pGlukkon->field_FC_pPathTLV = nullptr;
        pGlukkon->field_100_pCollisionLine = nullptr;

        pGlukkon->field_B8_xpos = pSaveState->field_8_xpos;
        pGlukkon->field_BC_ypos = pSaveState->field_C_ypos;

        pGlukkon->field_C4_velx = pSaveState->field_10_xvel;
        pGlukkon->field_C8_vely = pSaveState->field_14_yvel;

        pGlukkon->field_1D8_falling_velx_scale_factor = pSaveState->field_58_falling_velx_scale_factor;
        pGlukkon->field_C0_path_number = pSaveState->field_18_path;
        pGlukkon->field_C2_lvl_number = pSaveState->field_1A_level;
        pGlukkon->field_CC_sprite_scale = pSaveState->field_1C_sprite_scale;

        pGlukkon->field_D0_r = pSaveState->field_20_r;
        pGlukkon->field_D2_g = pSaveState->field_22_g;
        pGlukkon->field_D4_b = pSaveState->field_24_b;

        pGlukkon->field_1A0_red = pSaveState->field_20_r;
        pGlukkon->field_1A2_green = pSaveState->field_22_g;
        pGlukkon->field_1A4_blue = pSaveState->field_24_b;

        pGlukkon->field_106_current_motion = pSaveState->field_28_current_motion;

        GlukkonTypes glukType = pGlukkon->field_1A8_tlvData.field_22_glukkon_type;
        if (glukType > GlukkonTypes::StoryPhleg_3)
        {
            glukType = GlukkonTypes::Normal_0;
        }

        const AnimRecord& animRec = AnimRec(sGlukkonsFrameTableOffsetTable_554524[static_cast<s32>(glukType)][pSaveState->field_28_current_motion]);
        pGlukkon->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);

        pGlukkon->field_20_animation.field_92_current_frame = pSaveState->field_2A_current_frame;
        pGlukkon->field_20_animation.field_E_frame_change_counter = pSaveState->field_2C_frame_change_counter;
        pGlukkon->mFlags.Set(BaseGameObject::Options::eDrawable_Bit4, pSaveState->field_2F_drawable & 1);
        pGlukkon->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pSaveState->field_26_flipX & 1);
        pGlukkon->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pSaveState->field_2E_render & 1);

        if (IsLastFrame(&pGlukkon->field_20_animation))
        {
            pGlukkon->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
        }

        pGlukkon->field_10C_health = pSaveState->field_30_health;
        pGlukkon->field_106_current_motion = pSaveState->field_34_current_motion;
        pGlukkon->field_108_next_motion = pSaveState->field_36_next_motion;
        pGlukkon->field_F8_LastLineYPos = FP_FromInteger(pSaveState->field_38_last_line_ypos);
        pGlukkon->field_114_flags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
        pGlukkon->field_1D4_timer = pSaveState->field_54_timer;
        pGlukkon->field_104_collision_line_type = pSaveState->field_3A_line_type;
        pGlukkon->field_214_tlv_info = pSaveState->field_44_tlvInfo;
        pGlukkon->SetBrain(sGlukkon_brain_table_5544A0[pSaveState->field_48_brain_state_idx]);
        pGlukkon->field_210_brain_sub_state = pSaveState->field_50_brain_sub_state;
        pGlukkon->field_1E2_prevent_depossession = pSaveState->field_5E_prevent_depossession;
        pGlukkon->field_1E4_level = pSaveState->field_60_level;
        pGlukkon->field_1E6_path = pSaveState->field_62_path;
        pGlukkon->field_1E8_camera = pSaveState->field_64_camera;
        pGlukkon->field_1EA_speak = pSaveState->field_66_speak;
        pGlukkon->field_1E0_gamespeak_pitch = pSaveState->field_68_gamespeak_pitch;
        pGlukkon->field_1DC_previous_ypos = pSaveState->field_6C_previous_ypos;
        pGlukkon->field_1F0_randomish_speak_timer = pSaveState->field_70_randomish_speak_timer;
        pGlukkon->field_1F4_turn_or_help_timer = pSaveState->field_74_turn_or_help_timer;
        pGlukkon->field_1F8_panic_timer = pSaveState->field_78_panic_timer;
        pGlukkon->field_1FC = pSaveState->field_7C;
        pGlukkon->field_200_knockback_delay_after_getting_shot_timer = pSaveState->field_80_knockback_delay_after_getting_shot_timer;
        pGlukkon->field_204_getting_shot_timer = pSaveState->field_84_getting_shot_timer;
        pGlukkon->field_208_obj_id = pSaveState->field_88_obj_id;
        pGlukkon->field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed, pSaveState->field_8C_can_be_possessed);
    }

    return sizeof(Glukkon_SaveState);
}

const static BrainFunctionData<TGlukkonBrainFn> sGlukkonBrainTable[6] = {
    {&Glukkon::Brain_0_Calm_WalkAround_440B40, 0x402D60, "Brain_0_Calm_WalkAround"},
    {&Glukkon::Brain_1_Panic_4412F0, 0x403049, "Brain_1_Panic"},
    {&Glukkon::Brain_2_Slapped_441720, 0x403864, "Brain_2_Slapped"},
    {&Glukkon::Brain_3_PlayerControlled_441A30, 0x401BF4, "Brain_3_PlayerControlled"},
    {&Glukkon::Brain_4_Death_442010, 0x401CE9, "Brain_4_Death"},
    {&Glukkon::Brain_5_WaitToSpawn_442490, 0x40357B, "Brain_5_WaitToSpawn"}};

void Glukkon::SetBrain(TGlukkonBrainFn fn)
{
    ::SetBrain(fn, field_20C_brain_state_fn, sGlukkonBrainTable);
}

bool Glukkon::BrainIs(TGlukkonBrainFn fn)
{
    return ::BrainIs(fn, field_20C_brain_state_fn, sGlukkonBrainTable);
}

Glukkon::Glukkon(Path_Glukkon* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    ;
    field_1EC_unused = -1;

    field_1A8_tlvData = *pTlv;

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_214_tlv_info = tlvInfo;

    switch (field_1A8_tlvData.field_22_glukkon_type)
    {
        case GlukkonTypes::Normal_0:
        case GlukkonTypes::Normal_4:
        case GlukkonTypes::Normal_5:
        {
            Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kGlukkonResID_801);
            const AnimRecord& rec = AnimRec(AnimId::Glukkon_Normal_Idle);
            u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
            Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
            break;
        }

        case GlukkonTypes::StoryAslik_1:
        {
            Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAslikResID_803);
            const AnimRecord& rec = AnimRec(AnimId::Glukkon_Aslik_Idle);
            u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
            Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
            break;
        }

        case GlukkonTypes::StoryDripik_2:
        {
            Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kDripikResID_805);
            const AnimRecord& rec = AnimRec(AnimId::Glukkon_Dripik_Idle);
            u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
            Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
            break;
        }

        case GlukkonTypes::StoryPhleg_3:
        {
            Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kPhlegResID_807);
            const AnimRecord& rec = AnimRec(AnimId::Glukkon_Phleg_Idle);
            u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
            Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
            break;
        }

        default:
            break;
    }

    Init_43F260();
}

void Glukkon::VRender(PrimHeader** ot)
{
    s16 eyeIndices[1];
    if (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryAslik_1 || field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryPhleg_3)
    {
        eyeIndices[0] = 63;
    }
    else
    {
        eyeIndices[0] = 62;
    }
    renderWithGlowingEyes(ot, this, &field_118_pPalAlloc[0], ALIVE_COUNTOF(field_118_pPalAlloc),
                          &field_198_pal_rect, field_1A0_red, field_1A2_green, field_1A4_blue, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

s32 Glukkon::VGetSaveState(u8* pSaveBuffer)
{
    Glukkon_SaveState* pSaveState = reinterpret_cast<Glukkon_SaveState*>(pSaveBuffer);

    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }
    pSaveState->field_0_id = AETypes::eGlukkon_67;
    pSaveState->field_4_object_id = field_C_objectId;
    pSaveState->field_8_xpos = field_B8_xpos;
    pSaveState->field_C_ypos = field_BC_ypos;
    pSaveState->field_10_xvel = field_C4_velx;
    pSaveState->field_14_yvel = field_C8_vely;
    pSaveState->field_18_path = field_C0_path_number;
    pSaveState->field_1A_level = field_C2_lvl_number;
    pSaveState->field_1C_sprite_scale = field_CC_sprite_scale;
    pSaveState->field_20_r = field_D0_r;
    pSaveState->field_22_g = field_D2_g;
    pSaveState->field_24_b = field_D4_b;
    pSaveState->field_28_current_motion = field_106_current_motion;
    pSaveState->field_2A_current_frame = field_20_animation.field_92_current_frame;
    pSaveState->field_2C_frame_change_counter = field_20_animation.field_E_frame_change_counter;
    pSaveState->field_26_flipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pSaveState->field_2E_render = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pSaveState->field_2F_drawable = mFlags.Get(BaseGameObject::Options::eDrawable_Bit4);
    pSaveState->field_30_health = field_10C_health;
    pSaveState->field_34_current_motion = field_106_current_motion;
    pSaveState->field_36_next_motion = field_108_next_motion;
    pSaveState->field_38_last_line_ypos = FP_GetExponent(field_F8_LastLineYPos);
    if (field_100_pCollisionLine)
    {
        pSaveState->field_3A_line_type = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pSaveState->field_3A_line_type = -1;
    }
    pSaveState->field_40_bIsActiveChar = this == static_cast<Glukkon*>(sControlledCharacter_5C1B8C);
    pSaveState->field_44_tlvInfo = field_214_tlv_info;

    pSaveState->field_48_brain_state_idx = 0;

    s16 idx = 0;
    for (auto& fn : sGlukkon_brain_table_5544A0)
    {
        if (BrainIs(fn))
        {
            pSaveState->field_48_brain_state_idx = idx;
            break;
        }
        idx++;
    }

    pSaveState->field_50_brain_sub_state = field_210_brain_sub_state;
    pSaveState->field_54_timer = field_1D4_timer;
    pSaveState->field_58_falling_velx_scale_factor = field_1D8_falling_velx_scale_factor;
    pSaveState->field_5E_prevent_depossession = field_1E2_prevent_depossession;
    pSaveState->field_60_level = field_1E4_level;
    pSaveState->field_62_path = field_1E6_path;
    pSaveState->field_64_camera = field_1E8_camera;
    pSaveState->field_66_speak = field_1EA_speak;
    pSaveState->field_68_gamespeak_pitch = field_1E0_gamespeak_pitch;
    pSaveState->field_6C_previous_ypos = field_1DC_previous_ypos;
    pSaveState->field_70_randomish_speak_timer = field_1F0_randomish_speak_timer;
    pSaveState->field_74_turn_or_help_timer = field_1F4_turn_or_help_timer;
    pSaveState->field_78_panic_timer = field_1F8_panic_timer;
    pSaveState->field_7C = field_1FC;
    pSaveState->field_80_knockback_delay_after_getting_shot_timer = field_200_knockback_delay_after_getting_shot_timer;
    pSaveState->field_84_getting_shot_timer = field_204_getting_shot_timer;
    pSaveState->field_88_obj_id = field_208_obj_id;
    pSaveState->field_8C_can_be_possessed = field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed);
    pSaveState->field_8E_type_id = Type();

    return sizeof(Glukkon_SaveState);
}

void Glukkon::M_Idle_0_442D10()
{
    HandleInput_443BB0();
}

void Glukkon::M_Walk_1_442D30()
{
    if (DoMovement_444190())
    {
        switch (field_20_animation.field_92_current_frame)
        {
            case 0:
            case 9:
                if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
                {
                    if (field_108_next_motion == eGlukkonMotions::M_Jump_4_443030)
                    {
                        SetAnim_43F9C0(eGlukkonMotions::M_WalkToJump_18_443A00);
                    }
                }
                else if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & InputCommands::Enum::eRun)
                {
                    SetAnim_43F9C0(eGlukkonMotions::M_WalkToJump_18_443A00, TRUE);
                }

                MapFollowMe(TRUE);
                return;

            case 2:
            case 6:
            case 10:
            case 14:
                PlaySound_4447D0(0, this);
                field_212_currentWalkPitch++;
                return;

            case 8:
            case 17:
                if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
                {
                    if (field_108_next_motion == eGlukkonMotions::M_Idle_0_442D10 || field_108_next_motion == eGlukkonMotions::M_Turn_2_442F10 || field_108_next_motion == eGlukkonMotions::M_Speak1_11_4437D0 || field_108_next_motion == eGlukkonMotions::M_Speak2_12_4438F0 || field_108_next_motion == eGlukkonMotions::M_Speak3_23_443910 || field_108_next_motion == eGlukkonMotions::M_LongLaugh_13_443930)
                    {
                        if (field_20_animation.field_92_current_frame != 8)
                        {
                            SetAnim_43F9C0(eGlukkonMotions::M_EndWalk_15_443970);
                        }
                        else
                        {
                            SetAnim_43F9C0(eGlukkonMotions::M_EndSingleStep_24_443990);
                        }
                    }
                }
                else
                {
                    if ((field_C4_velx > FP_FromInteger(0) && (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & InputCommands::Enum::eLeft)) || (field_C4_velx < FP_FromInteger(0) && (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & InputCommands::Enum::eRight)) || !(sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & (InputCommands::Enum::eLeft | InputCommands::Enum::eRight)))
                    {
                        if (field_20_animation.field_92_current_frame == 8)
                        {
                            SetAnim_43F9C0(eGlukkonMotions::M_EndSingleStep_24_443990, TRUE);
                        }
                        else
                        {
                            SetAnim_43F9C0(eGlukkonMotions::M_EndWalk_15_443970, TRUE);
                        }
                    }
                }
                break;

            default:
                return;
        }
    }
}

void Glukkon::M_Turn_2_442F10()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToStand_443B70();
    }
}

void Glukkon::M_KnockBack_3_442F40()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        PlaySound_4447D0(2, this);
    }

    if (field_100_pCollisionLine)
    {
        SlowDown_444700(FP_FromDouble(0.35));
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                SetAnim_43F9C0(eGlukkonMotions::M_KnockBackStandBegin_20_442FC0, TRUE);
            }
        }
    }
    else
    {
        M_Fall_7_443510();
    }
}

const FP sGlukkonVelY_5453DC[10] = {
    FP_FromDouble(-10.011077880859375),
    FP_FromDouble(-10.583099365234375),
    FP_FromDouble(-7.2048492431640625),
    FP_FromDouble(-4.6619873046875),
    FP_FromInteger(-3),
    FP_FromInteger(3),
    FP_FromDouble(4.4048004150390625),
    FP_FromDouble(7.0737457275390625),
    FP_FromDouble(10.93603515625),
    FP_FromDouble(15.046417236328125)};

const FP sGlukkonJumpVelX_54539C[10] = {
    FP_FromDouble(10.335205078125),
    FP_FromDouble(21.649551391601562),
    FP_FromDouble(18.9296875),
    FP_FromDouble(12.507583618164062),
    FP_FromDouble(10.808151245117188),
    FP_FromDouble(7.7117919921875),
    FP_FromDouble(5.19439697265625),
    FP_FromDouble(4.020263671875),
    FP_FromDouble(2.5957489013671875),
    FP_FromDouble(0.920867919921875)};

void Glukkon::M_Jump_4_443030()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_449CF0(field_110_id));

    if (field_20_animation.field_92_current_frame >= 10)
    {
        JumpHelper();
        return;
    }

    if (field_20_animation.field_92_current_frame == 0)
    {
        SFX_Play(SoundEffect::PickupItem_28, 50, -900);
        field_F8_LastLineYPos = field_BC_ypos;
        if (pPlatform)
        {
            pPlatform->VRemove(this);
            field_110_id = -1;
        }
        field_100_pCollisionLine = nullptr;
    }

    field_C8_vely = (field_CC_sprite_scale * sGlukkonVelY_5453DC[field_20_animation.field_92_current_frame]);

    FP velXTableValue = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velXTableValue = -sGlukkonJumpVelX_54539C[field_20_animation.field_92_current_frame];
    }
    else
    {
        velXTableValue = sGlukkonJumpVelX_54539C[field_20_animation.field_92_current_frame];
    }

    field_C4_velx = (field_CC_sprite_scale * velXTableValue);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) || WallHit_408750(field_CC_sprite_scale * FP_FromInteger(2), field_C4_velx))
    {
        field_C8_vely = FP_FromInteger(0);
        field_1D8_falling_velx_scale_factor = FP_FromDouble(0.35);
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
        }
        else
        {
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
        }
        SetAnim_43F9C0(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
        MapFollowMe(TRUE);
        return;
    }

    if (sControlledCharacter_5C1B8C == this)
    {
        SetActiveCameraDelayedFromDir();
    }

    Bool32 bCollision = FALSE;
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (field_100_pCollisionLine)
    {
        bCollision = FALSE;
    }
    else
    {
        bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromInteger(0));
    }

    if (field_20_animation.field_92_current_frame == 0 || !bCollision)
    {
        if (field_20_animation.field_92_current_frame == 9 && !field_100_pCollisionLine)
        {
            SetAnim_43F9C0(eGlukkonMotions::M_Fall_7_443510, TRUE);
        }
        JumpHelper();
        return;
    }

    switch (pLine->field_8_type)
    {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            PlaySound_4447D0(1, this);
            field_B8_xpos = hitX;
            field_100_pCollisionLine = pLine;
            field_BC_ypos = hitY;
            MapFollowMe(TRUE);
            GetOnPlatforms_444060();
            break;

        case 1u:
        case 2u:
            field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
            break;

        default:
            if (field_20_animation.field_92_current_frame == 9 && !field_100_pCollisionLine)
            {
                SetAnim_43F9C0(eGlukkonMotions::M_Fall_7_443510, TRUE);
            }
            JumpHelper();
            return;
    }
}

void Glukkon::JumpHelper()
{
    if (field_20_animation.field_92_current_frame != 15)
    {
        return;
    }

    if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
    {
        const auto input_pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

        if ((field_C4_velx > FP_FromInteger(0) && (input_pressed & InputCommands::Enum::eLeft)) || (field_C4_velx < FP_FromInteger(0) && (input_pressed & InputCommands::Enum::eRight)))
        {
            // Direction changed
            SetAnim_43F9C0(eGlukkonMotions::M_JumpToStand_17_4439D0, TRUE);
        }
        else if (input_pressed & (InputCommands::Enum::eLeft | InputCommands::Enum::eRight))
        {
            if (!(input_pressed & InputCommands::Enum::eRun))
            {
                // Still moving but not running, so start walking
                SetAnim_43F9C0(eGlukkonMotions::M_JumpToWalk_19_443A30, TRUE);
            }
        }
        else if (!(input_pressed & InputCommands::Enum::eHop))
        {
            // Not changing direction or trying to move at all, back to standing
            SetAnim_43F9C0(eGlukkonMotions::M_JumpToStand_17_4439D0, TRUE);
        }
    }
    else
    {
        if (field_108_next_motion != 1)
        {
            if (field_108_next_motion == eGlukkonMotions::M_Idle_0_442D10 || field_108_next_motion == eGlukkonMotions::M_Turn_2_442F10 || field_108_next_motion == eGlukkonMotions::M_Speak1_11_4437D0 || field_108_next_motion == eGlukkonMotions::M_Speak2_12_4438F0 || field_108_next_motion == eGlukkonMotions::M_Speak3_23_443910 || field_108_next_motion == eGlukkonMotions::M_LongLaugh_13_443930)
            {
                // Leaving jump to a motion that requires standing
                SetAnim_43F9C0(eGlukkonMotions::M_JumpToStand_17_4439D0);
            }
        }
        else
        {
            // No next motion so just stand
            SetAnim_43F9C0(eGlukkonMotions::M_JumpToWalk_19_443A30, TRUE);
        }
    }

    MapFollowMe(TRUE);
    return;
}

void Glukkon::M_JumpToFall_5_4434C0()
{
    M_WalkToFall_6_4434E0();
}

void Glukkon::M_WalkToFall_6_4434E0()
{
    M_Fall_7_443510();

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim_43F9C0(eGlukkonMotions::M_Fall_7_443510, TRUE);
    }
}

void Glukkon::M_Fall_7_443510()
{
    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx = field_C4_velx - (field_CC_sprite_scale * field_1D8_falling_velx_scale_factor);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx = (field_CC_sprite_scale * field_1D8_falling_velx_scale_factor) + field_C4_velx;
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    const auto bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (BrainIs(&Glukkon::Brain_3_PlayerControlled_441A30))
    {
        SetActiveCameraDelayedFromDir();
    }

    if (bCollision)
    {
        switch (pLine->field_8_type)
        {
            case 0:
            case 4:
            case 32:
            case 36:
                field_100_pCollisionLine = pLine;
                field_BC_ypos = hitY;
                field_B8_xpos = hitX;
                field_C8_vely = FP_FromInteger(0);

                GetOnPlatforms_444060();

                if (hitY - field_F8_LastLineYPos > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(7)))
                {
                    SetAnim_43F9C0(eGlukkonMotions::M_DeathFall_8_443760, TRUE);
                    SetBrain(&Glukkon::Brain_4_Death_442010);
                    field_210_brain_sub_state = 0;
                }
                else if (field_106_current_motion != eGlukkonMotions::M_KnockBack_3_442F40)
                {
                    SetAnim_43F9C0(eGlukkonMotions::M_Land_9_443790, TRUE);
                }
                break;

            case 1u:
            case 2u:
                field_B8_xpos = hitX;
                field_C4_velx = -field_C4_velx / FP_FromInteger(2);
                break;

            default:
                return;
        }
    }
}

void Glukkon::M_DeathFall_8_443760()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        SFX_Play_46FA90(SoundEffect::KillEffect_64, 85);
    }
}

void Glukkon::M_Land_9_443790()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Glukkon::PlaySound_4447D0(1, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleInput_443BB0();
    }
}

void Glukkon::M_ChantShake_10_443B50()
{
    if (!field_100_pCollisionLine)
    {
        M_Fall_7_443510();
    }
}

void Glukkon::M_Speak1_11_4437D0()
{
    if (field_20_animation.field_92_current_frame == 2 && field_1EA_speak != GlukkonSpeak::None)
    {
        if (gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                field_BC_ypos,
                0))
        {
            if (field_1FC)
            {
                Event_Broadcast_422BC0(kEventUnknown17, this);
                field_1FC = 0;
            }

            if (BrainIs(&Glukkon::Brain_3_PlayerControlled_441A30))
            {
                GameSpeakEvents evToBePushed;
                switch (field_1EA_speak)
                {
                    case GlukkonSpeak::Hey_0:
                        evToBePushed = GameSpeakEvents::Glukkon_Hey_36;
                        break;
                    case GlukkonSpeak::DoIt_1:
                        evToBePushed = GameSpeakEvents::Glukkon_DoIt_37;
                        break;
                    case GlukkonSpeak::StayHere_2:
                        evToBePushed = GameSpeakEvents::Glukkon_StayHere_38;
                        break;
                    case GlukkonSpeak::Commere_3:
                        evToBePushed = GameSpeakEvents::Glukkon_Commere_39;
                        break;
                    case GlukkonSpeak::AllOYa_4:
                        evToBePushed = GameSpeakEvents::Glukkon_AllOYa_40;
                        break;
                    case GlukkonSpeak::Heh_5:
                        evToBePushed = GameSpeakEvents::Glukkon_Heh_41;
                        break;
                    case GlukkonSpeak::Help_6:
                        evToBePushed = GameSpeakEvents::Glukkon_Help_42;
                        break;
                    case GlukkonSpeak::Laugh_7:
                        evToBePushed = GameSpeakEvents::Glukkon_Laugh_43;
                        break;
                    case GlukkonSpeak::KillEm_8:
                        evToBePushed = GameSpeakEvents::Glukkon_KillEm_44;
                        break;
                    case GlukkonSpeak::Unused_9:
                        evToBePushed = GameSpeakEvents::Glukkon_Unknown_45;
                        break;
                    case GlukkonSpeak::Unused_10:
                        evToBePushed = GameSpeakEvents::Glukkon_Unknown_46;
                        break;
                    case GlukkonSpeak::What_11:
                        evToBePushed = GameSpeakEvents::Glukkon_What_47;
                        break;
                    default:
                        evToBePushed = GameSpeakEvents::eUnknown_35; //GlukkonSpeak::None
                        break;
                }
                if (evToBePushed != GameSpeakEvents::eUnknown_35)
                {
                    pEventSystem_5BC11C->PushEvent_4218D0(evToBePushed);
                }
            }

            PlaySound_GameSpeak_444AF0(field_1EA_speak, 0, 0, 0);

            if (field_1EA_speak == GlukkonSpeak::Help_6)
            {
                SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetTrue_0);
            }
        }
        field_1EA_speak = GlukkonSpeak::None;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleInput_443BB0();
    }
}

void Glukkon::M_Speak2_12_4438F0()
{
    M_Speak1_11_4437D0();
}

void Glukkon::M_LongLaugh_13_443930()
{
    M_Speak1_11_4437D0();
}

void Glukkon::M_BeginWalk_14_443950()
{
    M_JumpToWalk_19_443A30();
}

void Glukkon::M_EndWalk_15_443970()
{
    M_JumpToStand_17_4439D0();
}

void Glukkon::M_StandToJump_16_4439B0()
{
    M_WalkToJump_18_443A00();
}

void Glukkon::M_JumpToStand_17_4439D0()
{
    DoMovement_444190();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Glukkon::HandleInput_443BB0();
    }
}

void Glukkon::M_WalkToJump_18_443A00()
{
    DoMovement_444190();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim_43F9C0(eGlukkonMotions::M_Jump_4_443030);
    }
}

void Glukkon::M_JumpToWalk_19_443A30()
{
    DoMovement_444190();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim_43F9C0(eGlukkonMotions::M_Walk_1_442D30);
    }
}

void Glukkon::M_KnockBackStandBegin_20_442FC0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        PlaySound_GameSpeak_444AF0(GlukkonSpeak::Heh_5, 0, 0, 0);
        SetAnim_43F9C0(eGlukkonMotions::M_KnockBackStandEnd_22_443010, TRUE);
    }
}

void Glukkon::M_GetShot_21_443A60()
{
    if (!field_100_pCollisionLine)
    {
        M_Fall_7_443510();
        if (field_106_current_motion != eGlukkonMotions::M_GetShot_21_443A60)
        {
            SetAnim_43F9C0(eGlukkonMotions::M_GetShot_21_443A60, TRUE);
        }
    }

    if (static_cast<s32>(sGnFrame_5C1B84) >= field_204_getting_shot_timer)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            field_210_brain_sub_state = 2;
            const FP shotXVel = FP_FromInteger(20) * field_CC_sprite_scale;
            if (field_C4_velx >= FP_FromInteger(0))
            {
                field_C4_velx = field_C4_velx <= FP_FromInteger(0) ? FP_FromInteger(0) : shotXVel;
            }
            else
            {
                field_C4_velx = -shotXVel;
            }
        }
    }

    if (static_cast<s32>(sGnFrame_5C1B84) > field_200_knockback_delay_after_getting_shot_timer)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_C4_velx > FP_FromInteger(0));
        SetAnim_43F9C0(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
    }
}

void Glukkon::M_KnockBackStandEnd_22_443010()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim_43F9C0(eGlukkonMotions::M_Idle_0_442D10, TRUE);
    }
}

void Glukkon::M_Speak3_23_443910()
{
    M_Speak1_11_4437D0();
}

void Glukkon::M_EndSingleStep_24_443990()
{
    M_JumpToStand_17_4439D0();
}

s16 Glukkon::Brain_0_Calm_WalkAround_440B40()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    auto pObj = sObjectIds.Find_449CF0(field_110_id);
    LiftPoint* pLiftPoint = nullptr;
    if (pObj && pObj->Type() == AETypes::eLiftPoint_78)
    {
        pLiftPoint = static_cast<LiftPoint*>(pObj);
        if (!pLiftPoint->vOnAnyFloor_461920() && field_210_brain_sub_state != 7)
        {
            field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
            return 7;
        }
    }

    if (!field_100_pCollisionLine)
    {
        return 8;
    }

    if (sActiveHero_5C1B68->field_10C_health < FP_FromInteger(0))
    {
        Speak_444640(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death_442010);
        return 6;
    }

    BaseAnimatedWithPhysicsGameObject* pEvent17 = nullptr;

    switch (field_210_brain_sub_state)
    {
        case 0:
            if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }

            if (ShouldPanic_440200(FALSE))
            {
                if (Event_Is_Event_In_Range_422C30(
                        kEventAbeOhm,
                        field_B8_xpos,
                        field_BC_ypos,
                        field_D6_scale))
                {
                    field_1D4_timer = sGnFrame_5C1B84 + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame_5C1B84 + field_1A8_tlvData.field_16_scream_help_delay;
                }
                Speak_444640(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }

            pEvent17 = Event_Is_Event_In_Range_422C30(
                kEventUnknown17,
                field_B8_xpos,
                field_BC_ypos,
                field_D6_scale);
            if (pEvent17 && pEvent17 != this)
            {
                field_1D4_timer = sGnFrame_5C1B84 + 20;
                return 9;
            }

            if (field_1A8_tlvData.field_14_behaviour == Path_Glukkon::Behavior::eCheckForWalls_1)
            {
                if (Check_IsOnEndOfLine(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked_4442F0(field_C4_velx, 1))
                {
                    field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                    return 2;
                }
                field_108_next_motion = eGlukkonMotions::M_BeginWalk_14_443950;
                return 1;
            }
            else
            {
                field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
                return 1;
            }
            break;

        case 1:
            if (ShouldPanic_440200(FALSE))
            {
                if (Event_Is_Event_In_Range_422C30(
                        kEventAbeOhm,
                        field_B8_xpos,
                        field_BC_ypos,
                        field_D6_scale))
                {
                    field_1D4_timer = sGnFrame_5C1B84 + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame_5C1B84 + field_1A8_tlvData.field_16_scream_help_delay;
                }

                Speak_444640(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }
            else
            {
                auto pEvent17_1 = Event_Is_Event_In_Range_422C30(
                    kEventUnknown17,
                    field_B8_xpos,
                    field_BC_ypos,
                    field_D6_scale);
                if (pEvent17_1 && pEvent17_1 != this)
                {
                    field_1D4_timer = sGnFrame_5C1B84 + 20;
                    return 9;
                }

                if (field_1A8_tlvData.field_14_behaviour == Path_Glukkon::Behavior::eCheckForWalls_1)
                {
                    if (Check_IsOnEndOfLine(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked_4442F0(field_C4_velx, 1))
                    {
                        if (static_cast<s32>(sGnFrame_5C1B84) <= field_1F0_randomish_speak_timer)
                        {
                            field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
                            field_1D4_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 120);
                            return 4;
                        }
                        else
                        {
                            field_1F0_randomish_speak_timer = sGnFrame_5C1B84 + 120;
                            SpeakRandomish_4405D0();
                            return 3;
                        }
                    }
                }
                else
                {
                    if (Math_NextRandom() < 5 && static_cast<s32>(sGnFrame_5C1B84) > field_1F4_turn_or_help_timer)
                    {
                        field_1F4_turn_or_help_timer = sGnFrame_5C1B84 + 120;
                        field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                        return 2;
                    }
                }

                if (Math_NextRandom() >= 5 || static_cast<s32>(sGnFrame_5C1B84) <= field_1F0_randomish_speak_timer)
                {
                    return field_210_brain_sub_state;
                }

                field_1F0_randomish_speak_timer = sGnFrame_5C1B84 + 120;
                SpeakRandomish_4405D0();
                return 6;
            }
            break;

        case 2:
        case 8:
            if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            return 0;

        case 3:
            if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
            {
                return field_210_brain_sub_state;
            }
            field_1D4_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 120);
            return 4;

        case 4:
            if (ShouldPanic_440200(FALSE))
            {
                if (Event_Is_Event_In_Range_422C30(
                        kEventAbeOhm,
                        field_B8_xpos,
                        field_BC_ypos,
                        field_D6_scale))
                {
                    field_1D4_timer = sGnFrame_5C1B84 + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame_5C1B84 + field_1A8_tlvData.field_16_scream_help_delay;
                }
                Speak_444640(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }

            pEvent17 = Event_Is_Event_In_Range_422C30(
                kEventUnknown17,
                field_B8_xpos,
                field_BC_ypos,
                field_D6_scale);
            if (pEvent17 && pEvent17 != this)
            {
                field_1D4_timer = sGnFrame_5C1B84 + 20;
                return 9;
            }
            else
            {
                if (static_cast<s32>(sGnFrame_5C1B84) <= field_1D4_timer)
                {
                    return field_210_brain_sub_state;
                }
                field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                return 2;
            }
            break;

        case 5:
            if (ShouldPanic_440200(FALSE))
            {
                if (Event_Is_Event_In_Range_422C30(
                        kEventAbeOhm,
                        field_B8_xpos,
                        field_BC_ypos,
                        field_D6_scale))
                {
                    field_1D4_timer = sGnFrame_5C1B84 + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame_5C1B84 + field_1A8_tlvData.field_16_scream_help_delay;
                }
                Speak_444640(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }
            else
            {
                auto pEvent17_3 = Event_Is_Event_In_Range_422C30(
                    kEventUnknown17,
                    field_B8_xpos,
                    field_BC_ypos,
                    field_D6_scale);
                if (pEvent17_3 && pEvent17_3 != this)
                {
                    field_1D4_timer = sGnFrame_5C1B84 + 20;
                    return 9;
                }
                else
                {
                    if (static_cast<s32>(sGnFrame_5C1B84) <= field_1D4_timer)
                    {
                        return field_210_brain_sub_state;
                    }
                    return 0;
                }
            }
            break;

        case 6:
            if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            field_1D4_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 120);
            return 5;

        case 7:
            if (pLiftPoint)
            {
                if (!pLiftPoint->vOnAnyFloor_461920())
                {
                    return field_210_brain_sub_state;
                }
                return 0;
            }
            else
            {
                field_110_id = -1;
                return 0;
            }
            break;

        case 9:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_1D4_timer)
            {
                return field_210_brain_sub_state;
            }
            field_1FC = 1;
            Speak_444640(GlukkonSpeak::Heh_5);
            return 6;

        default:
            return field_210_brain_sub_state;
    }
}

s16 Glukkon::Brain_1_Panic_4412F0()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_449CF0(field_110_id));
    if (pLiftPoint && pLiftPoint->Type() == AETypes::eLiftPoint_78 && !pLiftPoint->vOnAnyFloor_461920() && field_210_brain_sub_state != 6)
    {
        field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
        return 6;
    }

    if (!field_100_pCollisionLine)
    {
        return 7;
    }

    if (sActiveHero_5C1B68->field_10C_health < FP_FromInteger(0))
    {
        Glukkon::Speak_444640(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death_442010);
        return 6;
    }

    switch (field_210_brain_sub_state)
    {
        case 0:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_1D4_timer || field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            field_1F8_panic_timer = sGnFrame_5C1B84;
            Speak_444640(GlukkonSpeak::Help_6);
            return 4;

        case 1:
            if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }

            if (ShouldPanic_440200(TRUE))
            {
                field_1F8_panic_timer = sGnFrame_5C1B84;
            }
            else if (static_cast<s32>(sGnFrame_5C1B84) - field_1F8_panic_timer > field_1A8_tlvData.field_1A_to_calm_delay)
            {
                SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetFalse_1);
                SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);
                return 0;
            }

            if (field_1A8_tlvData.field_14_behaviour != Path_Glukkon::Behavior::eIgnoreWalls_0)
            {
                if (Check_IsOnEndOfLine(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked_4442F0(field_C4_velx, 1))
                {
                    field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                    return 3;
                }
                field_108_next_motion = eGlukkonMotions::M_BeginWalk_14_443950;
            }
            else
            {
                field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
            }
            return 2;

        case 2:
            if (field_1A8_tlvData.field_14_behaviour == Path_Glukkon::Behavior::eCheckForWalls_1)
            {
                if (Check_IsOnEndOfLine(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked_4442F0(field_C4_velx, 1))
                {
                    Glukkon::Speak_444640(GlukkonSpeak::Help_6);
                    return 5;
                }
            }
            else
            {
                if (Math_NextRandom() < 5u && static_cast<s32>(sGnFrame_5C1B84) > field_1F4_turn_or_help_timer)
                {
                    field_1F4_turn_or_help_timer = sGnFrame_5C1B84 + 120;
                    Glukkon::Speak_444640(GlukkonSpeak::Help_6);
                    return 5;
                }
            }
            if (Math_NextRandom() >= 0xAu || static_cast<s32>(sGnFrame_5C1B84) <= field_1F0_randomish_speak_timer)
            {
                return field_210_brain_sub_state;
            }
            Glukkon::Speak_444640(GlukkonSpeak::Help_6);
            return 4;

        case 3:
            if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            return 1;

        case 4:
            if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            field_1F0_randomish_speak_timer = sGnFrame_5C1B84 + 60;
            return 1;

        case 5:
            if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
            {
                return field_210_brain_sub_state;
            }
            field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
            return 3;

        case 6:
            if (pLiftPoint)
            {
                if (!pLiftPoint->vOnAnyFloor_461920())
                {
                    return field_210_brain_sub_state;
                }
            }
            else
            {
                field_110_id = -1;
            }
            return 1;

        case 7:
            if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            return 1;

        default:
            return field_210_brain_sub_state;
    }
}

s16 Glukkon::Brain_2_Slapped_441720()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    if (field_106_current_motion == eGlukkonMotions::M_Fall_7_443510)
    {
        return 3;
    }

    if (sActiveHero_5C1B68->field_10C_health < FP_FromInteger(0))
    {
        Glukkon::Speak_444640(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death_442010);
        return 6;
    }

    switch (field_210_brain_sub_state)
    {
        case 0:
            if (field_106_current_motion)
            {
                return field_210_brain_sub_state;
            }

            if (ShouldPanic_440200(TRUE))
            {
                field_1F8_panic_timer = sGnFrame_5C1B84;
            }
            else if ((s32)(sGnFrame_5C1B84 - field_1F8_panic_timer) > field_1A8_tlvData.field_1A_to_calm_delay)
            {
                SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetFalse_1);
                SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);

                if (FP_FromInteger(field_1A8_tlvData.field_8_top_left.field_0_x) >= field_B8_xpos)
                {
                    if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
                    {
                        return 2;
                    }
                    if (FP_FromInteger(field_1A8_tlvData.field_8_top_left.field_0_x) >= field_B8_xpos)
                    {
                        return 0;
                    }
                }

                if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
                {
                    return 0;
                }
                return 2;
            }

            if ((field_100_pCollisionLine && Check_IsOnEndOfLine(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 4)) || PathBlocked_4442F0(field_C4_velx, 0))
            {
                field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                return 2;
            }
            else
            {
                field_108_next_motion = eGlukkonMotions::M_Jump_4_443030;
                return 1;
            }
            break;

        case 1:
            if (ShouldPanic_440200(TRUE))
            {
                field_1F8_panic_timer = sGnFrame_5C1B84;
            }
            else if (static_cast<s32>(sGnFrame_5C1B84 - field_1F8_panic_timer) > field_1A8_tlvData.field_1A_to_calm_delay)
            {
                field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
                return 0;
            }

            if (!field_100_pCollisionLine || (!Check_IsOnEndOfLine(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 4) && !PathBlocked_4442F0(field_C4_velx, 0)))
            {
                if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame_5C1B84) <= field_1F0_randomish_speak_timer)
                {
                    return field_210_brain_sub_state;
                }
                Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Help_6, 0, 0, 0);
                field_1F0_randomish_speak_timer = sGnFrame_5C1B84 + 40;
                return field_210_brain_sub_state;
            }

            field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
            return 2;

        case 2:
        case 3:
            if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            return 0;

        default:
            return field_210_brain_sub_state;
    }
}

s16 Glukkon::Brain_3_PlayerControlled_441A30()
{
    auto pDeathFadeOut = static_cast<DeathFadeOut*>(sObjectIds.Find_449CF0(field_208_obj_id));
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    switch (field_210_brain_sub_state)
    {
        case 0:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_1D4_timer)
            {
                return field_210_brain_sub_state;
            }
            ToStand_443B70();
            return 1;

        case 1:
        {
            if ((field_1A8_tlvData.field_22_glukkon_type != GlukkonTypes::StoryAslik_1 &&
                field_1A8_tlvData.field_22_glukkon_type != GlukkonTypes::StoryDripik_2 &&
                field_1A8_tlvData.field_22_glukkon_type != GlukkonTypes::StoryPhleg_3) ||
                !SwitchStates_Get_466020(field_1A8_tlvData.field_26_play_movie_switch_id))
            {
                if (Input_IsChanting_45F260() && field_106_current_motion != eGlukkonMotions::M_Jump_4_443030 && !field_1E2_prevent_depossession)
                {
                    field_1D4_timer = sGnFrame_5C1B84 + 30;
                    SFX_Play_46FA90(SoundEffect::PossessEffect_17, 0);
                    SetAnim_43F9C0(10, TRUE);
                    return 2;
                }
                return field_210_brain_sub_state;
            }

            if (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryAslik_1)
            {
                sVisitedFeecoEnder_5C1C06 = 1;
            }
            else if (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryDripik_2)
            {
                sVisitedBarracks_5C1C04 = 1;
            }
            else
            {
                sVisitedBonewerks_5C1C02 = 1;
            }

            auto pDeathFadeOutMem = ae_new<DeathFadeOut>(Layer::eLayer_FadeFlash_40, 1, 0, 8, TPageAbr::eBlend_2);
            if (pDeathFadeOutMem)
            {
                field_208_obj_id = pDeathFadeOutMem->field_8_object_id;
            }
        }
            return 3;

        case 2:
            if (Input_IsChanting_45F260()
                || ((field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryAslik_1 || field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryDripik_2 || field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryPhleg_3)
                    && SwitchStates_Get_466020(field_1A8_tlvData.field_26_play_movie_switch_id)))
            {
                if (!(static_cast<s32>(sGnFrame_5C1B84) % 4))
                {
                    const FP xRand = FP_FromInteger(Math_RandomRange_496AB0(-20, 20));
                    const FP yRand = FP_FromInteger(Math_RandomRange_496AB0(20, 50));
                    New_TintChant_Particle_426BE0(
                        (field_CC_sprite_scale * xRand) + field_B8_xpos,
                        field_BC_ypos - (field_CC_sprite_scale * yRand),
                        field_CC_sprite_scale,
                        Layer::eLayer_0);
                }

                if (static_cast<s32>(sGnFrame_5C1B84) > field_1D4_timer || sActiveHero_5C1B68->field_10C_health <= FP_FromInteger(0))
                {
                    field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
                    SetBrain(&Glukkon::Brain_4_Death_442010);
                    field_210_brain_sub_state = 2;
                    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);
                }
                return field_210_brain_sub_state;
            }
            ToStand_443B70();
            return 1;

        case 3:
            if (pDeathFadeOut && !pDeathFadeOut->field_7E_bDone)
            {
                return field_210_brain_sub_state;
            }

            for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
            {
                auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() == AETypes::eSlig_125)
                {
                    pObj->mFlags.Set(BaseGameObject::eDead);
                }
            }

            SetUpdateDelay(2);
            return 4;

        case 4:
        {
            pScreenManager_5BB5F4->field_40_flags &= ~0x10000;
            sLevelId_dword_5CA408 = static_cast<u32>(gMap.mCurrentLevel);

            const FmvInfo* pFmvRec = Path_Get_FMV_Record_460F70(gMap.mCurrentLevel, field_1A8_tlvData.field_28_movie_to_play_fmvID);

            u32 pos = 0;
            Get_fmvs_sectors_494460(pFmvRec->field_0_pName, 0, 0, &pos, 0, 0);

            ae_new<Movie>(pFmvRec->field_4_id, pos, pFmvRec->field_6_flags & 1, pFmvRec->field_8_flags, pFmvRec->field_A_volume);
        }
            return 5;

        case 5:
            if (sMovie_ref_count_BB4AE4)
            {
                return field_210_brain_sub_state;
            }
            gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();
            pScreenManager_5BB5F4->DecompressCameraToVRam_40EF60((u16**) gMap.field_2C_camera_array[0]->field_C_pCamRes); // TODO: Cast hack
            if (pDeathFadeOut)
            {
                pDeathFadeOut->Init_427140(Layer::eLayer_FadeFlash_40, 0, 1, 8);
            }
            return 6;

        case 6:
            GetSoundAPI().SND_Restart();
            pScreenManager_5BB5F4->field_40_flags |= 0x10000;
            field_1D4_timer = sGnFrame_5C1B84 + 30;
            SFX_Play_46FA90(SoundEffect::PossessEffect_17, 0);
            SetAnim_43F9C0(10, TRUE);
            return 2;

        default:
            return field_210_brain_sub_state;
    }
}

static GibType AsGibType(GlukkonTypes glukkonType)
{
    switch (glukkonType)
    {
        case GlukkonTypes::Normal_0:
            return GibType::Glukkon_6;

        case GlukkonTypes::StoryAslik_1:
            return GibType::Aslik_7;

        case GlukkonTypes::StoryDripik_2:
            return GibType::Dripik_8;

        case GlukkonTypes::StoryPhleg_3:
            return GibType::Phleg_9;

        case GlukkonTypes::Normal_4:
            ALIVE_FATAL("Didn't expect glukkon type 4 for gibs!");
            //return GibType::Fleech_10; // TODO: This can't be correct

        case GlukkonTypes::Normal_5:
            ALIVE_FATAL("Didn't expect glukkon type 5 for gibs!");
            //return GibType::Glukkon_6; // TODO: Was out of range

        default:
            return GibType::Glukkon_6;
    }
}

s16 Glukkon::Brain_4_Death_442010()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (field_210_brain_sub_state)
    {
        case 0:
            if (field_106_current_motion != eGlukkonMotions::M_DeathFall_8_443760 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_210_brain_sub_state;
            }
            else
            {
                field_10C_health = FP_FromInteger(0);
                field_1D4_timer = sGnFrame_5C1B84 + 90;
                return 1;
            }
            break;

        case 1:
            if (static_cast<s32>(sGnFrame_5C1B84) > field_1D4_timer)
            {
                ToDead_43F640();
                return field_210_brain_sub_state;
            }
            else
            {
                field_CC_sprite_scale -= FP_FromDouble(0.008);

                field_D0_r -= 2;
                field_D2_g -= 2;
                field_D4_b -= 2;

                DeathSmokeEffect(true);

                return field_210_brain_sub_state;
            }
            break;

        case 2:
        {
            ae_new<Gibs>(
                AsGibType(field_1A8_tlvData.field_22_glukkon_type),
                field_B8_xpos,
                field_BC_ypos,
                field_C4_velx,
                field_C8_vely,
                field_CC_sprite_scale,
                0);

            ae_new<Blood>(
                field_B8_xpos,
                field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale,
                20);

            New_Smoke_Particles_426C70(
                field_B8_xpos,
                field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
                field_CC_sprite_scale,
                3,
                128u,
                128u,
                128u);

            SFX_Play_46FA90(SoundEffect::KillEffect_64, 128, field_CC_sprite_scale);
            SFX_Play_46FA90(SoundEffect::FallingItemHit_47, 90, field_CC_sprite_scale);

            field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

            SetAnim_43F9C0(eGlukkonMotions::M_ChantShake_10_443B50, TRUE);

            field_C8_vely = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            field_10C_health = FP_FromInteger(0);
            field_1D4_timer = sGnFrame_5C1B84 + 40;
        }
            return 3;

        case 3:
            if (static_cast<s32>(sGnFrame_5C1B84) > field_1D4_timer)
            {
                ToDead_43F640();
            }
            return field_210_brain_sub_state;

        case 4:
        case 5:
            if (!field_100_pCollisionLine || field_106_current_motion != eGlukkonMotions::M_KnockBack_3_442F40 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_210_brain_sub_state;
            }
            else
            {
                field_1D4_timer = sGnFrame_5C1B84 + 90;
                return 1;
            }
            break;

        default:
            return field_210_brain_sub_state;
    }
}

const PSX_Point v00554768[8] = {
    {-25, -5},
    {-30, -22},
    {-25, -39},
    {-10, -51},
    {6, -39},
    {9, -23},
    {5, -6},
    {0, 0}};


s16 Glukkon::Brain_5_WaitToSpawn_442490()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    if (field_210_brain_sub_state == 0)
    {
        if (!SwitchStates_Get_466020(field_1A8_tlvData.field_1C_spawn_switch_id))
        {
            return field_210_brain_sub_state;
        }
        field_1D4_timer = sGnFrame_5C1B84 + field_1A8_tlvData.field_20_spawn_delay;
        return 1;
    }
    else if (field_210_brain_sub_state == 2)
    {
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
        {
            return field_210_brain_sub_state;
        }
        SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);
        field_210_brain_sub_state = 0;
        return field_210_brain_sub_state;
    }
    else if (field_210_brain_sub_state == 1)
    {
        if (static_cast<s32>(sGnFrame_5C1B84) <= field_1D4_timer)
        {
            return field_210_brain_sub_state;
        }

        mFlags.Set(BaseGameObject::eDrawable_Bit4);
        field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);

        SetType(AETypes::eGlukkon_67);

        if (field_1A8_tlvData.field_1E_spawn_type == Path_Glukkon::SpawnType::eFullSpawnEffects_3)
        {
            SFX_Play(SoundEffect::Zap1_49, 60, -300);

            for (const auto& p : v00554768)
            {
                const s16 sparkX = FP_GetExponent(FP_FromInteger(p.field_0_x) + field_B8_xpos + FP_FromInteger(13));
                const s16 sparkY = FP_GetExponent(field_BC_ypos + FP_FromInteger(p.field_2_y) - FP_FromInteger(11));
                ae_new<Spark>(FP_FromInteger(sparkX), FP_FromInteger(sparkY), FP_FromInteger(1), 9, -31, 159, SparkType::eBigChantParticle_1);
            }

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            New_DestroyOrCreateObject_Particle_426F40(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2) + (field_CC_sprite_scale * FP_FromInteger(60)),
                field_CC_sprite_scale);

            ae_new<ParticleBurst>(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(18),
                6,
                FP_FromInteger(1),
                BurstType::eBigRedSparks_3,
                9);

            Speak_444640(GlukkonSpeak::Heh_5);
            return 2;
        }
        else
        {
            SFX_Play_46FA90(SoundEffect::GlukkonSpawn_112, 0);
            Speak_444640(GlukkonSpeak::Heh_5);
            return 2;
        }
    }
    return field_210_brain_sub_state;
}

void Glukkon::Init_43F260()
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    mFlags.Set(BaseGameObject::eDrawable_Bit4);

    SetTint_425600(&kGlukkonTints_5546B4[0], gMap.mCurrentLevel);
    field_B8_xpos = FP_FromInteger((field_1A8_tlvData.field_8_top_left.field_0_x + field_1A8_tlvData.field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(field_1A8_tlvData.field_8_top_left.field_2_y);

    if (field_1A8_tlvData.field_12_start_direction == Path_Glukkon::StartDirection::eLeft_1)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    if (field_1A8_tlvData.field_1C_spawn_switch_id)
    {
        if (field_1A8_tlvData.field_1E_spawn_type == Path_Glukkon::SpawnType::eFacingLeft_1)
        {
            field_B8_xpos -= ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }
        else if (field_1A8_tlvData.field_1E_spawn_type == Path_Glukkon::SpawnType::eFacingRight_2)
        {
            field_B8_xpos += ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        mFlags.Clear(BaseGameObject::eDrawable_Bit4);
        SetBrain(&Glukkon::Brain_5_WaitToSpawn_442490);
        field_210_brain_sub_state = 0;
        SetType(AETypes::eNone_0);
    }
    else
    {
        field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
        SetType(AETypes::eGlukkon_67);
        SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);
        field_210_brain_sub_state = 0;
    }

    if (field_1A8_tlvData.field_10_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = Layer::eLayer_8;
    }
    else if (field_1A8_tlvData.field_10_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = Layer::eLayer_27;
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos + FP_FromInteger(79),
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16)
        == 1)
    {
        field_BC_ypos = hitY;
    }

    SetAnim_43F9C0(0, TRUE);

    field_208_obj_id = -1;
    field_110_id = -1;
    field_1D4_timer = 0;
    field_10C_health = FP_FromInteger(1);
    field_1F0_randomish_speak_timer = 0;
    field_1F4_turn_or_help_timer = 0;
    field_1F8_panic_timer = 0;
    field_1EA_speak = GlukkonSpeak::None;
    field_1E0_gamespeak_pitch = 0;
    field_1FC = 0;
    field_200_knockback_delay_after_getting_shot_timer = 0;
    field_204_getting_shot_timer = 0;
    field_1DC_previous_ypos = field_BC_ypos;

    if (!field_E0_pShadow)
    {
        field_E0_pShadow = ae_new<Shadow>();
    }
}

Glukkon::~Glukkon()
{
    if (field_10C_health <= FP_FromInteger(0))
    {
        Path::TLV_Reset_4DB8E0(field_214_tlv_info, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_214_tlv_info, -1, 0, 0);
    }

    mFlags.Set(BaseGameObject::eDrawable_Bit4); // Seems wrong to do this here ??

    if (this == sControlledCharacter_5C1B8C)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
    }
}

void Glukkon::VUpdate()
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

            if (field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_32 || field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_36)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);

                PSX_Point xy = {bRect.x, bRect.y};
                PSX_Point wh = {bRect.w, bRect.h};
                xy.field_2_y += 5;
                wh.field_2_y += 5;

                vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
            }
        }
        field_208_obj_id = BaseGameObject::RefreshId(field_208_obj_id);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (!Input_IsChanting_45F260())
        {
            field_1E2_prevent_depossession = 0;
        }

        const auto oldMotion = field_106_current_motion;

        field_210_brain_sub_state = (this->*field_20C_brain_state_fn)();

        const FP oldXPos = field_B8_xpos;
        const FP oldYPos = field_BC_ypos;

        (this->*sGlukkon_motion_table_5544C0[field_106_current_motion])();

        // TODO: This is extra debug logging to figure out the motion names
        if (oldMotion != field_106_current_motion)
        {
            //LOG_INFO("Glukkon: Old motion = " << sGlukkonMotionNames[oldMotion] << " new motion = " << sGlukkonMotionNames[field_106_current_motion]);
        }

        if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
        {
            Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            VOn_TLV_Collision(pTlv);
        }

        Update_Slurg_WatchPoints_440600();

        if (sControlledCharacter_5C1B8C == this && field_110_id != -1)
        {
            field_C8_vely = field_BC_ypos - field_1DC_previous_ypos;
            SetActiveCameraDelayedFromDir();
        }
        field_1DC_previous_ypos = field_BC_ypos;
    }
}

void Glukkon::VPossessed()
{
    SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetFalse_1);
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_1E2_prevent_depossession = 1;
    SetAnim_43F9C0(eGlukkonMotions::M_ChantShake_10_443B50, TRUE);
    SetBrain(&Glukkon::Brain_3_PlayerControlled_441A30);
    field_210_brain_sub_state = 0;
    field_1D4_timer = sGnFrame_5C1B84 + 35;
    field_1E4_level = gMap.mCurrentLevel;
    field_1E6_path = gMap.mCurrentPath;
    field_1E8_camera = gMap.mCurrentCamera;
}

void Glukkon::Update_Slurg_WatchPoints_440600()
{
    if (field_106_current_motion == eGlukkonMotions::M_Walk_1_442D30 || (field_106_current_motion == eGlukkonMotions::M_Jump_4_443030 && field_20_animation.field_92_current_frame > 8))
    {
        if (sGnFrame_5C1B84 & 1)
        {
            const s8 count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
            if (count < 5)
            {
                Slurg_Step_Watch_Points* pPoints = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
                pPoints->field_0_points[count].field_0_xPos = FP_GetExponent(field_B8_xpos);
                pPoints->field_0_points[count].field_2_yPos = field_100_pCollisionLine->field_0_rect.y - 5;
                sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = count + 1;
            }
        }
    }
}

void Glukkon::SetAnim_43F9C0(s16 currentMotion, s16 bClearNextMotion)
{
    s32 typeIndex = static_cast<s32>(field_1A8_tlvData.field_22_glukkon_type);
    if (typeIndex > 3)
    {
        typeIndex = 0;
    }

    const AnimRecord& animRec = AnimRec(sGlukkonsFrameTableOffsetTable_554524[typeIndex][currentMotion]);
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);

    field_106_current_motion = currentMotion;
    if (bClearNextMotion)
    {
        field_108_next_motion = -1;
    }
}

void Glukkon::Speak_444640(GlukkonSpeak speak)
{
    switch (speak)
    {
        case GlukkonSpeak::Hey_0:
        case GlukkonSpeak::DoIt_1:
        case GlukkonSpeak::Heh_5:
        case GlukkonSpeak::Help_6:
            field_108_next_motion = eGlukkonMotions::M_Speak1_11_4437D0;
            field_1EA_speak = speak;
            break;

        case GlukkonSpeak::StayHere_2:
            field_108_next_motion = eGlukkonMotions::M_Speak3_23_443910;
            field_1EA_speak = speak;
            break;

        case GlukkonSpeak::Commere_3:
        case GlukkonSpeak::AllOYa_4:
        case GlukkonSpeak::KillEm_8:
        case GlukkonSpeak::Unused_9:
        case GlukkonSpeak::Unused_10:
            field_108_next_motion = eGlukkonMotions::M_Speak2_12_4438F0;
            field_1EA_speak = speak;
            break;

        case GlukkonSpeak::Laugh_7:
            field_108_next_motion = eGlukkonMotions::M_LongLaugh_13_443930;
            field_1EA_speak = speak;
            break;

        default:
            field_1EA_speak = speak;
            break;
    }
}

void Glukkon::HandleInput_443BB0()
{
    MapFollowMe(TRUE);

    if (BrainIs(&Glukkon::Brain_3_PlayerControlled_441A30) && field_210_brain_sub_state == 1 && !(field_114_flags.Get(Flags_114::e114_Bit10_Teleporting)))
    {
        const auto inputHeld = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
        const auto matchButtons = InputCommands::Enum::eGameSpeak1 | InputCommands::Enum::eGameSpeak2 | InputCommands::Enum::eGameSpeak3 | InputCommands::Enum::eGameSpeak4 | InputCommands::Enum::eGameSpeak5 | InputCommands::Enum::eGameSpeak6 | InputCommands::Enum::eGameSpeak7 | InputCommands::Enum::eGameSpeak8 | InputCommands::Enum::eChant;

        if (inputHeld & matchButtons)
        {
            field_1EA_speak = GlukkonSpeak::None;

            if (inputHeld & InputCommands::Enum::eGameSpeak1)
            {
                field_1EA_speak = GlukkonSpeak::Hey_0;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak2)
            {
                field_1EA_speak = GlukkonSpeak::Commere_3;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak3)
            {
                field_1EA_speak = GlukkonSpeak::StayHere_2;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak4)
            {
                field_1EA_speak = GlukkonSpeak::DoIt_1;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak5)
            {
                field_1EA_speak = GlukkonSpeak::KillEm_8;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak7)
            {
                field_1EA_speak = GlukkonSpeak::Help_6;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak8)
            {
                field_1EA_speak = GlukkonSpeak::Laugh_7;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak6)
            {
                field_1EA_speak = GlukkonSpeak::AllOYa_4;
            }

            if (field_1EA_speak != GlukkonSpeak::None)
            {
                Speak_444640(field_1EA_speak);
            }
        }
        else
        {
            const auto inputPressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            if (inputPressed & InputCommands::Enum::eRight)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                }
                else
                {
                    field_108_next_motion = eGlukkonMotions::M_BeginWalk_14_443950;
                }
            }
            else if (inputPressed & InputCommands::Enum::eLeft)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_108_next_motion = eGlukkonMotions::M_BeginWalk_14_443950;
                }
                else
                {
                    field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                }
            }

            if (inputPressed & InputCommands::Enum::eRun)
            {
                if (field_108_next_motion == eGlukkonMotions::M_Walk_1_442D30)
                {
                    field_108_next_motion = eGlukkonMotions::M_Jump_4_443030;
                }
            }

            if (inputPressed & InputCommands::Enum::eHop)
            {
                field_108_next_motion = eGlukkonMotions::M_Jump_4_443030;
            }
        }
    }

    switch (field_108_next_motion)
    {
        case -1:
            if (field_106_current_motion == eGlukkonMotions::M_Idle_0_442D10)
            {
                return;
            }
            field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
            SetAnim_43F9C0(field_108_next_motion, TRUE);
            break;

        case eGlukkonMotions::M_Idle_0_442D10:
        case eGlukkonMotions::M_Turn_2_442F10:
        case eGlukkonMotions::M_Speak1_11_4437D0:
        case eGlukkonMotions::M_Speak2_12_4438F0:
        case eGlukkonMotions::M_LongLaugh_13_443930:
        case eGlukkonMotions::M_Speak3_23_443910:
            SetAnim_43F9C0(field_108_next_motion, TRUE);
            break;

        case eGlukkonMotions::M_Walk_1_442D30:
        case eGlukkonMotions::M_BeginWalk_14_443950:
            if (field_106_current_motion != eGlukkonMotions::M_Walk_1_442D30)
            {
                field_212_currentWalkPitch = 0;
            }
            [[fallthrough]];

        case eGlukkonMotions::M_Jump_4_443030:
        case eGlukkonMotions::M_StandToJump_16_4439B0:
        {
            FP xOff = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                xOff = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), xOff))
            {
                SetAnim_43F9C0(field_108_next_motion, TRUE);
            }
        }
        break;

        default:
            return;
    }
}

s16 Glukkon::ShouldPanic_440200(s16 panicEvenIfNotFacingMe)
{
    if (IsLineOfSightBetween_4403B0(this, sControlledCharacter_5C1B8C)
        && !(sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
        && !BaseAliveGameObject::IsInInvisibleZone(sControlledCharacter_5C1B8C)
        && !Event_Get_422C00(kEventResetting)
        && gMap.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0)
        && gMap.Is_Point_In_Current_Camera_4810D0(
            sControlledCharacter_5C1B8C->field_C2_lvl_number,
            sControlledCharacter_5C1B8C->field_C0_path_number,
            sControlledCharacter_5C1B8C->field_B8_xpos,
            sControlledCharacter_5C1B8C->field_BC_ypos,
            0)
        && (panicEvenIfNotFacingMe || vIsFacingMe_4254A0(sControlledCharacter_5C1B8C)))
    {
        return 1;
    }

    // Panic if Abe is chanting
    if (Event_Is_Event_In_Range_422C30(
            kEventAbeOhm,
            field_B8_xpos,
            field_BC_ypos,
            field_D6_scale))
    {
        return 1;
    }

    // Panic if an alarm is on
    if (Event_Get_422C00(kEventAlarm))
    {
        return 1;
    }

    // Panic if player is speaking (I guess it can be rather alarming?)
    auto pSpeakEvent = Event_Is_Event_In_Range_422C30(
        kEventSpeaking,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale);
    return pSpeakEvent && pSpeakEvent == sControlledCharacter_5C1B8C;
}

s16 Glukkon::PathBlocked_4442F0(FP /*a2*/, s16 checkBounds)
{
    FP gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (field_106_current_motion == eGlukkonMotions::M_Jump_4_443030)
    {
        gridSize = (gridSize * FP_FromInteger(4));
    }

    Path_EnemyStopper::StopDirection direction = Path_EnemyStopper::StopDirection::Left_0;
    TlvTypes boundType = TlvTypes::ScrabLeftBound_43;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        boundType = TlvTypes::ScrabLeftBound_43;
        direction = Path_EnemyStopper::StopDirection::Left_0;
        gridSize = -gridSize;
    }
    else
    {
        boundType = TlvTypes::ScrabRightBound_44;
        direction = Path_EnemyStopper::StopDirection::Right_1;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), gridSize * FP_FromInteger(1)))
    {
        return 1;
    }

    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos), // TODO Abs() ??
        FP_GetExponent(field_B8_xpos + gridSize),
        FP_GetExponent(field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale)),
        TlvTypes::SlamDoor_85);

    auto pSlamDoorTlv = static_cast<Path_SlamDoor*>(field_FC_pPathTLV);

    if (pSlamDoorTlv && ((pSlamDoorTlv->field_10_bStart_closed == Choice_short::eYes_1 && !SwitchStates_Get_466020(pSlamDoorTlv->field_14_switch_id)) || (pSlamDoorTlv->field_10_bStart_closed == Choice_short::eNo_0 && SwitchStates_Get_466020(pSlamDoorTlv->field_14_switch_id))))
    {
        return 1;
    }

    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos + gridSize),
        FP_GetExponent(field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale)),
        TlvTypes::EnemyStopper_47);

    auto pEnemyStopper = static_cast<Path_EnemyStopper*>(field_FC_pPathTLV);
    if (pEnemyStopper
        && (pEnemyStopper->field_10_stop_direction == direction || pEnemyStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Both_2)
        && SwitchStates_Get_466020(pEnemyStopper->field_12_switch_id))
    {
        return 1;
    }

    if (!checkBounds)
    {
        return 0;
    }

    if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_B8_xpos),
            FP_GetExponent(field_BC_ypos), // TODO: Abs() ??
            FP_GetExponent(field_B8_xpos + gridSize),
            FP_GetExponent(field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale)),
            boundType))
    {
        return 1;
    }

    return 0;
}

void Glukkon::SpeakRandomish_4405D0()
{
    if (sGnFrame_5C1B84 & 1)
    {
        if ((sGnFrame_5C1B84 & 1) == 1)
        {
            Speak_444640(GlukkonSpeak::Heh_5);
        }
    }
    else
    {
        Speak_444640(GlukkonSpeak::Laugh_7);
    }
}

const FP sWalkData_545354[18] = {
    FP_FromInteger(0),
    FP_FromDouble(2.1124420166015625),
    FP_FromDouble(3.923065185546875),
    FP_FromDouble(2.1124420166015625),
    FP_FromDouble(0.816680908203125),
    FP_FromDouble(4.986083984375),
    FP_FromDouble(6.017730712890625),
    FP_FromDouble(2.9228973388671875),
    FP_FromDouble(2.1124114990234375),
    FP_FromDouble(3.923095703125),
    FP_FromDouble(2.1124114990234375),
    FP_FromDouble(0.816680908203125),
    FP_FromDouble(4.986114501953125),
    FP_FromDouble(6.0177154541015625),
    FP_FromDouble(2.9228973388671875),
    FP_FromDouble(2.1124114990234375),
    FP_FromDouble(3.923095703125),
    FP_FromDouble(2.1124114990234375)};


// These tables just contain all zeros
// TODO/NOTE: These are all pointless - the logic in 0x444190 will use 0 if there is no table
const FP sJumpToFallData_54542C[10] = {};
const FP sWalkToFallData_54541C[4] = {};
const FP sBeginWalkData_545454[6] = {};
const FP sEndWalkData_54546C[6] = {};
const FP sStandToJumpData_545484[6] = {};
const FP sJumpToStandData_54549C[6] = {};
const FP sWalkToJumpData_5454B4[6] = {};
const FP sJumpToWalkData_5454CC[26] = {};

const FP* motion_velx_table_5547C4[25] = {
    nullptr,                 // M_Idle_0_442D10
    sWalkData_545354,        // M_Walk_1_442D30
    nullptr,                 // M_Turn_2_442F10
    nullptr,                 // M_KnockBack_3_442F40
    sGlukkonJumpVelX_54539C, // M_Jump_4_443030
    sJumpToFallData_54542C,  // M_JumpToFall_5_4434C0
    sWalkToFallData_54541C,  // M_WalkToFall_6_4434E0
    nullptr,                 // M_Fall_7_443510
    nullptr,                 // M_DeathFall_8_443760
    nullptr,                 // M_Land_9_443790
    nullptr,                 // M_ChantShake_10_443B50
    nullptr,                 // M_Speak1_11_4437D0
    nullptr,                 // M_Speak2_12_4438F0
    nullptr,                 // M_LongLaugh_13_443930
    sBeginWalkData_545454,   // M_BeginWalk_14_443950
    sEndWalkData_54546C,     // M_EndWalk_15_443970
    sStandToJumpData_545484, // M_StandToJump_16_4439B0
    sJumpToStandData_54549C, // M_JumpToStand_17_4439D0
    sWalkToJumpData_5454B4,  // M_WalkToJump_18_443A00
    sJumpToWalkData_5454CC,  // M_JumpToWalk_19_443A30
    nullptr,                 // M_KnockBackStandBegin_20_442FC0
    nullptr,                 // M_GetShot_21_443A60
    nullptr,                 // M_KnockBackStandEnd_22_443010
    nullptr,                 // M_Speak3_23_443910
    nullptr                  // M_EndSingleStep_24_443990
};


s16 Glukkon::DoMovement_444190()
{
    const FP* pTable = motion_velx_table_5547C4[field_106_current_motion];
    if (pTable)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -pTable[field_20_animation.field_92_current_frame];
        }
        else
        {
            field_C4_velx = pTable[field_20_animation.field_92_current_frame];
        }
    }
    else
    {
        field_C4_velx = FP_FromInteger(0);
    }

    field_C4_velx = field_C4_velx * field_CC_sprite_scale;

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        field_1D8_falling_velx_scale_factor = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
        }
        else
        {
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
        }
        MapFollowMe(TRUE);
        SetAnim_43F9C0(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
        return FALSE;
    }
    else
    {
        FollowLine_443EB0();
        return field_100_pCollisionLine != nullptr;
    }
}

void Glukkon::FollowLine_443EB0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_449CF0(field_110_id));
    const FP prevXPos = field_B8_xpos;

    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        if (field_100_pCollisionLine)
        {
            if (pPlatform)
            {
                if (field_100_pCollisionLine->field_8_type != 32 && field_100_pCollisionLine->field_8_type != 36)
                {
                    pPlatform->VRemove(this);
                    field_110_id = -1;
                    field_1D8_falling_velx_scale_factor = FP_FromDouble(0.35);
                }
            }
            else if (field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_32 || field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_36)
            {
                GetOnPlatforms_444060();
            }
        }
        else
        {
            field_F8_LastLineYPos = field_BC_ypos;

            VOnTrapDoorOpen();

            if (field_106_current_motion == eGlukkonMotions::M_Walk_1_442D30 || field_106_current_motion == eGlukkonMotions::M_BeginWalk_14_443950 || field_106_current_motion == eGlukkonMotions::M_EndWalk_15_443970 || field_106_current_motion == eGlukkonMotions::M_WalkToJump_18_443A00)
            {
                SetAnim_43F9C0(eGlukkonMotions::M_WalkToFall_6_4434E0, TRUE);
            }
            else if (field_106_current_motion == eGlukkonMotions::M_Jump_4_443030 || field_106_current_motion == eGlukkonMotions::M_StandToJump_16_4439B0 || field_106_current_motion == eGlukkonMotions::M_JumpToStand_17_4439D0 || field_106_current_motion == eGlukkonMotions::M_JumpToWalk_19_443A30)
            {
                SetAnim_43F9C0(eGlukkonMotions::M_JumpToFall_5_4434C0, TRUE);
            }
            else
            {
                SetAnim_43F9C0(eGlukkonMotions::M_Fall_7_443510, TRUE);
            }

            field_B8_xpos = prevXPos + field_C4_velx;
            field_1D8_falling_velx_scale_factor = FP_FromInteger(1);

            if (field_106_current_motion == eGlukkonMotions::M_KnockBack_3_442F40)
            {
                field_1D8_falling_velx_scale_factor = FP_FromDouble(0.67);
            }
        }
    }
    else
    {
        field_F8_LastLineYPos = field_BC_ypos;
        SetAnim_43F9C0(eGlukkonMotions::M_Fall_7_443510, TRUE);
    }
}

void Glukkon::GetOnPlatforms_444060()
{
    vOnCollisionWith_424EE0(
        {FP_GetExponent(field_B8_xpos - FP_FromInteger(5)), FP_GetExponent(field_BC_ypos - FP_FromInteger(5))},
        {FP_GetExponent(field_B8_xpos + FP_FromInteger(5)), FP_GetExponent(field_BC_ypos + FP_FromInteger(5))},
        ObjList_5C1B78,
        1,
        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
}

SfxDefinition stepSfx_554840[3] = {
    {0u, 8u, 36u, 25u, 1524, 1905},
    {0u, 3u, 59u, 60u, 0, 254},
    {0u, 3u, 72u, 120u, 0, 254}};

void Glukkon::PlaySound_4447D0(s32 sndIdx, Glukkon* pGlukkon)
{
    s32 volumeLeft, volumeRight;
    s32 defaultSndIdxVol = stepSfx_554840[sndIdx].field_3_default_volume;

    s16 pitch;
    if (sndIdx || !pGlukkon)
    {
        pitch = Math_RandomRange_496AB0(stepSfx_554840[sndIdx].field_4_pitch_min, stepSfx_554840[sndIdx].field_6_pitch_max);
    }
    else
    {
        s16 pitchCap = pGlukkon->field_212_currentWalkPitch;
        if (pitchCap > 12)
        {
            pitchCap = pitchCap % 4 + 12;
        }
        pitch = 127 * pitchCap;
    }

    if (pGlukkon->field_CC_sprite_scale == FP_FromInteger(1))
    {
        volumeRight = defaultSndIdxVol;
    }
    else // Glukkon in background layer? TODO figure out if this does actually happen
    {
        volumeRight = defaultSndIdxVol / 2;
    }

    CameraPos direction = gMap.GetDirection_4811A0(
        pGlukkon->field_C2_lvl_number,
        pGlukkon->field_C0_path_number,
        pGlukkon->field_B8_xpos,
        pGlukkon->field_BC_ypos);
    PSX_RECT worldRect;
    gMap.Get_Camera_World_Rect_481410(direction, &worldRect);
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
        {
            volumeLeft = volumeRight;
            break;
        }
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
            volumeRight = volumeLeft;
        }
        break;
        case CameraPos::eCamLeft_3:
        {
            FP percentHowFar = (FP_FromInteger(worldRect.w) - pGlukkon->field_B8_xpos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            break;
        }
        case CameraPos::eCamRight_4:
        {
            FP percentHowFar = (pGlukkon->field_B8_xpos - FP_FromInteger(worldRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            break;
        }
        default:
            return;
    }

    if (pGlukkon->field_CC_sprite_scale == FP_FromDouble(0.5)) //TODO figure out if this does actually happen
    {
        volumeLeft = FP_GetExponent(FP_FromInteger(volumeLeft * 2) / FP_FromInteger(3));
        volumeRight = FP_GetExponent(FP_FromInteger(volumeRight * 2) / FP_FromInteger(3));
    }

    SFX_SfxDefinition_Play_4CA700(&stepSfx_554840[sndIdx], (s16) volumeLeft, (s16) volumeRight, pitch, pitch);
}

void Glukkon::ToDead_43F640()
{
    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (sControlledCharacter_5C1B8C == this)
    {
        // When its a player controlled gluk go back to the screen the player is in
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);

        if (gMap.mLevel != LevelIds::eMenu_0)
        {
            gMap.SetActiveCam_480D30(
                field_1E4_level,
                field_1E6_path,
                field_1E8_camera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }

    SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetFalse_1);

    if ((field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryAslik_1 ||
        field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryDripik_2 ||
        field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryPhleg_3) &&
        !SwitchStates_Get_466020(field_1A8_tlvData.field_26_play_movie_switch_id))
    {
        // If an exec is dead trigger ze gas
        SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_24_death_switch_id, SwitchOp::eSetTrue_0);
    }

    if (field_1A8_tlvData.field_1C_spawn_switch_id == 0)
    {
        // Don't spawn again, dead
        mFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (field_1A8_tlvData.field_1C_spawn_switch_id > 1)
        {
            // If above 1 then its an actual switch id
            SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_1C_spawn_switch_id, SwitchOp::eSetFalse_1);
        }

        // Spawn again if id is 1 or above
        Init_43F260();
    }
}

void Glukkon::VOn_TLV_Collision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_10C_health = FP_FromInteger(0);
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);

                PlaySound_GameSpeak_444AF0(GlukkonSpeak::Unused_9, 0, 0, 0);
                ToDead_43F640();

                // Muds love it when people DIE
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
            }
        }

        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            pTlv,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
    }
}

SfxDefinition gameSpeak_554858[15] = {
    {0u, 8u, 66u, 127u, 0, 0},
    {0u, 8u, 64u, 127u, 0, 0},
    {0u, 8u, 67u, 127u, 0, 0},
    {0u, 8u, 61u, 127u, 0, 0},
    {0u, 8u, 63u, 127u, 0, 0},
    {0u, 8u, 70u, 127u, 0, 0},
    {0u, 8u, 65u, 127u, 0, 0},
    {0u, 8u, 69u, 127u, 0, 0},
    {0u, 8u, 71u, 127u, 0, 0},
    {0u, 8u, 60u, 127u, 0, 0},
    {0u, 8u, 60u, 127u, 0, 0},
    {0u, 8u, 62u, 127u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};

void Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak sndIdx, s16 volume, s16 pitch, Glukkon* pGlukkon)
{
    auto sndIdxShort = (s16) sndIdx;
    s16 calcedVolume = volume;
    if (!calcedVolume)
    {
        calcedVolume = gameSpeak_554858[sndIdxShort].field_3_default_volume;
    }
    if (pGlukkon)
    {
        if (pGlukkon->field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            calcedVolume = FP_GetExponent(FP_FromInteger(calcedVolume * 2) / FP_FromInteger(3));
        }
    }
    SFX_SfxDefinition_Play_4CA420(&gameSpeak_554858[sndIdxShort], calcedVolume, pitch, pitch);
}

Bool32 Glukkon::IsLineOfSightBetween_4403B0(Glukkon* pGlukkon, BaseAliveGameObject* pOther)
{
    FP hitX = {}, hitY = {};
    PathLine* pathLine = nullptr;

    PSX_RECT bRect = {};
    pOther->vGetBoundingRect_424FD0(&bRect, 1);
    const FP X2 = FP_FromInteger((bRect.x + bRect.w) / 2);
    const FP Y2 = FP_FromInteger((bRect.y + bRect.y) / 2);

    return sCollisions_DArray_5C1128->Raycast_417A60(
               pGlukkon->field_B8_xpos,
               pGlukkon->field_BC_ypos,
               X2,
               Y2,
               &pathLine,
               &hitX,
               &hitY,
               (pGlukkon->field_D6_scale != 0 ? 1 : 0x10) | (pGlukkon->field_D6_scale != 0 ? 6 : 0x60) | (pGlukkon->field_D6_scale != 0 ? 8 : 0x80))
        != 1;
}

void Glukkon::ToStand_443B70()
{
    field_1D8_falling_velx_scale_factor = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    SetAnim_43F9C0(eGlukkonMotions::M_Idle_0_442D10);
    MapFollowMe(TRUE);
}

void Glukkon::SlowDown_444700(FP speed)
{
    if (FP_GetExponent(field_C4_velx))
    {
        FollowLine_443EB0();
        if (field_C4_velx <= FP_FromInteger(0))
        {
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = (field_CC_sprite_scale * speed) + field_C4_velx;
                if (field_C4_velx > FP_FromInteger(0))
                {
                    field_C4_velx = FP_FromInteger(0);
                }
            }
        }
        else
        {
            field_C4_velx = field_C4_velx - (field_CC_sprite_scale * speed);
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = FP_FromInteger(0);
            }
        }
    }
}

void Glukkon::VScreenChanged()
{
    BaseGameObject::VScreenChanged();
    SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetFalse_1);
    if (BrainIs(&Glukkon::Brain_5_WaitToSpawn_442490) && !field_210_brain_sub_state)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

void Glukkon::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        field_F8_LastLineYPos = field_BC_ypos;
        pPlatform->VRemove(this);
        field_110_id = -1;
        SetAnim_43F9C0(eGlukkonMotions::M_WalkToFall_6_4434E0, TRUE);
    }
}

s16 Glukkon::VTakeDamage(BaseGameObject* pFrom)
{
    if (BrainIs(&Glukkon::Brain_4_Death_442010))
    {
        return 1;
    }

    switch (pFrom->Type())
    {
        case AETypes::eBullet_15:
        {
            auto pBullet = static_cast<Bullet*>(pFrom);
            switch (pBullet->field_20_type)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                {
                    {
                        const FP yRand = (FP_FromInteger(Math_NextRandom() % 16)) - FP_FromInteger(8);
                        const FP xRand = FP_FromInteger(Math_NextRandom() & 0xF); // TODO: Might be wrong as was trying to make this abs() but result is unsigned anyway ??

                        const FP xPos = (field_CC_sprite_scale * (pBullet->field_30_x_distance <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6)));
                        ae_new<Blood>(
                            xPos + field_B8_xpos,
                            field_BC_ypos - (FP_FromInteger(25) * field_CC_sprite_scale),
                            ((pBullet->field_30_x_distance <= FP_FromInteger(0) ? -FP_FromInteger(1) : FP_FromInteger(1)) * xRand + FP_FromInteger(16)),
                            yRand,
                            field_CC_sprite_scale,
                            12);
                    }

                    {
                        const FP xPos = (field_CC_sprite_scale * (pBullet->field_30_x_distance <= FP_FromInteger(0) ? -FP_FromInteger(12) : FP_FromInteger(12)));
                        ae_new<Blood>(
                            xPos + field_B8_xpos,
                            field_BC_ypos - (FP_FromInteger(25) * field_CC_sprite_scale),
                            pBullet->field_30_x_distance <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6),
                            FP_FromInteger(0),
                            field_CC_sprite_scale,
                            8);
                    }
                }
                break;

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    ae_new<Blood>(
                        field_B8_xpos,
                        field_BC_ypos - (FP_FromInteger(25) * field_CC_sprite_scale),
                        FP_FromInteger(0),
                        FP_FromInteger(0),
                        field_CC_sprite_scale,
                        25);
                }
                break;

                default:
                    break;
            }

            field_200_knockback_delay_after_getting_shot_timer = sGnFrame_5C1B84 + 5;

            if (field_106_current_motion == eGlukkonMotions::M_GetShot_21_443A60)
            {
                field_10C_health = FP_FromInteger(0);
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
                return 1;
            }

            if (field_106_current_motion == eGlukkonMotions::M_KnockBack_3_442F40)
            {
                if (field_10C_health > FP_FromInteger(0))
                {
                    SetBrain(&Glukkon::Brain_4_Death_442010);
                    field_210_brain_sub_state = 4;
                }
                field_10C_health = FP_FromInteger(0);
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
                return 1;
            }

            field_204_getting_shot_timer = sGnFrame_5C1B84 + 20;
            SetAnim_43F9C0(eGlukkonMotions::M_GetShot_21_443A60, TRUE);

            SetBrain(&Glukkon::Brain_4_Death_442010);
            field_210_brain_sub_state = 4;

            if (pBullet->field_30_x_distance >= FP_FromInteger(0))
            {
                field_C4_velx = FP_FromDouble(0.001);
            }
            else
            {
                field_C4_velx = -FP_FromDouble(0.001);
            }
            field_10C_health = FP_FromInteger(0);
            Event_Broadcast_422BC0(kEventMudokonComfort, this);
        }
        break;

        case AETypes::eDrill_30:
        case AETypes::eBaseBomb_46:
        case AETypes::eMeatSaw_86:
        case AETypes::eExplosion_109:
            SetBrain(&Glukkon::Brain_4_Death_442010);
            field_210_brain_sub_state = 2;
            Event_Broadcast_422BC0(kEventMudokonComfort, this);
            break;

        case AETypes::eElectricWall_39:
            Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Unused_9, 0, field_1E0_gamespeak_pitch, this);
            return 1;

        case AETypes::eRockSpawner_48:
        case AETypes::eMineCar_89:
        case AETypes::eNeverSet_107:
            SetAnim_43F9C0(eGlukkonMotions::M_DeathFall_8_443760, TRUE);
            SetBrain(&Glukkon::Brain_4_Death_442010);
            field_210_brain_sub_state = 0;
            Event_Broadcast_422BC0(kEventMudokonComfort, this);
            break;

        case AETypes::eAbe_69:
            if (sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_62_Punch_454750)
            {
                if (Math_NextRandom() <= 32u)
                {
                    Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Unused_9, 0, 0, 0);
                }
                else
                {
                    Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Hey_0, 0, 0, 0);
                }
                field_1F8_panic_timer = sGnFrame_5C1B84;
                SetAnim_43F9C0(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
                SetBrain(&Glukkon::Brain_2_Slapped_441720);
                field_210_brain_sub_state = 0;
            }
            break;

        case AETypes::eSlog_126:
            if (field_106_current_motion != eGlukkonMotions::M_KnockBack_3_442F40)
            {
                field_10C_health = FP_FromInteger(0);
                SetBrain(&Glukkon::Brain_4_Death_442010);
                field_210_brain_sub_state = 5;
                Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
                if (!vIsFacingMe_4254A0(static_cast<BaseAnimatedWithPhysicsGameObject*>(pFrom)))
                {
                    field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
                }
                field_C4_velx = FP_FromInteger(0);
                SetAnim_43F9C0(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
            }
            break;

        case AETypes::eElectrocute_150:
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_10C_health = FP_FromInteger(0);
            SetBrain(&Glukkon::Brain_4_Death_442010);
            field_210_brain_sub_state = 3;
            field_1D4_timer = sGnFrame_5C1B84 + 1;
            Event_Broadcast_422BC0(kEventMudokonComfort, this);
            break;

        default:
            return 1;
    }

    return 1;
}
