#include "stdafx.h"
#include "Rope.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Animation.hpp"
#include "stdlib.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "Map.hpp"
#include "../relive_lib/AnimationUnknown.hpp"
#include "../relive_lib/PsxDisplay.hpp"

static const TintEntry kRopeTints[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

void ClipPoly_Vertically(Poly_FT4* pPoly, s32 minY, s32 maxY)
{
    const s32 d1 = minY - pPoly->Y0();
    const s16 polyHeight = pPoly->Y3() - pPoly->Y0();
    if (minY - pPoly->Y0() > 0 && d1 <= polyHeight)
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

Rope::Rope(s32 left, s32 top, s32 bottom, FP scale)
    : ::BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eRope);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AE_Rope));
    Animation_Init(GetAnimRes(AnimId::AE_Rope));

    SetTint(kRopeTints, gMap.mCurrentLevel);

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
        GetAnimation().SetSpriteScale(FP_FromDouble(0.7));
        SetSpriteScale(FP_FromDouble(0.7));
        SetScale(Scale::Bg);
    };

    GetAnimation().SetRGB(mRGB.r, mRGB.g, mRGB.b);

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
    // nullsub@4A11E0
}

Rope::~Rope()
{
    relive_delete[] mRopeAnim;
}

void Rope::VRender(OrderingTable& ot)
{
    PSX_Point camPos = {};
    gMap.GetCurrentCamCoords(&camPos);
    // In the current level/map?
    if (mCurrentLevel == gMap.mCurrentLevel && mCurrentPath == gMap.mCurrentPath)
    {
        // In the current camera x range?
        if (mXPos >= FP_FromInteger(camPos.x) && mXPos <= FP_FromInteger(camPos.x + 375))
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
                    mRopeAnim[idx].VRender(
                        screenX,
                        screenY,
                        ot,
                        0,
                        0);

                    ClipPoly_Vertically(
                        &mRopeAnim[idx].mPolys[gPsxDisplay.mBufferIndex],
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
