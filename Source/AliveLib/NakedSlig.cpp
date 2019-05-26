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
        sub_419890(0, 1);
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
        sub_419890(9, 1);
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

signed __int16 NakedSlig::sub_419890(__int16 /*currentMotion*/, __int16 /*bClearNextMotion*/)
{
    NOT_IMPLEMENTED();
    return 1;
}

__int16 CC NakedSlig::NextRandom_4197D0()
{
    return (Math_NextRandom() & 1) == 0;
}

struct ReimplToRealPair
{
    TNakedSligAIFn mNew;
    DWORD mReal;
};

const ReimplToRealPair sAiFns[] =
{
    { &NakedSlig::AI_0_419DE0, 0x419DE0 },
    { &NakedSlig::AI_1_419F60, 0x419F60 }
};

void NakedSlig::SetBrain(TNakedSligAIFn fn)
{
#if _WIN32 || !_WIN64
    if (!IsAlive())
    {
        // If not running as standalone set the address to be
        // the address of the real function rather than the reimpl as the real
        // game code compares the function pointer addresses (see IsBrain(x)).
        for (const auto& addrPair : sAiFns)
        {
            if (addrPair.mNew == fn)
            {
                const DWORD actualAddressToUse = addrPair.mReal;
                // Hack to overwrite the memeber function pointer bytes with arbitry data
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
