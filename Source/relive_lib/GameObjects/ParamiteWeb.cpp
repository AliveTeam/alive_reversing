#include "stdafx.h"
#include "ParamiteWeb.hpp"
#include "../Function.hpp"
#include "ShadowZone.hpp"
#include "ScreenManager.hpp"
#include "Rope.hpp"
#include "../MapWrapper.hpp"
#include "../AnimationUnknown.hpp"
#include "../PsxDisplay.hpp"
#include "../GameType.hpp"

ParamiteWeb::ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eRope);

    mYOffset = 0;

    if (scale == FP_FromInteger(1))
    {
        mSegmentLength = 15;
    }
    else
    {
        mSegmentLength = 7;
    }

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ParamiteWeb));
    Animation_Init(GetAnimRes(AnimId::ParamiteWeb));

    if (scale == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_24);
        SetScale(Scale::Fg);
        GetAnimation().SetSpriteScale(scale);
        SetSpriteScale(scale);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_Half_5);
        SetScale(Scale::Bg);

        if (GetGameType() == GameType::eAe)
        {
            GetAnimation().SetSpriteScale(FP_FromDouble(0.7));
            SetSpriteScale(FP_FromDouble(0.7));
            xpos += FP_FromInteger(2);
        }
        else
        {
            GetAnimation().SetSpriteScale(scale);
            SetSpriteScale(scale);
        }
    }

    GetAnimation().SetRGB(128, 128, 128);

    mXPos = xpos;
    mTtlRemainder = top;
    mYPos = FP_FromInteger(top);
    mTtl = bottom;

    mSegmentsCount = 240 / mSegmentLength;

    mWebRes = relive_new AnimationUnknown[mSegmentsCount];
    if (mWebRes)
    {
        for (s32 i = 0; i < mSegmentsCount; i++)
        {
            AnimationUnknown* pSegment = &mWebRes[i];
            pSegment->SetRender(true);
            pSegment->mAnimPtr = &GetAnimation();
            pSegment->SetRenderLayer(GetAnimation().GetRenderLayer());
            pSegment->mSpriteScale = GetSpriteScale();
            pSegment->SetSemiTrans(false);
            pSegment->SetBlending(false);
        }
    }

    mEnabled = false;
}

ParamiteWeb::~ParamiteWeb()
{
    relive_delete[] mWebRes;
}

void ParamiteWeb::VUpdate()
{
    if (mEnabled)
    {
        mTtlRemainder -= 8;
        if (mTtlRemainder <= mTtl)
        {
            SetDead(true);
        }
    }
}

void ParamiteWeb::VScreenChanged()
{
    if (GetMap().LevelChanged() || GetMap().PathChanged())
    {
        SetDead(true);
    }
}

void ParamiteWeb::VRender(OrderingTable& ot)
{
    PSX_Point camCoords = {};
    GetMap().GetCurrentCamCoords(&camCoords);
    if (mCurrentLevel == GetMap().mCurrentLevel && mCurrentPath == GetMap().mCurrentPath)
    {
        if (mXPos >= FP_FromInteger(camCoords.x) && mXPos <= FP_FromInteger(camCoords.x + 1024))
        {
            const FP cam_x = gScreenManager->CamXPos();
            const FP cam_y = gScreenManager->CamYPos();

            s16 minY = FP_GetExponent(FP_FromInteger(mTtl) - cam_y);
            s16 maxY = FP_GetExponent(FP_FromInteger(mTtlRemainder) - cam_y);

            s32 ypos_int = FP_GetExponent(mYPos);
            if (ypos_int > mTtlRemainder)
            {
                ypos_int = mTtlRemainder + (ypos_int - mTtlRemainder) % mSegmentLength;
            }

            s16 x_start = FP_GetExponent(mXPos - cam_x);
            if (GetGameType() == GameType::eAo)
            {
                x_start = PsxToPCX<s16>(x_start);
            }

            s16 y_start = FP_GetExponent((FP_FromInteger(ypos_int)) - cam_y);
            if (y_start > 240)
            {
                y_start = y_start % mSegmentLength + 240;
                ypos_int = FP_GetExponent(cam_y + FP_FromInteger(y_start));
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

            if (y_start >= minY)
            {
                for (s32 idx = 0; idx < mSegmentsCount; ++idx)
                {
                    s16 r = 128;
                    s16 g = 128;
                    s16 b = 128;
                    ShadowZone::ShadowZones_Calculate_Colour(FP_GetExponent(mXPos), ypos_int - (idx * mSegmentLength), GetScale(), &r, &g, &b);
                    mWebRes[idx].SetRGB(r, g, b);
                    mWebRes[idx].VRender(x_start, y_start, ot, 0, 0);
                    ClipPoly_Vertically(&mWebRes[idx].mPoly, minY, maxY);
                    y_start -= mSegmentLength;
                    if (y_start < minY)
                    {
                        break;
                    }
                }
            }
        }
    }
}
