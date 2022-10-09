#include "stdafx_ao.h"
#include "Function.hpp"
#include "Rope.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "PathData.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "Map.hpp"
#include "../relive_lib/AnimationUnknown.hpp"

namespace AO {

void ClipPoly_Vertically_4584B0(Poly_FT4* pPoly, s32 minY, s32 maxY)
{
    const s32 d1 = minY - Y0(pPoly);
    const s16 polyHeight = Y3(pPoly) - Y0(pPoly);
    if (d1 > 0 && d1 <= polyHeight)
    {
        SetXY0(pPoly, X0(pPoly), Y0(pPoly) + (s16) d1);
        SetXY1(pPoly, X1(pPoly), Y1(pPoly) + (s16) d1);

        SetUV0(pPoly, U0(pPoly), V0(pPoly) + (u8) d1);
        SetUV1(pPoly, U1(pPoly), V1(pPoly) + (u8) d1);
    }

    const s32 d2 = Y3(pPoly) - maxY;
    if (d2 > 0 && d2 <= polyHeight)
    {
        SetXY2(pPoly, X2(pPoly), Y2(pPoly) - (s16) d2);
        SetXY3(pPoly, X3(pPoly), Y3(pPoly) - (s16) d2);

        SetUV2(pPoly, U2(pPoly), V2(pPoly) - (u8) d2);
        SetUV3(pPoly, U3(pPoly), V3(pPoly) - (u8) d2);
    }
}

Rope::~Rope()
{
    relive_delete[] field_E8_pRopeRes;
}

Rope::Rope(s32 left, s32 top, s32 bottom, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eRope);

    mYOffset = 0;

    if (scale == FP_FromInteger(1))
    {
        field_E6_rope_length = 15;
    }
    else
    {
        field_E6_rope_length = 7;
    }

    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eRuptureFarms:
        case EReliveLevelIds::eDesert:
        case EReliveLevelIds::eDesertTemple:
        case EReliveLevelIds::eBoardRoom:
        case EReliveLevelIds::eRuptureFarmsReturn:
        case EReliveLevelIds::eDesertEscape:
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Rope_R1));
            Animation_Init(GetAnimRes(AnimId::Rope_R1));
            break;
        }

        default:
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Rope_Lines));
            Animation_Init(GetAnimRes(AnimId::Rope_Lines));
            break;
        }
    }
    mAnim.field_14_scale = scale;
    mSpriteScale = scale;
    if (scale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_RopeWebDrillMeatSaw_24;
        mScale = Scale::Fg;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_RopeWebDrillMeatSaw_Half_5;
        mScale = Scale::Bg;
    }

    mAnim.mRed = 128;
    mAnim.mGreen = 128;
    mAnim.mBlue = 128;

    field_F2_bottom = static_cast<s16>(bottom);
    field_E4_rope_segment_count = 240 / field_E6_rope_length + 1;

    mXPos = FP_FromInteger(left);
    mYPos = FP_FromInteger(bottom);
    field_EC_left = static_cast<s16>(left);
    field_F0_right = static_cast<s16>(left);
    field_EE_top = static_cast<s16>(top);

    field_E8_pRopeRes = relive_new AnimationUnknown[field_E4_rope_segment_count];
    if (field_E8_pRopeRes)
    {
        for (s32 i = 0; i < field_E4_rope_segment_count; i++)
        {
            AnimationUnknown* pSegment = &field_E8_pRopeRes[i];
            pSegment->mFlags.Set(AnimFlags::eRender);
            pSegment->field_68_anim_ptr = &mAnim;
            pSegment->mRenderLayer = mAnim.mRenderLayer;
            pSegment->field_6C_scale = scale;
            pSegment->mFlags.Clear(AnimFlags::eSemiTrans);
            pSegment->mFlags.Clear(AnimFlags::eBlending);
        }
    }
}

void Rope::VUpdate()
{
    // Empty
}

void Rope::VRender(PrimHeader** ppOt)
{
    PSX_Point camPos = {};
    gMap.GetCurrentCamCoords(&camPos);
    if (mCurrentLevel == gMap.mCurrentLevel)
    {
        if (mCurrentPath == gMap.mCurrentPath)
        {
            if (mXPos >= FP_FromInteger(camPos.x) && mXPos <= FP_FromInteger(camPos.x + 1024))
            {
                const FP camYPos = pScreenManager->mCamPos->y;

                s32 minY = FP_GetExponent((FP_FromInteger(pScreenManager->mCamYOff + field_EE_top))
                                          - camYPos);
                s32 maxY = FP_GetExponent((FP_FromInteger(pScreenManager->mCamYOff + field_F2_bottom))
                                          - camYPos);

                s16 ypos = FP_GetExponent(mYPos);
                if (ypos > field_F2_bottom)
                {
                    ypos = field_F2_bottom + ((ypos - field_F2_bottom) % field_E6_rope_length);
                }

                s16 screenX = PsxToPCX(
                    FP_GetExponent(mXPos + FP_FromInteger(pScreenManager->mCamXOff) - pScreenManager->mCamPos->x),
                    11);
                s16 screenY = FP_GetExponent(
                    (FP_FromInteger(pScreenManager->mCamYOff + ypos))
                    - camYPos);

                if (mYOffset + screenY > 240)
                {
                    screenY = screenY % field_E6_rope_length + 240;
                    ypos = FP_GetExponent(pScreenManager->mCamPos->y
                                          + FP_FromInteger(screenY - pScreenManager->mCamYOff));
                }
                if (minY < 0)
                {
                    minY = 0;
                }
                if (maxY > 240)
                {
                    maxY = 240;
                }

                mAnim.VRender(640, 240, ppOt, 0, 0);
                if (screenY >= minY)
                {
                    for (s32 idx = 0; idx < field_E4_rope_segment_count; idx++)
                    {
                        s16 r = 128;
                        s16 g = 128;
                        s16 b = 128;
                        ShadowZone::ShadowZones_Calculate_Colour(
                            FP_GetExponent(mXPos),
                            ypos - (idx * field_E6_rope_length),
                            mScale,
                            &r,
                            &g,
                            &b);

                        field_E8_pRopeRes[idx].mRed = static_cast<u8>(r);
                        field_E8_pRopeRes[idx].mGreen = static_cast<u8>(g);
                        field_E8_pRopeRes[idx].mBlue = static_cast<u8>(b);

                        field_E8_pRopeRes[idx].VRender(
                            screenX,
                            mYOffset + screenY,
                            ppOt, 0, 0);

                        PSX_RECT rect = {};
                        field_E8_pRopeRes[idx].GetRenderedSize(&rect);

                        ClipPoly_Vertically_4584B0(
                            &field_E8_pRopeRes[idx].field_10_polys[gPsxDisplay.mBufferIndex],
                            minY + mYOffset,
                            maxY + mYOffset);

                        screenY -= field_E6_rope_length;
                        if (screenY < minY)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}

} // namespace AO
