#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Rope.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "PathData.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "Map.hpp"
#include "../relive_lib/AnimationUnknown.hpp"

namespace AO {

void ClipPoly_Vertically(Poly_FT4* pPoly, s32 minY, s32 maxY)
{
    const s32 d1 = minY - pPoly->Y0();
    const s16 polyHeight = pPoly->Y3() - pPoly->Y0();
    if (d1 > 0 && d1 <= polyHeight)
    {
        pPoly->SetXY0(pPoly->X0(), pPoly->Y0() + (s16) d1);
        pPoly->SetXY1(pPoly->X1(), pPoly->Y1() + (s16) d1);

        pPoly->SetUV0(pPoly->U0(), pPoly->V0() + (u8) d1);
        pPoly->SetUV1(pPoly->U1(), pPoly->V1() + (u8) d1);
    }

    const s32 d2 = pPoly->Y3() - maxY;
    if (d2 > 0 && d2 <= polyHeight)
    {
        pPoly->SetXY2(pPoly->X2(), pPoly->Y2() - (s16) d2);
        pPoly->SetXY3(pPoly->X3(), pPoly->Y3() - (s16) d2);

        pPoly->SetUV2(pPoly->U2(), pPoly->V2() - (u8) d2);
        pPoly->SetUV3(pPoly->U3(), pPoly->V3() - (u8) d2);
    }
}

Rope::~Rope()
{
    relive_delete[] mRopeAnim;
}

Rope::Rope(s32 left, s32 top, s32 bottom, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eRope);

    mYOffset = 0;

    if (scale == FP_FromInteger(1))
    {
        mRopeLength = 15;
    }
    else
    {
        mRopeLength = 7;
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
    GetAnimation().SetSpriteScale(scale);
    SetSpriteScale(scale);
    if (scale == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_24);
        SetScale(Scale::Fg);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_Half_5);
        SetScale(Scale::Bg);
    }

    GetAnimation().SetRGB(128, 128, 128);

    mBottom = static_cast<s16>(bottom);
    mRopeSegmentCount = 240 / mRopeLength + 1;

    mXPos = FP_FromInteger(left);
    mYPos = FP_FromInteger(bottom);
    mTop = static_cast<s16>(top);

    mRopeAnim = relive_new AnimationUnknown[mRopeSegmentCount];
    if (mRopeAnim)
    {
        for (s32 i = 0; i < mRopeSegmentCount; i++)
        {
            AnimationUnknown* pSegment = &mRopeAnim[i];
            pSegment->SetRender(true);
            pSegment->mAnimPtr = &GetAnimation();
            pSegment->SetRenderLayer(GetAnimation().GetRenderLayer());
            pSegment->mSpriteScale = scale;
            pSegment->SetSemiTrans(false);
            pSegment->SetBlending(false);
        }
    }
}

void Rope::VUpdate()
{
    // Empty
}

void Rope::VRender(OrderingTable& ot)
{
    PSX_Point camPos = {};
    gMap.GetCurrentCamCoords(&camPos);
    if (mCurrentLevel == gMap.mCurrentLevel)
    {
        if (mCurrentPath == gMap.mCurrentPath)
        {
            if (mXPos >= FP_FromInteger(camPos.x) && mXPos <= FP_FromInteger(camPos.x + 1024))
            {
                const FP camYPos = gScreenManager->mCamPos->y;

                s32 minY = FP_GetExponent((FP_FromInteger(gScreenManager->mCamYOff + mTop))
                                          - camYPos);
                s32 maxY = FP_GetExponent((FP_FromInteger(gScreenManager->mCamYOff + mBottom))
                                          - camYPos);

                s16 ypos = FP_GetExponent(mYPos);
                if (ypos > mBottom)
                {
                    ypos = mBottom + ((ypos - mBottom) % mRopeLength);
                }

                s16 screenX = PsxToPCX(
                    FP_GetExponent(mXPos + FP_FromInteger(gScreenManager->mCamXOff) - gScreenManager->mCamPos->x),
                    11);
                s16 screenY = FP_GetExponent(
                    (FP_FromInteger(gScreenManager->mCamYOff + ypos))
                    - camYPos);

                if (mYOffset + screenY > 240)
                {
                    screenY = screenY % mRopeLength + 240;
                    ypos = FP_GetExponent(gScreenManager->mCamPos->y
                                          + FP_FromInteger(screenY - gScreenManager->mCamYOff));
                }
                if (minY < 0)
                {
                    minY = 0;
                }
                if (maxY > 240)
                {
                    maxY = 240;
                }

                GetAnimation().VRender(640, 240, ot, 0, 0);
                if (screenY >= minY)
                {
                    for (s32 idx = 0; idx < mRopeSegmentCount; idx++)
                    {
                        s16 r = 128;
                        s16 g = 128;
                        s16 b = 128;
                        ShadowZone::ShadowZones_Calculate_Colour(
                            FP_GetExponent(mXPos),
                            ypos - (idx * mRopeLength),
                            GetScale(),
                            &r,
                            &g,
                            &b);

                        mRopeAnim[idx].SetRGB(r, g, b);

                        mRopeAnim[idx].VRender(
                            screenX,
                            mYOffset + screenY,
                            ot, 0, 0);

                        PSX_RECT rect = {};
                        mRopeAnim[idx].GetRenderedSize(&rect);

                        ClipPoly_Vertically(
                            &mRopeAnim[idx].mPolys[gPsxDisplay.mBufferIndex],
                            minY + mYOffset,
                            maxY + mYOffset);

                        screenY -= mRopeLength;
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
