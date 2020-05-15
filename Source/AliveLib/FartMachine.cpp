#include "stdafx.h"
#include "FartMachine.hpp"
#include "Events.hpp"
#include "Function.hpp"

// TODO: Should be const but can't be due to mlgs hacks in Font obj
BYTE byte_550F08[32] =
{
    0u,
    0u,
    1u,
    128u,
    1u,
    132u,
    32u,
    132u,
    33u,
    128u,
    32u,
    132u,
    33u,
    132u,
    101u,
    206u,
    101u,
    140u,
    140u,
    177u,
    19u,
    148u,
    100u,
    206u,
    101u,
    206u,
    215u,
    152u,
    20u,
    161u,
    24u,
    216u
};

FartMachine* FartMachine::ctor_413060(Path_FartMachine* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x544424);
    SetVTable(&field_14C_anim, 0x544290);
    
    field_4_typeId = Types::eBrewMachine_13;

    field_F4_font_context.LoadFontType_433400(2);
    field_104_font.ctor_433590(3, byte_550F08, &field_F4_font_context);

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kUnknownResID_6016);
    Animation_Init_424E10(316, 19, 11, ppRes, 1, 1u);

    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kEvilFartResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID);

    field_DC_bApplyShadows &= ~1u;
    field_140_tlvInfo = tlvInfo;
    field_20_animation.field_C_render_layer = 23;
    field_1E4_remaining_brew_count = pTlv->field_10_num_brews;

    const BYTE savedBrewCount = pTlv->field_1_unknown;
    if (savedBrewCount == 0)
    {
        field_144_total_brew_count = field_1E4_remaining_brew_count;
    }
    else if (savedBrewCount > 30)
    {
        field_144_total_brew_count = 0;
    }
    else
    {
        field_144_total_brew_count = savedBrewCount;
    }

    field_13C_textX = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_0_x +  5) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));
    field_13E_textY = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_2_y + 10) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y));
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_1E6_cam_id = gMap_5C3030.sCurrentCamId_5C3034;
    return this;
}

BaseGameObject* FartMachine::VDestructor(signed int flags)
{
    return vdtor_413290(flags);
}

void FartMachine::VUpdate()
{
    vUpdate_4132C0();
}

void FartMachine::VRender(int** pOrderingTable)
{
    vRender_4133F0(pOrderingTable);
}

FartMachine* FartMachine::vdtor_413290(signed int flags)
{
    dtor_413330();
    if (flags & 1)
    {
        alive_delete_free(this);
    }
    return this;
}

void FartMachine::dtor_413330()
{
    SetVTable(this, 0x544424);
    Path::TLV_Reset_4DB8E0(field_140_tlvInfo, -1, 0, 0);
    field_104_font.dtor_433540();
    field_F4_font_context.dtor_433510();
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void FartMachine::vUpdate_4132C0()
{
    Path_FartMachine* pTlv = static_cast<Path_FartMachine*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_140_tlvInfo));
    if (field_144_total_brew_count > 0)
    {
        pTlv->field_1_unknown = static_cast<BYTE>(field_144_total_brew_count);
    }
    else
    {
        pTlv->field_1_unknown = 50;
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void FartMachine::vRender_4133F0(int** pOt)
{
    if (gMap_5C3030.sCurrentCamId_5C3034 == field_1E6_cam_id)
    {
        char text[12] = {};
        sprintf(text, "%02d", field_144_total_brew_count);
        const int textWidth = field_104_font.MeasureWidth_433700(text);
        short flickerAmount = 50;
        if (sDisableFontFlicker_5C9304)
        {
            flickerAmount = 0;
        }

        field_104_font.DrawString_4337D0(
            pOt,
            text,
            field_13C_textX,
            field_13E_textY,
            1,
            1,
            0,
            22,
            127,
            127,
            127,
            0,
            FP_FromInteger(1),
            field_13C_textX + textWidth,
            flickerAmount);

        const int v5 = 5 * textWidth;
        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            PsxToPCX(field_13C_textX),
            field_13E_textY,
            PsxToPCX(8 * v5),
            16,
            pScreenManager_5BB5F4->field_3A_idx);
    }

    BaseAnimatedWithPhysicsGameObject::VRender(pOt);
}
