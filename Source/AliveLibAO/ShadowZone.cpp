#include "stdafx_ao.h"
#include "Function.hpp"
#include "ShadowZone.hpp"
#include "stdlib.hpp"
#include "FixedPoint.hpp"
#include "Events.hpp"

START_NS_AO

ALIVE_VAR(1, 0x507B08, DynamicArrayT<ShadowZone>*, sShadowZone_dArray_507B08, nullptr);

ShadowZone* ShadowZone::ctor_435D30(Path_ShadowZone* pTlv, Map* /*pMap*/, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BB728);

    sShadowZone_dArray_507B08->Push_Back(this);

    field_16_path = gMap_507BA8.field_2_current_path;
    field_14_level = gMap_507BA8.field_0_current_level;

    field_10_tlvInfo = tlvInfo;

    field_20_mid_x = (pTlv->field_14_bottom_right.field_0_x - pTlv->field_10_top_left.field_0_x) / 2;
    field_22_mid_y = (pTlv->field_14_bottom_right.field_2_y - pTlv->field_10_top_left.field_2_y) / 2;

    field_18_centre_x = field_20_mid_x + pTlv->field_10_top_left.field_0_x;
    field_1A_centre_y = field_22_mid_y + pTlv->field_10_top_left.field_2_y;

    field_1C_centre_mid_x = field_20_mid_x - 75;
    if (field_1C_centre_mid_x < 0)
    {
        field_1C_centre_mid_x = 0;
    }

    field_1E_centre_mid_y = field_22_mid_y - 75;
    if (field_1E_centre_mid_y < 0)
    {
        field_1E_centre_mid_y = 0;
    }

    field_28_r = FP_FromInteger(pTlv->field_1C_r);
    field_2C_g = FP_FromInteger(pTlv->field_1E_g);
    field_30_b = FP_FromInteger(pTlv->field_20_b);

    field_24_id = pTlv->field_22_id;
    field_26_scale = pTlv->field_24_scale;

    return this;
}

BaseGameObject* ShadowZone::dtor_435F10()
{
    SetVTable(this, 0x4BB728);
    gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, -1, 0, 0);
    sShadowZone_dArray_507B08->Remove_Item(this);
    return dtor_487DF0();
}

BaseGameObject* ShadowZone::VDestructor(signed int flags)
{
    return Vdtor_436180(flags);
}

ShadowZone* ShadowZone::Vdtor_436180(signed int flags)
{
    dtor_435F10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void ShadowZone::VScreenChanged()
{
    VScreenChanged_435FE0();
}

void ShadowZone::VUpdate()
{
    VUpdate_435F80();
}

void ShadowZone::VUpdate_435F80()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void ShadowZone::VScreenChanged_435FE0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void ShadowZone::GetColourAmount_435E40(FP* pOut, __int16 xpos, __int16 ypos)
{
    const int deltaX = abs(xpos - field_18_centre_x);
    const int deltaY = abs(ypos - field_1A_centre_y);

    if (deltaX > field_20_mid_x || deltaY > field_22_mid_y)
    {
        // Not in range
        *pOut = FP_FromInteger(0);
    }
    else
    {
        if (deltaX > field_1C_centre_mid_x)
        {
            // Partly in range
            const FP midXDistance = FP_FromInteger(field_20_mid_x - field_1C_centre_mid_x);
            const FP value = midXDistance - FP_FromInteger(deltaX - field_1C_centre_mid_x);
            *pOut = value / midXDistance;

        }
        else
        {
            // Fully in range
            *pOut = FP_FromInteger(1);
        }
    }
}

END_NS_AO
