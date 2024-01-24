//#include "stdafx_ao.h"
#include "ShadowZone.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "FixedPoint.hpp"
#include "Events.hpp"
#include "FixedPoint.hpp"

static DynamicArrayT<ShadowZone>* sShadowZoneArray;

void ShadowZone::MakeArray()
{
    sShadowZoneArray = relive_new DynamicArrayT<ShadowZone>(4);
}

void ShadowZone::FreeArray()
{
    relive_delete sShadowZoneArray;
    sShadowZoneArray = nullptr;
}

ShadowZone::ShadowZone(relive::Path_ShadowZone* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    sShadowZoneArray->Push_Back(this);

    mTlvId = tlvId;

    mMidX = pTlv->Width() / 2;
    mMidY = pTlv->Height() / 2;

    mCentreX = mMidX + pTlv->mTopLeftX;
    mCentreY = mMidY + pTlv->mTopLeftY;

    mCentreMidX = mMidX - 75;
    if (mCentreMidX < 0)
    {
        mCentreMidX = 0;
    }

    mCentreMidY = mMidY - 75;
    if (mCentreMidY < 0)
    {
        mCentreMidY = 0;
    }

    mRed = FP_FromInteger(pTlv->mRGB.r);
    mGreen = FP_FromInteger(pTlv->mRGB.g);
    mBlue = FP_FromInteger(pTlv->mRGB.b);

    mScale = pTlv->mScale;
}

void ShadowZone::ShadowZones_Calculate_Colour(s32 xpos, s32 ypos, Scale scale, s16* r, s16* g, s16* b)
{
    for (s32 idx = 0; idx < sShadowZoneArray->Size(); idx++)
    {
        ShadowZone* pShadow = sShadowZoneArray->ItemAt(idx);
        if (!pShadow)
        {
            break;
        }

        if (pShadow->ApplysToScale(scale))
        {
            FP amount = {};

            // TODO: This was probably a reference, refactor later
            pShadow->GetColourAmount(&amount, static_cast<s16>(xpos), static_cast<s16>(ypos));

            *r = FP_GetExponent(FP_FromInteger(*r) + (pShadow->mRed * amount));
            *b = FP_GetExponent(FP_FromInteger(*b) + (pShadow->mBlue * amount));

            // NOTE: Never seems to be enabled, a debugging feature so instead of being hidden
            // in the shadow zones you appear green.
            //if (!word_5076F4))
            {
                *g = FP_GetExponent(FP_FromInteger(*g) + (pShadow->mGreen * amount));
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
    GetMap().TLV_Reset(mTlvId);
    sShadowZoneArray->Remove_Item(this);
}

void ShadowZone::VScreenChanged()
{
    SetDead(true);
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
        SetDead(true);
    }
}

void ShadowZone::GetColourAmount(FP* pOut, s16 xpos, s16 ypos)
{
    const s32 deltaX = abs(xpos - mCentreX);
    const s32 deltaY = abs(ypos - mCentreY);

    if (deltaX > mMidX || deltaY > mMidY)
    {
        // Not in range
        *pOut = FP_FromInteger(0);
    }
    else
    {
        if (deltaX > mCentreMidX)
        {
            // Partly in range
            const FP midXDistance = FP_FromInteger(mMidX - mCentreMidX);
            const FP value = midXDistance - FP_FromInteger(deltaX - mCentreMidX);

            *pOut = value / midXDistance;
        }
        else
        {
            // Fully in range
            *pOut = FP_FromInteger(1);
        }
    }
}
