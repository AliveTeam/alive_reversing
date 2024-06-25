#include "stdafx.h"
#include "Rope.hpp"
#include "../Function.hpp"
#include "../Animation.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"
#include "../MapWrapper.hpp"
#include "../AnimationUnknown.hpp"
#include "../PsxDisplay.hpp"
#include "../GameType.hpp"

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

void Rope::InitRopeAnimation()
{
    if (GetGameType() == GameType::eAo)
    {
        switch (GetMap().mCurrentLevel)
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
    }
    else
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AE_Rope));
        Animation_Init(GetAnimRes(AnimId::AE_Rope));
    }
}

Rope::~Rope()
{
    relive_delete[] mRopeAnim;
}

Rope::Rope(s32 left, s32 top, s32 bottom, FP scale)
    : ::BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eRope);

    mYOffset = 0;

    InitRopeAnimation();

    GetAnimation().SetSpriteScale(scale);
    SetSpriteScale(scale);

    if (scale == FP_FromInteger(1))
    {
        mRopeLength = 15;
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_24);
        SetScale(Scale::Fg);
    }
    else
    {
        mRopeLength = 7;
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_Half_5);
        SetScale(Scale::Bg);

        if (GetGameType() == GameType::eAe)
        {
            GetAnimation().SetSpriteScale(FP_FromDouble(0.7));
            SetSpriteScale(FP_FromDouble(0.7));
        }
    }

    // here we have a very tragic diff
    s16 rgb = GetGameType() == GameType::eAo ? 128 : 127;
    mRGB.SetRGB(rgb, rgb, rgb);
    GetAnimation().SetRGB(rgb, rgb, rgb);

    mTop = static_cast<s16>(top);
    mBottom = static_cast<s16>(bottom);

    mXPos = FP_FromInteger(left);
    mYPos = FP_FromInteger(bottom);

    mRopeSegmentCount = (240 / mRopeLength) + 1; // psx screen height

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
    GetMap().GetCurrentCamCoords(&camPos);
    // In the current level/map?
    if (mCurrentLevel == GetMap().mCurrentLevel && mCurrentPath == GetMap().mCurrentPath)
    {
        const s16 camXOffset = GetGameType() == GameType::eAo ? 1024 : 375;

        // In the current camera x range?
        if (mXPos >= FP_FromInteger(camPos.x) && mXPos <= FP_FromInteger(camPos.x + camXOffset))
        {
            const FP camXPos = gScreenManager->CamXPos();
            const FP camYPos = gScreenManager->CamYPos();

            s16 minY = FP_GetExponent(FP_FromInteger(mTop) - camYPos);
            s16 maxY = FP_GetExponent(FP_FromInteger(mBottom) - camYPos);

            s16 ypos = FP_GetExponent(mYPos);
            if (ypos > mBottom)
            {
                ypos = mBottom + ((ypos - mBottom) % mRopeLength);
            }

            s16 screenX = FP_GetExponent(mXPos - camXPos);
            if (GetGameType() == GameType::eAo)
            {
                screenX = PsxToPCX(screenX, 11);
            }

            s16 screenY = ypos - FP_GetExponent(camYPos);

            if (screenY > 240)
            {
                screenY = (screenY % mRopeLength) + 240;
                ypos = FP_GetExponent(camYPos + (FP_FromInteger(screenY)));
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
                    // Apply shadow to the segments colour
                    s16 r = mRGB.r;
                    s16 g = mRGB.g;
                    s16 b = mRGB.b;

                    ShadowZone::ShadowZones_Calculate_Colour(
                        FP_GetExponent(mXPos),
                        ypos - (idx * mRopeLength),
                        GetScale(),
                        &r,
                        &g,
                        &b);

                    mRopeAnim[idx].SetRGB(r, g, b);

                    // Render the segment
                    mRopeAnim[idx].VRender(screenX, screenY, ot, 0, 0);

                    ClipPoly_Vertically(
                        &mRopeAnim[idx].mPoly,
                        minY,
                        maxY);

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