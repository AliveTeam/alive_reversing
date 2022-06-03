#include "stdafx.h"
#include "ShadowZone.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Events.hpp"

ShadowZone::ShadowZone(Path_ShadowZone* pTlv, Path* /*pPath*/, u32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    sShadowZone_dArray_5C1B80->Push_Back(this);

    field_26_path = gMap.mCurrentPath;
    field_24_level = gMap.mCurrentLevel;

    field_20_tlvInfo = tlvInfo;

    field_30_mid_x = (pTlv->field_C_bottom_right.field_0_x - pTlv->field_8_top_left.field_0_x) / 2;
    field_32_mid_y = (pTlv->field_C_bottom_right.field_2_y - pTlv->field_8_top_left.field_2_y) / 2;

    field_28_centre_x = field_30_mid_x + pTlv->field_8_top_left.field_0_x;
    field_2C_centre_mid_x = field_30_mid_x - 75;
    if (field_2C_centre_mid_x < 0)
    {
        field_2C_centre_mid_x = 0;
    }

    field_2A_center_y = field_32_mid_y + pTlv->field_8_top_left.field_2_y;
    field_2E_centre_mid_y = field_32_mid_y - 75;
    if (field_2E_centre_mid_y < 0)
    {
        field_2E_centre_mid_y = 0;
    }

    field_38_r = FP_FromInteger(pTlv->field_14_r);
    field_3C_g = FP_FromInteger(pTlv->field_16_g);
    field_40_b = FP_FromInteger(pTlv->field_18_b);

    field_34_switch_id = pTlv->field_1A_switch_id;
    field_36_scale = pTlv->field_1C_scale;
}

void ShadowZone::ShadowZones_Calculate_Colour(s32 xpos, s32 ypos, s16 scale, s16* r, s16* g, s16* b)
{
    for (s32 idx = 0; idx < sShadowZone_dArray_5C1B80->Size(); idx++)
    {
        ShadowZone* pShadow = sShadowZone_dArray_5C1B80->ItemAt(idx);
        if (!pShadow)
        {
            break;
        }

        if (pShadow->ApplysToScale(scale))
        {
            FP amount = {};
            // TODO: This was probably a reference, refactor later
            pShadow->GetColourAmount(&amount, static_cast<s16>(xpos), static_cast<s16>(ypos));

            *r = FP_GetExponent(FP_FromInteger(*r) + (pShadow->field_38_r * amount));
            *b = FP_GetExponent(FP_FromInteger(*b) + (pShadow->field_40_b * amount));

            // NOTE: Never seems to be enabled, a debugging feature so instead of being hidden
            // in the shadow zones you appear green.
            //if (!word_5C1BE2)
            {
                *g = FP_GetExponent(FP_FromInteger(*g) + (pShadow->field_3C_g * amount));
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

ShadowZone::~ShadowZone()
{
    Path::TLV_Reset(field_20_tlvInfo, -1, 0, 0);
    sShadowZone_dArray_5C1B80->Remove_Item(this);
}

void ShadowZone::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

s16 ShadowZone::ApplysToScale(s16 scale)
{
    if (field_36_scale == ShadowZoneScale::eBoth_0)
    {
        return 1;
    }
    else if (field_36_scale == ShadowZoneScale::eFull_2 && scale == 1)
    {
        return 1;
    }
    else if (field_36_scale == ShadowZoneScale::eHalf_1 && scale == 0)
    {
        return 1;
    }
    return 0;
}

void ShadowZone::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

FP* ShadowZone::GetColourAmount(FP* pOut, s16 xpos, s16 ypos)
{
    const s32 deltaX = abs(xpos - field_28_centre_x);
    const s32 deltaY = abs(ypos - field_2A_center_y);

    if (deltaX > field_30_mid_x || deltaY > field_32_mid_y)
    {
        // Not in range
        *pOut = FP_FromInteger(0);
    }
    else
    {
        if (deltaX > field_2C_centre_mid_x)
        {
            // Partly in range
            const FP midXDistance = FP_FromInteger(field_30_mid_x - field_2C_centre_mid_x);
            const FP value = midXDistance - FP_FromInteger(deltaX - field_2C_centre_mid_x);

            *pOut = value / midXDistance;
        }
        else
        {
            // Fully in range
            *pOut = FP_FromInteger(1);
        }
    }
    return pOut;
}
