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
    &NakedSlig::M_1_41B890,                // Idle
    &NakedSlig::M_2_41BF00,                // Speak ??
    &NakedSlig::M_3_41B280,                // Crawling
    &NakedSlig::M_4_41B620,                // Beat/chant shake?
    &NakedSlig::M_5_41B650,                // Gibbed/explode
    &NakedSlig::M_6_41B870,                // Start to be hit or hit floor ??
    &NakedSlig::M_7_41C010,                // Idle not moving ??
    &NakedSlig::M_8_41BF70,                // Speak
    &NakedSlig::M_9_41BD80,                // Wake up
    &NakedSlig::M_10_41B400,               // Pushing into wall
    &NakedSlig::M_11_41B590,               // Turning
    &NakedSlig::M_12_418C30,               // Out of chant shake
    &NakedSlig::M_13_418C50,               // 
    &NakedSlig::M_14_41C040,               // Idle 2 ??
    &NakedSlig::M_15_41B600,               // Idle 3 ??
    &NakedSlig::M_16_41B3C0,               // Pushing into wall saying ow ?
    &NakedSlig::M_17_41B3A0                // Crawl to idle ??
});

struct ReimplToRealPair
{
    TNakedSligAIFn mNew;
    DWORD mReal;
};

const ReimplToRealPair sAiFns[] =
{
    { &NakedSlig::AI_0_419DE0, 0x419DE0 },
    { &NakedSlig::AI_1_419F60, 0x419F60 },
    { &NakedSlig::AI_2_419FE0, 0x419FE0 },
    { &NakedSlig::AI_3_41A5B0, 0x41A5B0 }, // Controlled by abe
    { &NakedSlig::AI_4_41A880, 0x41A880 },
    { &NakedSlig::AI_5_41ADF0, 0x41ADF0 },
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
    field_1B8 = 0;
    field_1C4 = 0;
    field_1C0 = -1;
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

    if (field_1E8_tlv.field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }
    else  if (field_1E8_tlv.field_10_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (field_1E8_tlv.field_14_state == 2)
    {
        Set_AnimAndMotion_419890(0, 1);
        SetBrain(&NakedSlig::AI_1_419F60);
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
        Set_AnimAndMotion_419890(9, 1);
        SetBrain(&NakedSlig::AI_0_419DE0);
    }

    field_208_brain_sub_state = 0;
    if (field_1E8_tlv.field_16_locker_direction == 2)
    {
        field_1E0 = NextRandom_4197D0();
    }
    else
    {
        field_1E0 = field_1E8_tlv.field_16_locker_direction;
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
            field_1B8 = 0;
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

void NakedSlig::SetBrain(TNakedSligAIFn fn)
{
#if _WIN32 || !_WIN64
    if (IsAlive())
    {
        // If not running as standalone set the address to be
        // the address of the real function rather than the reimpl as the real
        // game code compares the function pointer addresses (see IsBrain(x)).
        for (const auto& addrPair : sAiFns)
        {
            if (addrPair.mNew == fn)
            {
                const DWORD actualAddressToUse = addrPair.mReal;
                // Hack to overwrite the member function pointer bytes with arbitrary data
                memcpy(&field_204_brain_state, &actualAddressToUse, sizeof(DWORD));
                return;
            }
        }
    }
#endif
    field_204_brain_state = fn;
}

bool NakedSlig::BrainIs(TNakedSligAIFn fn)
{
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
    if (field_10C_health > FP_FromInteger(0) || field_1E8_tlv.field_1A_reset_on_death)
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

__int16 NakedSlig::AI_0_419DE0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 NakedSlig::AI_1_419F60()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 NakedSlig::AI_2_419FE0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 NakedSlig::AI_3_41A5B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 NakedSlig::AI_4_41A880()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 NakedSlig::AI_5_41ADF0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void NakedSlig::M_0_41B260()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_1_41B890()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_2_41BF00()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_3_41B280()
{
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

void NakedSlig::M_9_41BD80()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_10_41B400()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_11_41B590()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_12_418C30()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_13_418C50()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_14_41C040()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_15_41B600()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_16_41B3C0()
{
    NOT_IMPLEMENTED();
}

void NakedSlig::M_17_41B3A0()
{
    NOT_IMPLEMENTED();
}
