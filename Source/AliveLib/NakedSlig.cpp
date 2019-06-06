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
    &NakedSlig::M_0_41B260,                // Sleeping
    &NakedSlig::M_1_TryToTransform_41B890,                // Transform/use slig button ?
    &NakedSlig::M_2_41BF00,                // Speak ??
    &NakedSlig::M_3_Crawling_41B280,                // Crawling
    &NakedSlig::M_4_41B620,                // Beat/chant shake?
    &NakedSlig::M_5_41B650,                // Gibbed/explode
    &NakedSlig::M_6_41B870,                // Start to be hit or hit floor ??
    &NakedSlig::M_7_41C010,                // Idle not moving ??
    &NakedSlig::M_8_41BF70,                // Speak
    &NakedSlig::M_9_Snoozing_41BD80,
    &NakedSlig::M_10_StartToPushOnWall_41B400,               // Pushing into wall
    &NakedSlig::M_11_TurnAround_41B590,               // Turning
    &NakedSlig::M_12_PossessedFlapAbout_418C30,               // Out of chant shake
    &NakedSlig::M_13_418C50,               // 
    &NakedSlig::M_14_PossessedToIdle_41C040,               // Idle 2 ??
    &NakedSlig::M_15_41B600,               // Idle 3 ??
    &NakedSlig::M_16_PushWall_41B3C0,               // Pushing into wall saying ow ?
    &NakedSlig::M_17_StopPushingWall_41B3A0                // Crawl to idle ??
});

struct ReimplToRealPair
{
    TNakedSligAIFn mOurFn;
    DWORD mOriginalGameFn[2];
};

const ReimplToRealPair sAiFns[] =
{
    { &NakedSlig::AI_0_Sleeping_419DE0, {0x419DE0, 0x401D1B } },
    { &NakedSlig::AI_1_Idle_419F60, {0x419F60, 0x40340E } },
    { &NakedSlig::AI_2_PanicGetALocker_419FE0, { 0x419FE0, 0x419FE0 } },
    { &NakedSlig::AI_3_Possesed_41A5B0, {0x41A5B0, 0x404539 } },
    { &NakedSlig::AI_4_GetKilled_41A880, {0x41A880, 0x403265 } },
    { &NakedSlig::AI_5_41ADF0, {0x41ADF0, 0x40484A } },
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
    field_1C0_speak = NakedSligSpeak::Speak_None;
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
        Set_AnimAndMotion_419890(NakedSligMotion::M_0_41B260, TRUE);
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
        Set_AnimAndMotion_419890(NakedSligMotion::M_9_Snoozing_41BD80, TRUE);
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

void NakedSlig::vPossessed_4195F0()
{
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_1B8_bChanting = TRUE;
    Set_AnimAndMotion_419890(NakedSligMotion::M_12_PossessedFlapAbout_418C30, TRUE);
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

        field_208_brain_sub_state = (this->*field_204_brain_state)();
        
        const FP oldX = field_B8_xpos;
        const FP oldY = field_BC_ypos;

        (this->*sNakedSlig_motions_551428[field_106_current_motion])();

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

__int16 NakedSlig::sub_41C740(FP /*velX*/)
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

#if _WIN32 || !_WIN64
static DWORD GetOriginalFn(TNakedSligAIFn fn)
{
    // If not running as standalone set the address to be
    // the address of the real function rather than the reimpl as the real
    // game code compares the function pointer addresses (see IsBrain(x)).
    for (const auto& addrPair : sAiFns)
    {
        if (addrPair.mOurFn == fn )
        {
            const DWORD actualAddressToUse = addrPair.mOriginalGameFn[1];
            // Hack to overwrite the member function pointer bytes with arbitrary data
            return actualAddressToUse;
        }
    }
    ALIVE_FATAL("No matching address!");
}
#endif

void NakedSlig::SetBrain(TNakedSligAIFn fn)
{
#if _WIN32 || !_WIN64
    if (IsAlive())
    {
        const DWORD actualAddressToUse = GetOriginalFn(fn);
        // Hack to overwrite the member function pointer bytes with arbitrary data
        memcpy(&field_204_brain_state, &actualAddressToUse, sizeof(DWORD));
        return;
    }
#endif
    field_204_brain_state = fn;
}

bool NakedSlig::BrainIs(TNakedSligAIFn fn)
{
#if _WIN32 || !_WIN64
    if (IsAlive())
    {
        const DWORD actualAddressToUse = GetOriginalFn(fn);
        TNakedSligAIFn hack = nullptr;
        memcpy(&hack, &actualAddressToUse, sizeof(DWORD));
        return hack == field_204_brain_state;
    }
#endif
    return field_204_brain_state == fn;
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
    field_1B0 = 0;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    Set_AnimAndMotion_419890(NakedSligMotion::M_0_41B260, FALSE);
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

        field_108_next_motion = NakedSligMotion::M_2_41BF00;
        return 2;
    }

    if (field_208_brain_sub_state != 2 || field_106_current_motion != NakedSligMotion::M_2_41BF00 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
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
    SetBrain(&NakedSlig::AI_2_PanicGetALocker_419FE0); // j_NakedSlig::AI_2_419FE0
    return 0;
}

__int16 NakedSlig::AI_1_Idle_419F60()
{
    NOT_IMPLEMENTED();
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
            field_108_next_motion = NakedSligMotion::M_3_Crawling_41B280;
            return 2;
        }
        field_108_next_motion = NakedSligMotion::M_11_TurnAround_41B590;
        return 3;

    case 1:
        if (field_106_current_motion != NakedSligMotion::M_0_41B260)
        {
            return field_208_brain_sub_state;
        }
        return 0;

    case 2:
        if (sub_41C740(field_C4_velx))
        {
            field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame_5C1B84 + 30;
            field_108_next_motion = NakedSligMotion::M_11_TurnAround_41B590;
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
            field_108_next_motion = NakedSligMotion::M_8_41BF70;
            field_1C8 = sGnFrame_5C1B84 + 60;
            field_1C0_speak = NakedSligSpeak::Speak_10;
            return 4;
        }
        break;

    case 3:
        if (field_106_current_motion != NakedSligMotion::M_11_TurnAround_41B590 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_208_brain_sub_state;
        }
        field_108_next_motion = NakedSligMotion::M_3_Crawling_41B280;
        return 2;

    case 4:
    {
        if (field_106_current_motion != NakedSligMotion::M_8_41BF70 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
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

        if (!Raycast_408750(FP_FromInteger(35), gridScale))
        {
            field_108_next_motion = NakedSligMotion::M_3_Crawling_41B280;
            return 2;
        }
        field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame_5C1B84 + 30;
        field_108_next_motion = NakedSligMotion::M_11_TurnAround_41B590;
        return 7;
    }

    case 5:
        if (!field_1E4_pPantsOrWingsTlv)
        {
            field_1E4_pPantsOrWingsTlv = FindPantsOrWings_419750();
        }

        if (field_106_current_motion != NakedSligMotion::M_0_41B260 || (field_1E4_pPantsOrWingsTlv && field_1E4_pPantsOrWingsTlv->field_1_unknown))
        {
            if (Math_NextRandom() & 1)
            {
                field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame_5C1B84 + 30;
                return 8;
            }
            else
            {
                field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame_5C1B84 + 30;
                field_108_next_motion = NakedSligMotion::M_11_TurnAround_41B590;
                return 7;
            }
        }
        else
        {
            field_1E4_pPantsOrWingsTlv->field_1_unknown &= 0xFF;
            field_1E4_pPantsOrWingsTlv->field_1_unknown |= 1;

            field_108_next_motion = NakedSligMotion::M_1_TryToTransform_41B890;
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
        if (field_106_current_motion == NakedSligMotion::M_11_TurnAround_41B590 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_108_next_motion = NakedSligMotion::M_3_Crawling_41B280;
            return 8;
        }
        return field_208_brain_sub_state;

    case 8:
        if (sub_41C740(field_C4_velx))
        {
            field_108_next_motion = NakedSligMotion::M_11_TurnAround_41B590;
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

        field_108_next_motion = NakedSligMotion::M_8_41BF70;
        field_1C8 = sGnFrame_5C1B84 + 60;
        field_1C0_speak = NakedSligSpeak::Speak_10;
        return 9;

    case 9:
    {
        if (field_106_current_motion != NakedSligMotion::M_8_41BF70 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
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

        if (Raycast_408750(FP_FromInteger(35), gridScale))
        {
            field_108_next_motion = NakedSligMotion::M_11_TurnAround_41B590;
            return 3;
        }
        else
        {
            field_108_next_motion = NakedSligMotion::M_3_Crawling_41B280;
            return 8;
        }
    }
        break;

    case 10:
        if (field_106_current_motion == NakedSligMotion::M_14_PossessedToIdle_41C040)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                Set_AnimAndMotion_419890(NakedSligMotion::M_0_41B260, TRUE);
                field_208_brain_sub_state = 0;
                return field_208_brain_sub_state;
            }
        }
        else if (static_cast<int>(sGnFrame_5C1B84) > field_1AC_timer)
        {
            Set_AnimAndMotion_419890(NakedSligMotion::M_14_PossessedToIdle_41C040, TRUE);
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
        Set_AnimAndMotion_419890(NakedSligMotion::M_14_PossessedToIdle_41C040, TRUE);
        return 1;

    case 1:
        if (!Input_IsChanting_45F260() || field_1B8_bChanting)
        {
            return field_208_brain_sub_state;
        }
        field_1AC_timer = sGnFrame_5C1B84 + 30;
        SFX_Play_46FA90(17, 0);
        Set_AnimAndMotion_419890(NakedSligMotion::M_12_PossessedFlapAbout_418C30, TRUE);
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
            Set_AnimAndMotion_419890(NakedSligMotion::M_14_PossessedToIdle_41C040, TRUE);
            return 1;
        }
        break;

    case 3:
        if (field_106_current_motion != NakedSligMotion::M_14_PossessedToIdle_41C040)
        {
            if (static_cast<int>(sGnFrame_5C1B84) > field_1AC_timer)
            {
                Set_AnimAndMotion_419890(NakedSligMotion::M_14_PossessedToIdle_41C040, TRUE);
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
        if (field_106_current_motion != NakedSligMotion::M_7_41C010 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
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
                Sfx_Slig_4C04F0(
                    10,
                    static_cast<short>(2 * (field_1AC_timer & 0xFFFF - sGnFrame_5C1B84)),
                    field_1C2_pitch,
                    this);
            }

            if (static_cast<int>(sGnFrame_5C1B84) == field_1AC_timer - 6)
            {
                SND_SEQ_Play_4CAB10(9u, 1, 65, 65);
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

__int16 NakedSlig::AI_5_41ADF0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void NakedSlig::M_0_41B260()
{
    HandleCommon_41C0B0();
}

void NakedSlig::M_1_TryToTransform_41B890()
{
    NOT_IMPLEMENTED();
    HandleCommon_41C0B0(); // TODO: Standalone HACK HACK HACK!
}

void NakedSlig::M_2_41BF00()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_3_Crawling_41B280()
{
    if (CanCrawl_41C5D0())
    {
        if (field_20_animation.field_92_current_frame == 3 || field_20_animation.field_92_current_frame == 6)
        {
            Slig_Sfx_4BFFE0(Math_RandomRange_496AB0(14, 16), this);
        }
        else if (field_20_animation.field_92_current_frame == 11)
        {
            if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
            {
                if (field_108_next_motion == NakedSligMotion::M_0_41B260 ||
                    field_108_next_motion == NakedSligMotion::M_11_TurnAround_41B590 ||
                    field_108_next_motion == NakedSligMotion::M_7_41C010 ||
                    field_108_next_motion == NakedSligMotion::M_8_41BF70)
                {
                    Set_AnimAndMotion_419890(15, FALSE);
                }
            }
            else
            {
                if ((field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(InputCommands::eLeft)) ||
                    (field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(InputCommands::eRight)) ||
                   !(sInputObject_5BD4E0.isPressed(InputCommands::eGameSpeak3))) // Was 0xC
                {
                    Set_AnimAndMotion_419890(15, TRUE);
                }
            }
            MapFollowMe_408D10(TRUE);
        }
    }
}

void NakedSlig::M_4_41B620()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_5_41B650()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_6_41B870()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_7_41C010()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_8_41BF70()
{
    if (field_20_animation.field_92_current_frame == 2 && field_1C0_speak != NakedSligSpeak::Speak_None)
    {
        if (gMap_5C3030.sCurrentPathId_5C3032 == field_C0_path_number && 
            gMap_5C3030.sCurrentLevelId_5C3030 == field_C2_lvl_number && 
            Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
        {
            Sfx_Slig_4C04F0(static_cast<short>(field_1C0_speak), 0, 0, this);
        }
        field_1C0_speak = NakedSligSpeak::Speak_None;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon_41C0B0();
    }
}

void NakedSlig::M_9_Snoozing_41BD80()
{
    if (field_108_next_motion == NakedSligMotion::M_2_41BF00)
    {
        Set_AnimAndMotion_419890(NakedSligMotion::M_2_41BF00, TRUE);
    }
    else if (!(sGnFrame_5C1B84 & 31))
    {
        if ((sGnFrame_5C1B84 / 25) & 1)
        {
            Slig_Sfx_4BFFE0(5, this);
        }
        else
        {
            Slig_Sfx_4BFFE0(4, this);
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

void NakedSlig::M_10_StartToPushOnWall_41B400()
{
    MoveOnLine_41C3D0();

    if (field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 8)
    {
        MapFollowMe_408D10(TRUE);
        field_C4_velx = FP_FromInteger(0);
        Slig_Sfx_4BFFE0(Math_RandomRange_496AB0(14, 16), this);
    }

    if(BrainIs(&NakedSlig::AI_3_Possesed_41A5B0))
    {
        const bool flipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
        if ((!flipX && sInputObject_5BD4E0.isPressed(InputCommands::eLeft)) ||
            (flipX && sInputObject_5BD4E0.isPressed(InputCommands::eRight)) ||
            !(sInputObject_5BD4E0.isPressed((InputCommands)(InputCommands::eLeft | InputCommands::eRight))))
        {
            Set_AnimAndMotion_419890(NakedSligMotion::M_17_StopPushingWall_41B3A0, TRUE);
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

            if (!Raycast_408750(field_CC_sprite_scale * FP_FromInteger(30), yPos))
            {
                Set_AnimAndMotion_419890(NakedSligMotion::M_3_Crawling_41B280, TRUE);
            }
        }
    }
    else
    {
        if (field_108_next_motion == NakedSligMotion::M_0_41B260 ||
            field_108_next_motion == NakedSligMotion::M_1_TryToTransform_41B890 ||
            field_108_next_motion == NakedSligMotion::M_11_TurnAround_41B590 ||
            field_108_next_motion == NakedSligMotion::M_7_41C010 ||
            field_108_next_motion == NakedSligMotion::M_8_41BF70)
        {
            Set_AnimAndMotion_419890(NakedSligMotion::M_17_StopPushingWall_41B3A0, FALSE);
        }
    }
}

void NakedSlig::M_11_TurnAround_41B590()
{
    if (field_20_animation.field_92_current_frame == 8)
    {
        Slig_Sfx_4BFFE0(Math_RandomRange_496AB0(14, 16), this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        MapFollowMe_408D10(TRUE);
        HandleCommon_41C0B0();
    }
}

void NakedSlig::M_12_PossessedFlapAbout_418C30()
{
    // Do nothing
}

void NakedSlig::M_13_418C50()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_14_PossessedToIdle_41C040()
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

void NakedSlig::M_15_41B600()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon_41C0B0();
    }
}

void NakedSlig::M_16_PushWall_41B3C0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Sfx_Slig_4C04F0(Math_RandomRange_496AB0(13, 14), 0, 0, this);
        Set_AnimAndMotion_419890(NakedSligMotion::M_10_StartToPushOnWall_41B400, TRUE);
    }
}

void NakedSlig::M_17_StopPushingWall_41B3A0()
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
      //TODO: Refactor this into isPressed.

      if (sInputObject_5BD4E0.isPressed(InputCommands::eRight))
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_108_next_motion = NakedSligMotion::M_11_TurnAround_41B590;
            }
            else
            {
                field_108_next_motion = NakedSligMotion::M_3_Crawling_41B280;
            }
        }
        else if (sInputObject_5BD4E0.isPressed(InputCommands::eLeft))
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_108_next_motion = NakedSligMotion::M_3_Crawling_41B280;
            }
            else
            {
                field_108_next_motion = NakedSligMotion::M_11_TurnAround_41B590;
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
                    field_108_next_motion = NakedSligMotion::M_1_TryToTransform_41B890;
                    field_1D0_slig_button_id = pSligButton->field_8_object_id;
                }
                else
                {
                    field_108_next_motion = NakedSligMotion::M_8_41BF70;
                    field_1C0_speak = NakedSligSpeak::Speak_9;
                }
            }
        }
        if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak1))
        {
            field_1C0_speak = NakedSligSpeak::Speak_0;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak3))
        {
            field_1C0_speak = sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0 ? NakedSligSpeak::Speak_2 : NakedSligSpeak::Speak_8;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak4))
        {
            field_1C0_speak = sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0 ? NakedSligSpeak::Speak_8 : NakedSligSpeak::Speak_2;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak2))
        {
            field_1C0_speak = NakedSligSpeak::Speak_1;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak6))
        {
            field_1C0_speak = NakedSligSpeak::Speak_5;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak7))
        {
            field_1C0_speak = NakedSligSpeak::Speak_6;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak5))
        {
            field_1C0_speak = NakedSligSpeak::Speak_7;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::eGameSpeak8))
        {
            field_1C0_speak = NakedSligSpeak::Speak_3;
        }

        if (field_1C0_speak != NakedSligSpeak::Speak_None)
        {
            field_108_next_motion = NakedSligMotion::M_8_41BF70;
        }
    }

    switch (field_108_next_motion)
    {
    case -1: // TODO: None constant
        if (field_106_current_motion != NakedSligMotion::M_0_41B260)
        {
            field_108_next_motion = NakedSligMotion::M_0_41B260;
            Set_AnimAndMotion_419890(field_108_next_motion, TRUE);
            break;
        }
        break;

    case NakedSligMotion::M_0_41B260:
    case NakedSligMotion::M_1_TryToTransform_41B890:
    case NakedSligMotion::M_7_41C010:
    case NakedSligMotion::M_8_41BF70:
    case NakedSligMotion::M_11_TurnAround_41B590:
        Set_AnimAndMotion_419890(field_108_next_motion, TRUE);
        break;

    case NakedSligMotion::M_3_Crawling_41B280:
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

        if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(30), gridScale))
        {
            if (sControlledCharacter_5C1B8C == this)
            {
                field_108_next_motion = NakedSligMotion::M_16_PushWall_41B3C0;
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

void CC Slig_Sfx_4BFFE0(__int16 /*effect*/, BaseAliveGameObject* /*pObj*/)
{
    NOT_IMPLEMENTED();
}

__int16 NakedSlig::CanCrawl_41C5D0()
{
    NOT_IMPLEMENTED();

    field_C4_velx = dword_54471C[field_20_animation.field_92_current_frame];
    
    FP gridScale = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -field_C4_velx;
        gridScale = -gridScale;
    }

    field_C4_velx = (field_C4_velx * field_CC_sprite_scale);

    if (sControlledCharacter_5C1B8C == this && Raycast_408750(field_CC_sprite_scale * FP_FromInteger(30), gridScale))
    {
        field_1B0 = 0;
        field_C8_vely = FP_FromInteger(0);
        Set_AnimAndMotion_419890(10, TRUE);
        const int snappedX = SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos));
        field_C4_velx = ((FP_FromInteger(snappedX) - field_B8_xpos) / FP_FromInteger(4));
        // TODO
        //Sfx_Slig_4C04F0(Math_RandomRange_496AB0(13, 14), 0, 0, this);
        return 0;
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
                sub_41C500();
            }
        }
        else
        {
            vOnTrapDoorOpen_4081F0();
            field_F8_LastLineYPos = field_BC_ypos;
            if (field_106_current_motion == NakedSligMotion::M_3_Crawling_41B280)
            {
                Set_AnimAndMotion_419890(NakedSligMotion::M_4_41B620, TRUE);
            }
            field_1B0 = 0x10000;
            field_B8_xpos = field_B8_xpos + field_C4_velx;
        }
    }
    else
    {
        field_F8_LastLineYPos = field_BC_ypos;
        Set_AnimAndMotion_419890(NakedSligMotion::M_5_41B650, TRUE);
    }
}

void NakedSlig::sub_41C500()
{
    NOT_IMPLEMENTED();
}
