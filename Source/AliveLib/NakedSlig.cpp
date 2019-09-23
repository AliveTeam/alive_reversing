#include "stdafx.h"
#include "NakedSlig.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "MusicController.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"
#include "SwitchStates.hpp"
#include "SnoozeParticle.hpp"
#include "Input.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "ScreenShake.hpp"
#include "Midi.hpp"
#include "Slig.hpp"
#include "FlyingSlig.hpp"
#include "NakedSligButton.hpp"
#include "Sfx.hpp"

TintEntry stru_5514B8[18] =
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
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};

const int sNakedSligFrameTableOffsets_551470[18] =
{
    53684,
    53852,
    53964,
    53560,
    53720,
    53736,
    53752,
    53780,
    53816,
    53928,
    54032,
    53616,
    54000,
    53780,
    54096,
    54076,
    54160,
    54188
};

ALIVE_ARY(1, 0x551428, TNakedSligMotionFn, 18, sNakedSlig_motions_551428,
{
    &NakedSlig::M_Idle_0_41B260,
    &NakedSlig::M_UsingButton_1_41B890,
    &NakedSlig::M_WakingUp_2_41BF00,
    &NakedSlig::M_Crawling_3_41B280,
    &NakedSlig::M_StartFalling_4_41B620,
    &NakedSlig::M_Falling_5_41B650,
    &NakedSlig::M_Landing_6_41B870,
    &NakedSlig::M_ToShakingToIdle_7_41C010,
    &NakedSlig::M_Speaking_8_41BF70,
    &NakedSlig::M_Snoozing_9_41BD80,
    &NakedSlig::M_PushingWall_10_41B400,
    &NakedSlig::M_TurnAround_11_41B590,
    &NakedSlig::M_Shaking_12_418C30,
    &NakedSlig::M_Empty_13_418C50,
    &NakedSlig::M_ShakingToIdle_14_41C040,
    &NakedSlig::M_EndCrawling_15_41B600,
    &NakedSlig::M_IdleToPushingWall_16_41B3C0,
    &NakedSlig::M_EndPushingWall_17_41B3A0
});


const static AIFunctionData<TNakedSligAIFn> sNakedSligAITable[] =
{
    { &NakedSlig::AI_0_Sleeping_419DE0, 0x401D1B, "AI_0_Sleeping" },
    { &NakedSlig::AI_1_Idle_419F60, 0x40340E, "AI_1_Idle" },
    { &NakedSlig::AI_2_PanicGetALocker_419FE0, 0x419FE0, "AI_2_PanicGetALocker" },
    { &NakedSlig::AI_3_Possesed_41A5B0, 0x404539, "AI_3_Possesed" },
    { &NakedSlig::AI_4_GetKilled_41A880, 0x403265, "AI_4_GetKilled" },
    { &NakedSlig::AI_5_Transformed_41ADF0, 0x40484A, "AI_5_Transformed" },
};

NakedSlig* NakedSlig::ctor_418C70(Path_NakedSlig* pTlv, int tlvInfo)
{
    ctor_408240(2);

    field_1DC = -1;
    SetVTable(this, 0x5446A8);
    field_4_typeId = Types::eCrawlingSlig_26;

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 449, 1, 0));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 423, 1, 0));
    Animation_Init_424E10(53684, 109, 37, field_10_resources_array.ItemAt(0), 1, 1);

    SetTint_425600(&stru_5514B8[0], gMap_5C3030.sCurrentLevelId_5C3030);

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);

    field_1CC = 0;
    field_1AC_timer = 0;
    field_1C8 = 0;
    field_1B8_bChanting = 0;
    field_1C4 = 0;
    field_1C0_speak = SligSpeak::None;
    field_1D0_slig_button_id = -1;
    field_1D4_obj_id = -1;
    field_1D8_obj_id = -1;
    field_1E4_pPantsOrWingsTlv = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    field_118_tlvInfo = tlvInfo;
    field_1E8_tlv = *pTlv;

    if (field_1E8_tlv.field_10_scale == Path_NakedSlig::Scale::Half_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }
    else  if (field_1E8_tlv.field_10_scale == Path_NakedSlig::Scale::Full_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (field_1E8_tlv.field_14_state == Path_NakedSlig::State::State_2)
    {
        Set_AnimAndMotion_419890(NakedSligMotion::M_Idle_0_41B260, TRUE);
        SetBrain(&NakedSlig::AI_1_Idle_419F60);
    }
    else
    {
        if (field_CC_sprite_scale == FP_FromInteger(1))
        {
            field_20_animation.field_C_render_layer = 25;
        }
        else
        {
            field_20_animation.field_C_render_layer = 6;
        }
        Set_AnimAndMotion_419890(NakedSligMotion::M_Snoozing_9_41BD80, TRUE);
        SetBrain(&NakedSlig::AI_0_Sleeping_419DE0);
    }

    field_208_brain_sub_state = 0;
    if (field_1E8_tlv.field_16_locker_direction == Path_NakedSlig::LockerDirection::Search_2)
    {
        field_1E0_locker_direction = NextRandom_4197D0() ? Path_NakedSlig::LockerDirection::Right_1 : Path_NakedSlig::LockerDirection::Left_0;
    }
    else
    {
        field_1E0_locker_direction = field_1E8_tlv.field_16_locker_direction;
    }

    field_1C6 = field_1E8_tlv.field_18_panic_id;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(37),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 0x10) == 1)
    {
        field_BC_ypos = hitY;
    }

    return this;
}

BaseGameObject* NakedSlig::VDestructor(signed int flags)
{
    return vdtor_418FB0(flags);
}

void NakedSlig::VUpdate()
{
    vUpdate_419100();
}

void NakedSlig::VRender(int** pOrderingTable)
{
    vRender_419990(pOrderingTable);
}

void NakedSlig::vRender_419990(int** ot)
{
    const __int16 eyeIndices[] = { 60, 62 };
    renderWithGlowingEyes(ot, this, &field_11C_pPalAlloc[0], ALIVE_COUNTOF(field_11C_pPalAlloc),
                        &field_19C_pal_rect, field_1A4_r, field_1A6_g, field_1A8_b, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

void NakedSlig::VPossessed_408F70()
{
    vPossessed_4195F0();
}

void NakedSlig::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_41C580();
}

void NakedSlig::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    vOn_TLV_Collision_419680(pTlv);
}

__int16 NakedSlig::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_4192B0(pFrom);
}

int NakedSlig::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_41C9A0(reinterpret_cast<NakedSlig_State*>(pSaveBuffer));
}

int CC NakedSlig::CreateFromSaveState_41AE80(const BYTE* pBuffer)
{
    auto pState = reinterpret_cast<const NakedSlig_State*>(pBuffer);

    auto pTlv = static_cast<Path_NakedSlig*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_44_tlvInfo));
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 449, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("CRAWLSLG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kSligBlowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBLOW.BAN", nullptr);
    }

    auto pNakedSlig = alive_new<NakedSlig>();
    pNakedSlig->ctor_418C70(pTlv, pState->field_44_tlvInfo);

    pNakedSlig->field_C_objectId = pState->field_4_obj_id;

    if (pState->field_40_bIsControlled)
    {
        sControlledCharacter_5C1B8C = pNakedSlig;
    }

    pNakedSlig->field_FC_pPathTLV = nullptr;
    pNakedSlig->field_100_pCollisionLine = nullptr;

    pNakedSlig->field_B8_xpos = pState->field_8_xpos;
    pNakedSlig->field_BC_ypos = pState->field_C_ypos;
    pNakedSlig->field_C4_velx = pState->field_10_velx;
    pNakedSlig->field_C8_vely = pState->field_14_vely;

    pNakedSlig->field_1B0 = pState->field_58;

    pNakedSlig->field_C0_path_number = pState->field_18_path_number;
    pNakedSlig->field_C2_lvl_number = pState->field_1A_lvl_number;
    pNakedSlig->field_CC_sprite_scale = pState->field_1C_sprite_scale;

    pNakedSlig->field_1A4_r = pState->field_20_r;
    pNakedSlig->field_D0_r = pState->field_20_r;

    pNakedSlig->field_1A6_g = pState->field_22_g;
    pNakedSlig->field_D2_g = pState->field_22_g;

    pNakedSlig->field_1A8_b = pState->field_24_b;
    pNakedSlig->field_D4_b = pState->field_24_b;

    pNakedSlig->field_106_current_motion = pState->field_28_current_motion;
    pNakedSlig->field_20_animation.Set_Animation_Data_409C80(sNakedSligFrameTableOffsets_551470[pState->field_28_current_motion], nullptr);
    pNakedSlig->field_20_animation.field_92_current_frame = pState->field_2A_anim_cur_frame;

    pNakedSlig->field_20_animation.field_E_frame_change_counter = pState->field_2C_anim_frame_change_counter;

    pNakedSlig->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_26_bFlipX & 1);
    pNakedSlig->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2E_bRender & 1);

    pNakedSlig->field_6_flags.Set(BaseGameObject::eDrawable, pState->field_2F_bDrawable & 1);

    if (IsLastFrame(&pNakedSlig->field_20_animation))
    {
        pNakedSlig->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pNakedSlig->field_10C_health = pState->field_30_health;
    pNakedSlig->field_106_current_motion = pState->field_34_cur_motion;
    pNakedSlig->field_108_next_motion = pState->field_36_next_motion;
    pNakedSlig->field_F8_LastLineYPos = FP_FromInteger(pState->field_38_unknown);
    pNakedSlig->field_114_flags.Set(Flags_114::e114_Bit9);
    pNakedSlig->field_1AC_timer = pState->field_54_timer;
    pNakedSlig->field_104_collision_line_type = pState->field_3A_line_type;
    pNakedSlig->field_118_tlvInfo = pState->field_44_tlvInfo;
    pNakedSlig->SetBrain(sNakedSligAITable[pState->field_48_ai_idx].mOurFn);
    pNakedSlig->field_208_brain_sub_state = pState->field_50_brain_sub_state;
    pNakedSlig->field_1B8_bChanting = pState->field_5E_bChanting;
    pNakedSlig->field_1BA_prev_level = pState->field_60_prev_leve;
    pNakedSlig->field_1BC_prev_path = pState->field_62_prev_path;
    pNakedSlig->field_1BE_prev_camera = pState->field_64_prev_camera;
    pNakedSlig->field_1B4 = pState->field_68;
    pNakedSlig->field_1D0_slig_button_id = pState->field_6C_slig_button_id;
    pNakedSlig->field_1D4_obj_id = pState->field_70_obj_id;
    pNakedSlig->field_1D8_obj_id = pState->field_74_obj_id;
    pNakedSlig->field_1C0_speak = pState->field_78_speak;
    pNakedSlig->field_1C2_pitch = pState->field_66_pitch;
    pNakedSlig->field_1C4 = pState->field_7A;
    pNakedSlig->field_1C8 = pState->field_7C;

    return sizeof(NakedSlig_State);
}

int NakedSlig::vGetSaveState_41C9A0(NakedSlig_State* pState)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    pState->field_0_type = Types::eCrawlingSlig_26;
    pState->field_4_obj_id = field_C_objectId;

    pState->field_8_xpos = field_B8_xpos;
    pState->field_C_ypos = field_BC_ypos;
    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_58 = field_1B0;

    pState->field_18_path_number = field_C0_path_number;
    pState->field_1A_lvl_number = field_C2_lvl_number;
    pState->field_1C_sprite_scale = field_CC_sprite_scale;

    pState->field_20_r = field_D0_r;
    pState->field_22_g = field_D2_g;
    pState->field_24_b = field_D4_b;
    pState->field_26_bFlipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_28_current_motion = field_106_current_motion;
    pState->field_2A_anim_cur_frame = field_20_animation.field_92_current_frame;
    pState->field_2C_anim_frame_change_counter = field_20_animation.field_E_frame_change_counter;
    pState->field_2F_bDrawable = field_6_flags.Get(BaseGameObject::eDrawable);
    pState->field_2E_bRender = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pState->field_30_health = field_10C_health;
    pState->field_34_cur_motion = field_106_current_motion;
    pState->field_36_next_motion = field_108_next_motion;
    pState->field_3A_line_type = -1;

    // TODO: Check correct
    pState->field_38_unknown = FP_GetExponent(field_F8_LastLineYPos);
  
    if (field_100_pCollisionLine)
    {
        pState->field_3A_line_type = field_100_pCollisionLine->field_8_type;
    }

    pState->field_40_bIsControlled = (this == sControlledCharacter_5C1B8C);
    pState->field_54_timer = field_1AC_timer;
    pState->field_44_tlvInfo = field_118_tlvInfo;
    pState->field_48_ai_idx = 0;

    int idx = 0;
    for (const auto& fn : sNakedSligAITable)
    {
        if (BrainIs(fn.mOurFn))
        {
            pState->field_48_ai_idx = idx;
            break;
        }
        idx++;
    }

    pState->field_50_brain_sub_state = field_208_brain_sub_state;
    pState->field_5E_bChanting = field_1B8_bChanting;
    pState->field_60_prev_leve = field_1BA_prev_level;
    pState->field_62_prev_path = field_1BC_prev_path;
    pState->field_64_prev_camera = field_1BE_prev_camera;
    pState->field_66_pitch = field_1C2_pitch;
    pState->field_68 = field_1B4;
    pState->field_6C_slig_button_id = field_1D0_slig_button_id;
    pState->field_70_obj_id = field_1D4_obj_id;
    pState->field_74_obj_id = field_1D8_obj_id;
    pState->field_78_speak = field_1C0_speak;
    pState->field_7A = field_1C4;
    pState->field_7C = field_1C8;
    return sizeof(NakedSlig_State);
}

void NakedSlig::vPossessed_4195F0()
{
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_1B8_bChanting = TRUE;
    Set_AnimAndMotion_419890(NakedSligMotion::M_Shaking_12_418C30, TRUE);
    SetBrain(&NakedSlig::AI_3_Possesed_41A5B0);
    field_208_brain_sub_state = 0;
    field_1AC_timer = sGnFrame_5C1B84 + 35;
    field_1BA_prev_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_1BC_prev_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_1BE_prev_camera = gMap_5C3030.sCurrentCamId_5C3034;
}

void NakedSlig::Set_AnimAndMotion_419890(__int16 currentMotion, __int16 bClearNextMotion)
{
    field_20_animation.Set_Animation_Data_409C80(sNakedSligFrameTableOffsets_551470[currentMotion], nullptr);
    field_106_current_motion = currentMotion;

    UpdateAnimBlock_419900();

    if (bClearNextMotion)
    {
        field_108_next_motion = -1;
    }
}

void NakedSlig::UpdateAnimBlock_419900()
{
    BYTE** ppRes = GetAnimBlock_419950(field_106_current_motion);
    if (!ppRes)
    {
        field_106_current_motion = 0;
        ppRes = GetAnimBlock_419950(0);
    }
    field_20_animation.Set_Animation_Data_409C80(sNakedSligFrameTableOffsets_551470[field_106_current_motion], ppRes);
}

BYTE** NakedSlig::GetAnimBlock_419950(int /*currentMotion*/)
{
    if (field_1CC)
    {
        field_1CC = 0;
    }
    return field_10_resources_array.ItemAt(0);
}

__int16 CC NakedSlig::NextRandom_4197D0()
{
    return (Math_NextRandom() & 1) == 0;
}

void NakedSlig::vUpdate_419100()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (field_114_flags.Get(Flags_114::e114_Bit9))
        {
            field_114_flags.Clear(Flags_114::e114_Bit9);
            if (field_104_collision_line_type == -1)
            {
                field_100_pCollisionLine = 0;
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
            field_1D4_obj_id = Find_Flags_4DC170(field_1D4_obj_id);
            field_1D8_obj_id = Find_Flags_4DC170(field_1D8_obj_id);
            field_1D0_slig_button_id = Find_Flags_4DC170(field_1D0_slig_button_id);
        }

        if (!Input_IsChanting_45F260())
        {
            field_1B8_bChanting = 0;
        }

        const auto oldMotion = field_106_current_motion;

        field_208_brain_sub_state = (this->*field_204_brain_state)();
        
        const FP oldX = field_B8_xpos;
        const FP oldY = field_BC_ypos;

        (this->*sNakedSlig_motions_551428[field_106_current_motion])();

        if (oldMotion != field_106_current_motion)
        {
            LOG_INFO("NakedSlig: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
        }

        if (oldX != field_B8_xpos || oldY != field_BC_ypos)
        {
            auto pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);

            VOn_TLV_Collision_4087F0(pTlv);
        }
    }
}

__int16 NakedSlig::HandleEnemyStopper_41C740(FP /*velX*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

Path_TLV* NakedSlig::FindPantsOrWings_419750()
{
    Path_TLV* pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(nullptr, field_B8_xpos, field_BC_ypos, field_B8_xpos, field_BC_ypos);
    while (pTlvIter)
    {
        if (pTlvIter->field_4_type == TlvTypes::SligGetPants_104 || pTlvIter->field_4_type == TlvTypes::SligGetWings_105)
        {
            return pTlvIter;
        }
        pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlvIter, field_B8_xpos, field_BC_ypos, field_B8_xpos, field_BC_ypos);
    }
    return nullptr;
}

BaseGameObject* NakedSlig::FindSligButton_419840()
{
    return FindObjectOfType_425180(Types::eSligButton_16, field_B8_xpos, field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale));

}

void NakedSlig::vOnTrapDoorOpen_41C580()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        Set_AnimAndMotion_419890(NakedSligMotion::M_StartFalling_4_41B620, TRUE);
    }
}

void NakedSlig::vOn_TLV_Collision_419680(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_10C_health = FP_FromInteger(0);
                SetBrain(&NakedSlig::AI_4_GetKilled_41A880);
                field_208_brain_sub_state = 5;
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
                Sfx_Slig_GameSpeak_4C04F0(SligSpeak::Help_10, 0, field_1C2_pitch, this);
                field_1AC_timer = sGnFrame_5C1B84 + 60;
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

__int16 NakedSlig::vTakeDamage_4192B0(BaseGameObject* pFrom)
{
    if (!BrainIs(&NakedSlig::AI_5_Transformed_41ADF0))
    {
        switch (pFrom->field_4_typeId)
        {
        case Types::eBullet_15:
        case Types::eGrinder_30:
        case Types::eBaseBomb_46:
        case Types::eRockSpawner_48:
        case Types::eType_86:
        case Types::eMineCar_89:
        case Types::eType_107:
        case Types::eExplosion_109:
            if (!BrainIs(&NakedSlig::AI_4_GetKilled_41A880))
            {
                SetBrain(&NakedSlig::AI_4_GetKilled_41A880);
                field_208_brain_sub_state = 2;
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
            }
            return 1;

        case Types::eElectricWall_39:
            Sfx_Slig_GameSpeak_4C04F0(SligSpeak::Help_10, 0, 0, this);
            return 1;

        case Types::eSlig_125:
        {
            // Take a BEATING
            field_1AC_timer = sGnFrame_5C1B84 + 20;
            field_10C_health -= FP_FromDouble(0.13);

            if (field_10C_health <= FP_FromInteger(0))
            {
                SetBrain(&NakedSlig::AI_4_GetKilled_41A880);
                field_208_brain_sub_state = 2;
                return 0;
            }

            Set_AnimAndMotion_419890(eSligMotions::M_Falling_7_4B42D0, TRUE);
            Sfx_Slig_GameSpeak_4C04F0(SligSpeak::Help_10, 0, 0, this);

            if (BrainIs(&NakedSlig::AI_2_PanicGetALocker_419FE0))
            {
                field_208_brain_sub_state = 10;
            }
            else if (BrainIs(&NakedSlig::AI_3_Possesed_41A5B0))
            {
                field_208_brain_sub_state = 3;
            }
            else
            {
                SetBrain(&NakedSlig::AI_2_PanicGetALocker_419FE0);
                field_208_brain_sub_state = 10;
            }
        }
            return 1;

        case Types::eSlog_126:
            if (!BrainIs(&NakedSlig::AI_4_GetKilled_41A880))
            {
                SetBrain(&NakedSlig::AI_4_GetKilled_41A880);
                field_208_brain_sub_state = 4;
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                field_10C_health = FP_FromInteger(0);
                MapFollowMe_408D10(TRUE);
                field_1AC_timer = sGnFrame_5C1B84 + 15;
                Set_AnimAndMotion_419890(NakedSligMotion::M_Empty_13_418C50, TRUE);
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
            }
            return 1;

        case Types::eElectrocute_150:
            if (!BrainIs(&NakedSlig::AI_4_GetKilled_41A880))
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                field_10C_health = FP_FromInteger(0);
                field_1AC_timer = sGnFrame_5C1B84 + 1;
                SetBrain(&NakedSlig::AI_4_GetKilled_41A880);
                field_208_brain_sub_state = 3;
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
            }
            return 1;

        default:
            return 1;
        }
    }
    return 1;
}

void NakedSlig::SetBrain(TNakedSligAIFn fn)
{
    ::SetBrain(fn, field_204_brain_state, sNakedSligAITable);
}

bool NakedSlig::BrainIs(TNakedSligAIFn fn)
{
    return ::BrainIs(fn, field_204_brain_state, sNakedSligAITable);
}

void NakedSlig::dtor_418FE0()
{
    SetVTable(this, 0x5446A8);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        MusicController::sub_47FD60(0, this, 0, 0);
        if (gMap_5C3030.field_A_5C303A_levelId != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_1BA_prev_level,
                field_1BC_prev_path,
                field_1BE_prev_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }
    if (field_10C_health > FP_FromInteger(0) || field_1E8_tlv.field_1A_reset_on_death == Path_NakedSlig::ResetOnDeath::Yes_1)
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 1);
    }

    dtor_4080B0();
}

NakedSlig* NakedSlig::vdtor_418FB0(signed int flags)
{
    dtor_418FE0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

BOOL NakedSlig::PanicOn_419810()
{
    return SwitchStates_Get_466020(field_1E8_tlv.field_18_panic_id) != 0;
}

void NakedSlig::ToIdle_41C070()
{
    field_1B0 = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    Set_AnimAndMotion_419890(NakedSligMotion::M_Idle_0_41B260, FALSE);
    MapFollowMe_408D10(TRUE);
}

__int16 NakedSlig::AI_0_Sleeping_419DE0()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(0, this, 0, 0);
    }
    
    if (field_208_brain_sub_state == 0)
    {
        if (!(static_cast<int>(sGnFrame_5C1B84) % 16))
        {
            if (field_1C4 > 0)
            {
                field_1C4--;
            }
        }

        if (PanicOn_419810())
        {
            field_1AC_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(15, 45);
            return 1;
        }
        else
        {
            return field_208_brain_sub_state;
        }
    }

    if (field_208_brain_sub_state == 1)
    {
        if (static_cast<int>(sGnFrame_5C1B84) <= field_1AC_timer)
        {
            return field_208_brain_sub_state;
        }

        field_108_next_motion = NakedSligMotion::M_WakingUp_2_41BF00;
        return 2;
    }

    if (field_208_brain_sub_state != 2 || field_106_current_motion != NakedSligMotion::M_WakingUp_2_41BF00 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return field_208_brain_sub_state;
    }

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 27;
    }
    else
    {
        field_20_animation.field_C_render_layer = 8;
    }

    ToIdle_41C070();
    SetBrain(&NakedSlig::AI_2_PanicGetALocker_419FE0);
    return 0;
}

__int16 NakedSlig::AI_1_Idle_419F60()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(4, this, 0, 0);
    }

    if (PanicOn_419810())
    {
        SetBrain(&NakedSlig::AI_2_PanicGetALocker_419FE0);
    }
    return 0;
}

__int16 NakedSlig::AI_2_PanicGetALocker_419FE0()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(8, this, 0, 0);
    }

    if (!field_100_pCollisionLine && field_208_brain_sub_state != 1)
    {
        field_208_brain_sub_state = 1;
    }

    switch (field_208_brain_sub_state)
    {
    case 0:
        if ((field_1E0_locker_direction != Path_NakedSlig::LockerDirection::Right_1 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))) && 
            (field_1E0_locker_direction != Path_NakedSlig::LockerDirection::Left_0  ||   field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
        {
            field_108_next_motion = NakedSligMotion::M_Crawling_3_41B280;
            return 2;
        }
        field_108_next_motion = NakedSligMotion::M_TurnAround_11_41B590;
        return 3;

    case 1:
        if (field_106_current_motion != NakedSligMotion::M_Idle_0_41B260)
        {
            return field_208_brain_sub_state;
        }
        return 0;

    case 2:
        if (HandleEnemyStopper_41C740(field_C4_velx))
        {
            field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame_5C1B84 + 30;
            field_108_next_motion = NakedSligMotion::M_TurnAround_11_41B590;
            return 7;
        }

        field_1E4_pPantsOrWingsTlv = FindPantsOrWings_419750();
        if (field_1E4_pPantsOrWingsTlv)
        {
            ToIdle_41C070();
            return 5;
        }
        else
        {
            if (Math_NextRandom() >= 10u || static_cast<int>(sGnFrame_5C1B84) <= field_1C8)
            {
                return field_208_brain_sub_state;
            }
            field_108_next_motion = NakedSligMotion::M_Speaking_8_41BF70;
            field_1C8 = sGnFrame_5C1B84 + 60;
            field_1C0_speak = SligSpeak::Help_10;
            return 4;
        }
        break;

    case 3:
        if (field_106_current_motion != NakedSligMotion::M_TurnAround_11_41B590 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_208_brain_sub_state;
        }
        field_108_next_motion = NakedSligMotion::M_Crawling_3_41B280;
        return 2;

    case 4:
    {
        if (field_106_current_motion != NakedSligMotion::M_Speaking_8_41BF70 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_208_brain_sub_state;
        }

        FP gridScale = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridScale = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            gridScale = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        if (!WallHit_408750(FP_FromInteger(35), gridScale))
        {
            field_108_next_motion = NakedSligMotion::M_Crawling_3_41B280;
            return 2;
        }
        field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame_5C1B84 + 30;
        field_108_next_motion = NakedSligMotion::M_TurnAround_11_41B590;
        return 7;
    }

    case 5:
        if (!field_1E4_pPantsOrWingsTlv)
        {
            field_1E4_pPantsOrWingsTlv = FindPantsOrWings_419750();
        }

        if (field_106_current_motion != NakedSligMotion::M_Idle_0_41B260 || (field_1E4_pPantsOrWingsTlv && field_1E4_pPantsOrWingsTlv->field_1_unknown))
        {
            if (Math_NextRandom() & 1)
            {
                field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame_5C1B84 + 30;
                return 8;
            }
            else
            {
                field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame_5C1B84 + 30;
                field_108_next_motion = NakedSligMotion::M_TurnAround_11_41B590;
                return 7;
            }
        }
        else
        {
            field_1E4_pPantsOrWingsTlv->field_1_unknown &= 0xFF;
            field_1E4_pPantsOrWingsTlv->field_1_unknown |= 1;

            field_108_next_motion = NakedSligMotion::M_UsingButton_1_41B890;
            field_1AC_timer = sGnFrame_5C1B84 + 20;
            return 6;
        }
        break;

    case 6:
        if (field_1E4_pPantsOrWingsTlv)
        {
            return field_208_brain_sub_state;
        }
        field_1E4_pPantsOrWingsTlv = FindPantsOrWings_419750();
        return field_208_brain_sub_state;

    case 7:
        if (field_106_current_motion == NakedSligMotion::M_TurnAround_11_41B590 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_108_next_motion = NakedSligMotion::M_Crawling_3_41B280;
            return 8;
        }
        return field_208_brain_sub_state;

    case 8:
        if (HandleEnemyStopper_41C740(field_C4_velx))
        {
            field_108_next_motion = NakedSligMotion::M_TurnAround_11_41B590;
            return 3;
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_1AC_timer)
        {
            field_108_next_motion = 11;
            return 3;
        }

        if (Math_NextRandom() >= 10u || static_cast<int>(sGnFrame_5C1B84) <= field_1C8)
        {
            return field_208_brain_sub_state;
        }

        field_108_next_motion = NakedSligMotion::M_Speaking_8_41BF70;
        field_1C8 = sGnFrame_5C1B84 + 60;
        field_1C0_speak = SligSpeak::Help_10;
        return 9;

    case 9:
    {
        if (field_106_current_motion != NakedSligMotion::M_Speaking_8_41BF70 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_208_brain_sub_state;
        }

        FP gridScale = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridScale = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            gridScale = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        if (WallHit_408750(FP_FromInteger(35), gridScale))
        {
            field_108_next_motion = NakedSligMotion::M_TurnAround_11_41B590;
            return 3;
        }
        else
        {
            field_108_next_motion = NakedSligMotion::M_Crawling_3_41B280;
            return 8;
        }
    }
        break;

    case 10:
        if (field_106_current_motion == NakedSligMotion::M_ShakingToIdle_14_41C040)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                Set_AnimAndMotion_419890(NakedSligMotion::M_Idle_0_41B260, TRUE);
                field_208_brain_sub_state = 0;
                return field_208_brain_sub_state;
            }
        }
        else if (static_cast<int>(sGnFrame_5C1B84) > field_1AC_timer)
        {
            Set_AnimAndMotion_419890(NakedSligMotion::M_ShakingToIdle_14_41C040, TRUE);
        }
        return field_208_brain_sub_state;

    default:
        return field_208_brain_sub_state;
    }
}

__int16 NakedSlig::AI_3_Possesed_41A5B0()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(9, this, 0, 0);
    }

    switch (field_208_brain_sub_state)
    {
    case 0:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_1AC_timer)
        {
            return field_208_brain_sub_state;
        }
        Set_AnimAndMotion_419890(NakedSligMotion::M_ShakingToIdle_14_41C040, TRUE);
        return 1;

    case 1:
        if (!Input_IsChanting_45F260() || field_1B8_bChanting)
        {
            return field_208_brain_sub_state;
        }
        field_1AC_timer = sGnFrame_5C1B84 + 30;
        SFX_Play_46FA90(17, 0);
        Set_AnimAndMotion_419890(NakedSligMotion::M_Shaking_12_418C30, TRUE);
        return 2;

    case 2:
        if (Input_IsChanting_45F260())
        {
            if (!(static_cast<int>(sGnFrame_5C1B84) % 4))
            {
                New_Chant_Particle_426BE0(
                    (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange_496AB0(-20, 20))) + field_B8_xpos,
                    field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange_496AB0(0, 30))),
                    field_CC_sprite_scale,
                    0);
            }

            if (static_cast<int>(sGnFrame_5C1B84) <= field_1AC_timer && sActiveHero_5C1B68->field_10C_health > FP_FromInteger(0))
            {
                return field_208_brain_sub_state;
            }

            sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
            field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
            gMap_5C3030.SetActiveCam_480D30(field_1BA_prev_level, field_1BC_prev_path, field_1BE_prev_camera, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            SetBrain(&NakedSlig::AI_4_GetKilled_41A880);
            MusicController::sub_47FD60(0, this, 0, 0);
            return field_208_brain_sub_state;
        }
        else
        {
            Set_AnimAndMotion_419890(NakedSligMotion::M_ShakingToIdle_14_41C040, TRUE);
            return 1;
        }
        break;

    case 3:
        if (field_106_current_motion != NakedSligMotion::M_ShakingToIdle_14_41C040)
        {
            if (static_cast<int>(sGnFrame_5C1B84) > field_1AC_timer)
            {
                Set_AnimAndMotion_419890(NakedSligMotion::M_ShakingToIdle_14_41C040, TRUE);
            }
            return field_208_brain_sub_state;
        }

        if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_208_brain_sub_state;
        }

        Set_AnimAndMotion_419890(0, 1);
        field_208_brain_sub_state = 1;
        return field_208_brain_sub_state;

    default:
        break;
    }
    return field_208_brain_sub_state;
}

__int16 NakedSlig::AI_4_GetKilled_41A880()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(0, this, 0, 0);
    }

    switch (field_208_brain_sub_state)
    {
    case 0:
        if (field_106_current_motion != NakedSligMotion::M_ToShakingToIdle_7_41C010 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_208_brain_sub_state;
        }
        return 1;

    case 1:
        if (field_1AC_timer < static_cast<int>((sGnFrame_5C1B84 + 80)))
        {
            field_CC_sprite_scale -= FP_FromDouble(0.008);
            field_D0_r -= 2;
            field_D2_g -= 2;
            field_D4_b -= 2;
        }

        if (static_cast<int>(sGnFrame_5C1B84) < field_1AC_timer - 24 && !(sGnFrame_5C1B84 % 5))
        {
            New_Particles_426C70(
                (FP_FromInteger(Math_RandomRange_496AB0(-24, 24)) * field_CC_sprite_scale) + field_B8_xpos,
                field_BC_ypos - FP_FromInteger(6),
                field_CC_sprite_scale / FP_FromInteger(2),
                2, 
                128u, 128u, 128u);

            SFX_Play_46FBA0(79u, 25, FP_GetExponent((FP_FromInteger(2200) * field_CC_sprite_scale)));
        }

        if (field_1AC_timer < static_cast<int>(sGnFrame_5C1B84))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        return field_208_brain_sub_state;

    case 2:
    {
        auto pGibs = alive_new<Gibs>();
        if (pGibs)
        {
            pGibs->ctor_40FB40(
                1,
                field_B8_xpos,
                field_BC_ypos,
                field_C4_velx,
                field_C8_vely,
                field_CC_sprite_scale,
                0);
        }

        auto pBlood = alive_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(
                field_B8_xpos,
                field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale,
                20);
        }

        New_Particles_426C70(
            field_B8_xpos,
            field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
            field_CC_sprite_scale,
            3,
            128u,
            128u,
            128u);

        SFX_Play_46FA90(64u, 128, field_CC_sprite_scale);
        SFX_Play_46FA90(47u, 90, field_CC_sprite_scale);

        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);

        Set_AnimAndMotion_419890(12, TRUE);
        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);
        field_10C_health = FP_FromInteger(0);
        field_1AC_timer = sGnFrame_5C1B84 + 40;
        return 3;
    }

    case 3:
        if (static_cast<int>(sGnFrame_5C1B84) > field_1AC_timer)
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        return field_208_brain_sub_state;

    case 4:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_1AC_timer)
        {
            return field_208_brain_sub_state;
        }
        field_1AC_timer = sGnFrame_5C1B84 + 90;
        return 1;

    case 5:
        if (static_cast<int>(sGnFrame_5C1B84) < field_1AC_timer)
        {
            if (!(static_cast<int>(field_1AC_timer - sGnFrame_5C1B84) % 15))
            {
                Sfx_Slig_GameSpeak_4C04F0(
                    SligSpeak::Help_10,
                    static_cast<short>(2 * (field_1AC_timer & 0xFFFF - sGnFrame_5C1B84)),
                    field_1C2_pitch,
                    this);
            }

            if (static_cast<int>(sGnFrame_5C1B84) == field_1AC_timer - 6)
            {
                SND_SEQ_Play_4CAB10(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
            }
            return field_208_brain_sub_state;
        }
        else
        {
            Abe_SFX_2_457A40(15, 0, 0x7FFF, this);
            auto pScreenShake = alive_new<ScreenShake>();
            if (pScreenShake)
            {
                pScreenShake->ctor_4ACF70(0, 0);
            }
            field_1AC_timer = sGnFrame_5C1B84 + 30;
            return 3;
        }
        break;

    default:
        return field_208_brain_sub_state;
    }
}

__int16 NakedSlig::AI_5_Transformed_41ADF0()
{
    BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_1D8_obj_id);
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(0, this, 0, 0);
    }

    if (!pObj || pObj->field_6_flags.Get(BaseGameObject::eDead))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    return 0;
}

void NakedSlig::M_Idle_0_41B260()
{
    HandleCommon_41C0B0();
}

void NakedSlig::M_UsingButton_1_41B890()
{
    // Check for using a slig button which is a button than can trigger an id
    auto pSligButton = static_cast<NakedSligButton*>(sObjectIds_5C1B70.Find_449CF0(field_1D0_slig_button_id));
    if (pSligButton && field_20_animation.field_92_current_frame == 8)
    {
        pSligButton->UseButton_414C60();
        field_1D0_slig_button_id = -1;
    }
    // If not using a button check if we are on a locker to get pants or wings
    else if (field_1E4_pPantsOrWingsTlv)
    {
        if (field_20_animation.field_92_current_frame == 7)
        {
            SFX_Play_46FA90(93, 0);
        }

        if (static_cast<int>(sGnFrame_5C1B84) == field_1AC_timer - 1)
        {
            New_Particle_426F40(
                field_B8_xpos,
                (field_CC_sprite_scale * FP_FromInteger(45)) + field_BC_ypos,
                field_CC_sprite_scale);
        }
        else if (static_cast<int>(sGnFrame_5C1B84) > field_1AC_timer)
        {
            if (field_1E4_pPantsOrWingsTlv->field_4_type == TlvTypes::SligGetPants_104  && ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 412, 0, 0))
            {
                // Transform to a walking slig

                SFX_Play_46FA90(114u, 0);

                auto pWalkingSlig = alive_new<Slig>();
                if (pWalkingSlig)
                {
                    pWalkingSlig->ctor_4B1370(static_cast<Path_Slig*>(field_1E4_pPantsOrWingsTlv), sPath_dword_BB47C0->TLVInfo_From_TLVPtr_4DB7C0(field_1E4_pPantsOrWingsTlv));
                }

                field_1D8_obj_id = pWalkingSlig->field_8_object_id;
                pWalkingSlig->field_CC_sprite_scale = field_CC_sprite_scale;

                pWalkingSlig->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX));

                if (BrainIs(&NakedSlig::AI_3_Possesed_41A5B0))
                {
                    pWalkingSlig->field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
                    pWalkingSlig->field_146_level = field_1BA_prev_level;
                    pWalkingSlig->field_148_path = field_1BC_prev_path;
                    pWalkingSlig->field_14A_camera = field_1BE_prev_camera;
                    pWalkingSlig->SetBrain(&Slig::AI_Possessed_2_4BBCF0);
                    pWalkingSlig->field_11C_ai_sub_state = 4;
                    sControlledCharacter_5C1B8C = pWalkingSlig;
                }
                field_10C_health = FP_FromInteger(0);
            }
            else if (field_1E4_pPantsOrWingsTlv->field_4_type == TlvTypes::SligGetWings_105 && ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 450, 0, 0))
            {
                // Transform to a flying slig

                SFX_Play_46FA90(113u, 0);

                auto pFlyingSlig = alive_new<FlyingSlig>();
                if (pFlyingSlig)
                {
                    pFlyingSlig->ctor_4342B0(static_cast<Path_FlyingSlig*>(field_1E4_pPantsOrWingsTlv), sPath_dword_BB47C0->TLVInfo_From_TLVPtr_4DB7C0(field_1E4_pPantsOrWingsTlv));
                }

                field_1D8_obj_id = pFlyingSlig->field_8_object_id;
                pFlyingSlig->field_B8_xpos = field_B8_xpos;
                pFlyingSlig->field_BC_ypos = field_BC_ypos - FP_FromInteger(15);
                pFlyingSlig->field_294_nextXPos = field_B8_xpos;
                pFlyingSlig->field_298_nextYPos = pFlyingSlig->field_BC_ypos;
                pFlyingSlig->field_CC_sprite_scale = field_CC_sprite_scale;
                pFlyingSlig->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX));

                if (BrainIs(&NakedSlig::AI_3_Possesed_41A5B0))
                {
                    pFlyingSlig->ToPlayerControlled_4360C0();
                    pFlyingSlig->field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
                    pFlyingSlig->field_2A0_abe_level = field_1BA_prev_level;
                    pFlyingSlig->field_2A2_abe_path = field_1BC_prev_path;
                    pFlyingSlig->field_2A4_abe_camera = field_1BE_prev_camera;
                    sControlledCharacter_5C1B8C = pFlyingSlig;
                    pFlyingSlig->field_2A8_max_x_speed = (FP_FromDouble(5.5) * field_CC_sprite_scale);
                    pFlyingSlig->field_2AC_up_vel = (-FP_FromDouble(5.5) * field_CC_sprite_scale);
                    pFlyingSlig->field_2B0_down_vel = (FP_FromDouble(5.5) * field_CC_sprite_scale);
                    pFlyingSlig->field_2B4_max_slow_down = (FP_FromDouble(0.3) * field_CC_sprite_scale);
                    pFlyingSlig->field_2B8_max_speed_up = (FP_FromDouble(0.8) * field_CC_sprite_scale);
                }
                else
                {
                    pFlyingSlig->SetBrain(&FlyingSlig::AI_FromNakedSlig_17_4355E0);
                }
                field_10C_health = FP_FromInteger(0);
            }
            else
            {
                // Impossible case - slig will go away and not transform into anything ??
            }

            // Final transform
            field_6_flags.Set(BaseGameObject::eDead);
            SetBrain(&NakedSlig::AI_5_Transformed_41ADF0);
            field_C8_vely = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            Set_AnimAndMotion_419890(0, TRUE);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_4_typeId = Types::eNone_0;
        }
    }
    else
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            HandleCommon_41C0B0();
        }
    }
}

void NakedSlig::M_WakingUp_2_41BF00()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eSnoozParticle_124)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1E4_state = SnoozeParticle::SnoozeParticleState::BlowingUp_2;
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon_41C0B0();
    }
}

void NakedSlig::M_Crawling_3_41B280()
{
    if (CanCrawl_41C5D0())
    {
        if (field_20_animation.field_92_current_frame == 3 || field_20_animation.field_92_current_frame == 6)
        {
            Slig_SoundEffect_4BFFE0(Math_RandomRange_496AB0(14, 16), this);
        }
        else if (field_20_animation.field_92_current_frame == 11)
        {
            if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
            {
                if (field_108_next_motion == NakedSligMotion::M_Idle_0_41B260 ||
                    field_108_next_motion == NakedSligMotion::M_TurnAround_11_41B590 ||
                    field_108_next_motion == NakedSligMotion::M_ToShakingToIdle_7_41C010 ||
                    field_108_next_motion == NakedSligMotion::M_Speaking_8_41BF70)
                {
                    Set_AnimAndMotion_419890(15, FALSE);
                }
            }
            else
            {
                if ((field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(InputCommands::eLeft)) ||
                    (field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(InputCommands::eRight)) ||
                   !(sInputObject_5BD4E0.isPressed(InputCommands::eLeft | InputCommands::eRight)))
                {
                    Set_AnimAndMotion_419890(15, TRUE);
                }
            }
            MapFollowMe_408D10(TRUE);
        }
    }
}

void NakedSlig::M_StartFalling_4_41B620()
{
    M_Falling_5_41B650();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Set_AnimAndMotion_419890(NakedSligMotion::M_Falling_5_41B650, TRUE);
    }
}

void NakedSlig::M_Falling_5_41B650()
{
    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx = field_C4_velx - (field_CC_sprite_scale * field_1B0);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx = (field_CC_sprite_scale * field_1B0) + field_C4_velx;
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const __int16  bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (BrainIs(&NakedSlig::AI_3_Possesed_41A5B0))
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
            PlatformCollide_41C500();
            field_BC_ypos = hitY;
            field_B8_xpos = hitX;
            MapFollowMe_408D10(TRUE);
            if ((hitY - field_F8_LastLineYPos) > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(5)))
            {
                SetBrain(&NakedSlig::AI_4_GetKilled_41A880);
                field_208_brain_sub_state = 2;
            }
            else
            {
                Set_AnimAndMotion_419890(6, TRUE);
            }
            break;

        case 1u:
        case 2u:
            field_C4_velx = -field_C4_velx / FP_FromInteger(2);
            break;

        default:
            return;
        }
    }
}

void NakedSlig::M_Landing_6_41B870()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Set_AnimAndMotion_419890(NakedSligMotion::M_ShakingToIdle_14_41C040, TRUE);
    }
}

void NakedSlig::M_ToShakingToIdle_7_41C010()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_108_next_motion == NakedSligMotion::M_ShakingToIdle_14_41C040)
        {
            Set_AnimAndMotion_419890(NakedSligMotion::M_ShakingToIdle_14_41C040, TRUE);
        }
    }
}

void NakedSlig::M_Speaking_8_41BF70()
{
    if (field_20_animation.field_92_current_frame == 2 && field_1C0_speak != SligSpeak::None)
    {
        if (gMap_5C3030.sCurrentPathId_5C3032 == field_C0_path_number && 
            gMap_5C3030.sCurrentLevelId_5C3030 == field_C2_lvl_number && 
            Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
        {
            Sfx_Slig_GameSpeak_4C04F0(field_1C0_speak, 0, 0, this);
        }
        field_1C0_speak = SligSpeak::None;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon_41C0B0();
    }
}

void NakedSlig::M_Snoozing_9_41BD80()
{
    if (field_108_next_motion == NakedSligMotion::M_WakingUp_2_41BF00)
    {
        Set_AnimAndMotion_419890(NakedSligMotion::M_WakingUp_2_41BF00, TRUE);
    }
    else if (!(sGnFrame_5C1B84 & 31))
    {
        if ((sGnFrame_5C1B84 / 25) & 1)
        {
            Slig_SoundEffect_4BFFE0(5, this);
        }
        else
        {
            Slig_SoundEffect_4BFFE0(4, this);
        }

        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
        {
            auto pSnoozeParticle = alive_new<SnoozeParticle>();
            if (pSnoozeParticle)
            {
                FP xOff = {};
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    xOff = -(field_CC_sprite_scale * FP_FromInteger(-10));
                }
                else
                {
                    xOff = (field_CC_sprite_scale * FP_FromInteger(-10));
                }

                const FP yOff = (field_CC_sprite_scale * FP_FromInteger(-10));
                pSnoozeParticle->ctor_4B06F0(
                    field_B8_xpos + xOff,
                    field_BC_ypos + yOff,
                    field_20_animation.field_C_render_layer,
                    field_20_animation.field_14_scale);
            }
        }
    }
}

void NakedSlig::M_PushingWall_10_41B400()
{
    MoveOnLine_41C3D0();

    if (field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 8)
    {
        MapFollowMe_408D10(TRUE);
        field_C4_velx = FP_FromInteger(0);
        Slig_SoundEffect_4BFFE0(Math_RandomRange_496AB0(14, 16), this);
    }

    if(BrainIs(&NakedSlig::AI_3_Possesed_41A5B0))
    {
        const bool flipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
        if ((!flipX && sInputObject_5BD4E0.isPressed(InputCommands::eLeft)) ||
            (flipX && sInputObject_5BD4E0.isPressed(InputCommands::eRight)) ||
            !(sInputObject_5BD4E0.isPressed(InputCommands::eLeft | InputCommands::eRight)))
        {
            Set_AnimAndMotion_419890(NakedSligMotion::M_EndPushingWall_17_41B3A0, TRUE);
        }
        else
        {
            FP yPos = {};
            if (flipX)
            {
                yPos = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }
            else
            {
                yPos = ScaleToGridSize_4498B0(field_CC_sprite_scale);
            }

            if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(30), yPos))
            {
                Set_AnimAndMotion_419890(NakedSligMotion::M_Crawling_3_41B280, TRUE);
            }
        }
    }
    else
    {
        if (field_108_next_motion == NakedSligMotion::M_Idle_0_41B260 ||
            field_108_next_motion == NakedSligMotion::M_UsingButton_1_41B890 ||
            field_108_next_motion == NakedSligMotion::M_TurnAround_11_41B590 ||
            field_108_next_motion == NakedSligMotion::M_ToShakingToIdle_7_41C010 ||
            field_108_next_motion == NakedSligMotion::M_Speaking_8_41BF70)
        {
            Set_AnimAndMotion_419890(NakedSligMotion::M_EndPushingWall_17_41B3A0, FALSE);
        }
    }
}

void NakedSlig::M_TurnAround_11_41B590()
{
    if (field_20_animation.field_92_current_frame == 8)
    {
        Slig_SoundEffect_4BFFE0(Math_RandomRange_496AB0(14, 16), this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        MapFollowMe_408D10(TRUE);
        HandleCommon_41C0B0();
    }
}

void NakedSlig::M_Shaking_12_418C30()
{
    // Do nothing
}

void NakedSlig::M_Empty_13_418C50()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_ShakingToIdle_14_41C040()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_10C_health > FP_FromInteger(0))
        {
            field_10C_health = FP_FromInteger(1);
        }
        ToIdle_41C070();
    }
}

void NakedSlig::M_EndCrawling_15_41B600()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon_41C0B0();
    }
}

void NakedSlig::M_IdleToPushingWall_16_41B3C0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Sfx_Slig_GameSpeak_4C04F0(static_cast<SligSpeak>(Math_RandomRange_496AB0(static_cast<int>(SligSpeak::Ouch1_13), static_cast<int>(SligSpeak::Ouch2_14))), 0, 0, this);
        Set_AnimAndMotion_419890(NakedSligMotion::M_PushingWall_10_41B400, TRUE);
    }
}

void NakedSlig::M_EndPushingWall_17_41B3A0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle_41C070();
    }
}

void NakedSlig::HandleCommon_41C0B0()
{
    MapFollowMe_408D10(TRUE);

    if (BrainIs(&NakedSlig::AI_3_Possesed_41A5B0) && field_208_brain_sub_state == 1)
    {
        if (sInputObject_5BD4E0.isPressed(InputCommands::eRight))
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_108_next_motion = NakedSligMotion::M_TurnAround_11_41B590;
            }
            else
            {
                field_108_next_motion = NakedSligMotion::M_Crawling_3_41B280;
            }
        }
        else if (sInputObject_5BD4E0.isPressed(InputCommands::eLeft))
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_108_next_motion = NakedSligMotion::M_Crawling_3_41B280;
            }
            else
            {
                field_108_next_motion = NakedSligMotion::M_TurnAround_11_41B590;
            }
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eUp))
        {
            field_1E4_pPantsOrWingsTlv = FindPantsOrWings_419750();
            if (field_1E4_pPantsOrWingsTlv)
            {
                if (!field_1E4_pPantsOrWingsTlv->field_1_unknown)
                {
                    field_1E4_pPantsOrWingsTlv->field_1_unknown &= 0xFF;
                    field_1E4_pPantsOrWingsTlv->field_1_unknown |= 1;
                    field_108_next_motion = 1;
                    field_1AC_timer = sGnFrame_5C1B84 + 20;
                }
            }
            else
            {
                auto pSligButton = FindSligButton_419840();
                if (pSligButton)
                {
                    field_108_next_motion = NakedSligMotion::M_UsingButton_1_41B890;
                    field_1D0_slig_button_id = pSligButton->field_8_object_id;
                }
                else
                {
                    field_108_next_motion = NakedSligMotion::M_Speaking_8_41BF70;
                    field_1C0_speak = SligSpeak::What_9;
                }
            }
        }
        if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak1))
        {
            field_1C0_speak = SligSpeak::Hi_0;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak3))
        {
            field_1C0_speak = sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0 ? SligSpeak::GetEm_2 : SligSpeak ::Freeze_8;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak4))
        {
            field_1C0_speak = sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0 ? SligSpeak ::Freeze_8 : SligSpeak::GetEm_2;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak2))
        {
            field_1C0_speak = SligSpeak::HereBoy_1;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak6))
        {
            field_1C0_speak = SligSpeak::Bullshit_5;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak7))
        {
            field_1C0_speak = SligSpeak::LookOut_6;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak5))
        {
            field_1C0_speak = SligSpeak::Bullshit2_7;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak8))
        {
            field_1C0_speak = SligSpeak::Laugh_3;
        }

        if (field_1C0_speak != SligSpeak::None)
        {
            field_108_next_motion = NakedSligMotion::M_Speaking_8_41BF70;
        }
    }

    switch (field_108_next_motion)
    {
    case -1: // TODO: None constant
        if (field_106_current_motion != NakedSligMotion::M_Idle_0_41B260)
        {
            field_108_next_motion = NakedSligMotion::M_Idle_0_41B260;
            Set_AnimAndMotion_419890(field_108_next_motion, TRUE);
            break;
        }
        break;

    case NakedSligMotion::M_Idle_0_41B260:
    case NakedSligMotion::M_UsingButton_1_41B890:
    case NakedSligMotion::M_ToShakingToIdle_7_41C010:
    case NakedSligMotion::M_Speaking_8_41BF70:
    case NakedSligMotion::M_TurnAround_11_41B590:
        Set_AnimAndMotion_419890(field_108_next_motion, TRUE);
        break;

    case NakedSligMotion::M_Crawling_3_41B280:
    {
        FP gridScale = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridScale = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            gridScale = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(30), gridScale))
        {
            if (sControlledCharacter_5C1B8C == this)
            {
                field_108_next_motion = NakedSligMotion::M_IdleToPushingWall_16_41B3C0;
            }
        }
        Set_AnimAndMotion_419890(field_108_next_motion, TRUE);
        break;
    }

    default:
        return;
    }
}

const FP dword_54471C[15] =
{
    FP_FromDouble(0),
    FP_FromDouble(0),
    FP_FromDouble(0),
    FP_FromDouble(1.17),
    FP_FromDouble(2.56),
    FP_FromDouble(2.85),
    FP_FromDouble(2.23),
    FP_FromDouble(1.87),
    FP_FromDouble(3.47),
    FP_FromDouble(3.61),
    FP_FromDouble(3.96),
    FP_FromDouble(2.69),
    FP_FromDouble(0),
    FP_FromDouble(0),
    FP_FromDouble(0)
};

const SfxDefinition stru_5607E0[17] =
{
    { 0u, 1u, 58u, 40u, -256, -256 },
    { 0u, 1u, 58u, 35u, 0, 0 },
    { 0u, 1u, 59u, 55u, 0, 0 },
    { 0u, 1u, 59u, 55u, 127, 127 },
    { 0u, 1u, 60u, 60u, 0, 0 },
    { 0u, 1u, 61u, 60u, 0, 0 },
    { 0u, 1u, 54u, 60u, 0, 0 },
    { 0u, 1u, 55u, 60u, 0, 0 },
    { 0u, 39u, 60u, 110u, -1000, -950 },
    { 0u, 1u, 48u, 90u, 0, 0 },
    { 0u, 1u, 49u, 90u, 0, 0 },
    { 0u, 1u, 50u, 90u, 0, 0 },
    { 0u, 0u, 66u, 50u, -512, -384 },
    { 0u, 0u, 67u, 50u, -512, -384 },
    { 0u, 3u, 36u, 60u, 0, 0 },
    { 0u, 3u, 37u, 60u, 0, 0 },
    { 0u, 3u, 38u, 60u, 0, 0 }
};

void CC Slig_SoundEffect_4BFFE0(__int16 effect, BaseAliveGameObject* pObj)
{
    LOG_INFO("effect num = " << effect);

    const SfxDefinition* pEffect = &stru_5607E0[effect];
    short vLeft = 0;
    short vRight = 0;
    if (Calc_Slig_Sound_Direction_4C01B0(pObj, 0, pEffect, &vLeft, &vRight))
    {
        short pitch = 0;
        if (effect == 9 || effect == 10 || effect == 11)
        {
            FP sndDistance = FP_FromInteger(abs(Math_Distance_496EB0(0, 0, FP_GetExponent(pObj->field_C4_velx), FP_GetExponent(pObj->field_C8_vely))));
            if (sndDistance > FP_FromInteger(8))
            {
                sndDistance = FP_FromInteger(8);
            }

            pitch = Math_RandomRange_496AB0(-127, 127) + FP_GetExponent((sndDistance / FP_FromInteger(8)) * FP_FromInteger(768)) + 512;
        }
        else
        {
            pitch = Math_RandomRange_496AB0(pEffect->field_4_pitch_min, pEffect->field_6_pitch_max);
        }
        SFX_SfxDefinition_Play_4CA700(pEffect, vLeft, vRight, pitch, pitch);
    }
}

__int16 NakedSlig::CanCrawl_41C5D0()
{
    field_C4_velx = dword_54471C[field_20_animation.field_92_current_frame];
    
    FP gridScale = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -field_C4_velx;
        gridScale = -gridScale;
    }

    field_C4_velx = (field_C4_velx * field_CC_sprite_scale);

    if (sControlledCharacter_5C1B8C == this && WallHit_408750(field_CC_sprite_scale * FP_FromInteger(30), gridScale))
    {
        field_1B0 = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);
        Set_AnimAndMotion_419890(10, TRUE);
        const int snappedX = SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos));
        field_C4_velx = ((FP_FromInteger(snappedX) - field_B8_xpos) / FP_FromInteger(4));
        Sfx_Slig_GameSpeak_4C04F0(static_cast<SligSpeak>(Math_RandomRange_496AB0(static_cast<int>(SligSpeak::Ouch1_13), static_cast<int>(SligSpeak::Ouch2_14))), 0, 0, this);
        return FALSE;
    }
    else
    {
        MoveOnLine_41C3D0();
        return field_100_pCollisionLine != nullptr;
    }
}

void NakedSlig::MoveOnLine_41C3D0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
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
                }
            }
            else if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                PlatformCollide_41C500();
            }
        }
        else
        {
            VOnTrapDoorOpen();
            field_F8_LastLineYPos = field_BC_ypos;
            if (field_106_current_motion == NakedSligMotion::M_Crawling_3_41B280)
            {
                Set_AnimAndMotion_419890(NakedSligMotion::M_StartFalling_4_41B620, TRUE);
            }
            field_1B0 = FP_FromInteger(1);
            field_B8_xpos = field_B8_xpos + field_C4_velx;
        }
    }
    else
    {
        field_F8_LastLineYPos = field_BC_ypos;
        Set_AnimAndMotion_419890(NakedSligMotion::M_Falling_5_41B650, TRUE);
    }
}

void NakedSlig::PlatformCollide_41C500()
{
    PSX_RECT bRect = {};
    vGetBoundingRect_424FD0(&bRect, 1);

    PSX_Point xy = { bRect.x, bRect.y };
    PSX_Point wh = { bRect.w, bRect.h };
    xy.field_2_y += 5;
    wh.field_2_y += 5;

    vOnCollisionWith_424EE0(
        xy,
        wh,
        ObjList_5C1B78, 1,
        (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
}
