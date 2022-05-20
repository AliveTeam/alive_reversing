#include "stdafx_ao.h"
#include "Function.hpp"
#include "ShadowZone.hpp"
#include "stdlib.hpp"
#include "FixedPoint.hpp"
#include "Events.hpp"

namespace AO {

ALIVE_VAR(1, 0x507B08, DynamicArrayT<ShadowZone>*, sShadowZone_dArray_507B08, nullptr);

ShadowZone* ShadowZone::ctor_435D30(Path_ShadowZone* pTlv, Map* /*pMap*/, s32 tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BB728);

    sShadowZone_dArray_507B08->Push_Back(this);

    field_16_path = gMap.mCurrentPath;
    field_14_level = gMap.mCurrentLevel;

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

void CC ShadowZone::ShadowZones_Calculate_Colour_435FF0(s32 xpos, s32 ypos, s16 scale, s16* r, s16* g, s16* b)
{
    for (s32 idx = 0; idx < sShadowZone_dArray_507B08->Size(); idx++)
    {
        ShadowZone* pShadow = sShadowZone_dArray_507B08->ItemAt(idx);
        if (!pShadow)
        {
            break;
        }

        if (pShadow->ApplysToScale(scale))
        {
            FP amount = {};

            // TODO: This was probably a reference, refactor later
            pShadow->GetColourAmount_435E40(&amount, static_cast<s16>(xpos), static_cast<s16>(ypos));

            *r = FP_GetExponent(FP_FromInteger(*r) + (pShadow->field_28_r * amount));
            *b = FP_GetExponent(FP_FromInteger(*b) + (pShadow->field_30_b * amount));

            // NOTE: Never seems to be enabled, a debugging feature so instead of being hidden
            // in the shadow zones you appear green.
            //if (!word_5076F4))
            {
                *g = FP_GetExponent(FP_FromInteger(*g) + (pShadow->field_2C_g * amount));
            }
        }
    }

    // Clamp min/max
    if (*r > 255)
    {
        *r = 255;
    }

    if (*r < 0)
    {
        *r = 0;
    }

    if (*g > 255)
    {
        *g = 255;
    }

    if (*g < 0)
    {
        *g = 0;
    }

    if (*b > 255)
    {
        *b = 255;
    }

    if (*b < 0)
    {
        *b = 0;
    }
}

s16 ShadowZone::ApplysToScale(s16 scale)
{
    if (field_26_scale == ShadowZoneScale::eBoth_0)
    {
        return 1;
    }
    else if (field_26_scale == ShadowZoneScale::eFull_2 && scale == 1)
    {
        return 1;
    }
    else if (field_26_scale == ShadowZoneScale::eHalf_1 && scale == 0)
    {
        return 1;
    }
    return 0;
}

BaseGameObject* ShadowZone::dtor_435F10()
{
    SetVTable(this, 0x4BB728);
    gMap.TLV_Reset_446870(field_10_tlvInfo, -1, 0, 0);
    sShadowZone_dArray_507B08->Remove_Item(this);
    return dtor_487DF0();
}

BaseGameObject* ShadowZone::VDestructor(s32 flags)
{
    return Vdtor_436180(flags);
}

ShadowZone* ShadowZone::Vdtor_436180(s32 flags)
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
        mFlags.Set(BaseGameObject::eDead);
    }
}

void ShadowZone::VScreenChanged_435FE0()
{
    mFlags.Set(BaseGameObject::eDead);
}

void ShadowZone::GetColourAmount_435E40(FP* pOut, s16 xpos, s16 ypos)
{
    const s32 deltaX = abs(xpos - field_18_centre_x);
    const s32 deltaY = abs(ypos - field_1A_centre_y);

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

} // namespace AO
