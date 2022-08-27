#include "stdafx_ao.h"
#include "Function.hpp"
#include "ShadowZone.hpp"
#include "stdlib.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/Events.hpp"

namespace AO {

ALIVE_VAR(1, 0x507B08, DynamicArrayT<ShadowZone>*, sShadowZone_dArray_507B08, nullptr);

ShadowZone::ShadowZone(relive::Path_ShadowZone* pTlv, Map* /*pMap*/, const TLVUniqueId& tlvId)
    : BaseGameObject(TRUE, 0)
{
    sShadowZone_dArray_507B08->Push_Back(this);

    field_16_path = gMap.mCurrentPath;
    field_14_level = gMap.mCurrentLevel;

    field_10_tlvInfo = tlvId;

    field_20_mid_x = pTlv->Width() / 2;
    field_22_mid_y = pTlv->Height() / 2;

    field_18_centre_x = field_20_mid_x + pTlv->mTopLeftX;
    field_1A_centre_y = field_22_mid_y + pTlv->mTopLeftY;

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

    field_28_r = FP_FromInteger(pTlv->mRGB.r);
    field_2C_g = FP_FromInteger(pTlv->mRGB.g);
    field_30_b = FP_FromInteger(pTlv->mRGB.b);

    mScale = pTlv->mScale;
}

void ShadowZone::ShadowZones_Calculate_Colour(s32 xpos, s32 ypos, Scale scale, s16* r, s16* g, s16* b)
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
            pShadow->GetColourAmount(&amount, static_cast<s16>(xpos), static_cast<s16>(ypos));

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

ShadowZone::~ShadowZone()
{
    Path::TLV_Reset(field_10_tlvInfo, -1, 0, 0);
    sShadowZone_dArray_507B08->Remove_Item(this);
}

void ShadowZone::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

s16 ShadowZone::ApplysToScale(Scale scale)
{
    if (mScale == relive::Path_ShadowZone::Scale::eBoth)
    {
        return 1;
    }
    else if (mScale == relive::Path_ShadowZone::Scale::eFull && scale == Scale::Fg)
    {
        return 1;
    }
    else if (mScale == relive::Path_ShadowZone::Scale::eHalf && scale == Scale::Bg)
    {
        return 1;
    }
    return 0;
}

void ShadowZone::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ShadowZone::GetColourAmount(FP* pOut, s16 xpos, s16 ypos)
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
