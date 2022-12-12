#include "stdafx_ao.h"
#include "Function.hpp"
#include "ParamiteWeb.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Rope.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "../relive_lib/AnimationUnknown.hpp"

namespace AO {

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
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_Half_5);
        SetScale(Scale::Bg);
    }

    GetAnimation().SetSpriteScale(scale);
    SetSpriteScale(scale);

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
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

//TODO @ 100% - merge with Rope::vRender()
void ParamiteWeb::VRender(PrimHeader** ppOt)
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);
    if (mCurrentLevel == gMap.mCurrentLevel && mCurrentPath == gMap.mCurrentPath)
    {
        if (mXPos >= FP_FromInteger(camCoords.x) && mXPos <= FP_FromInteger(camCoords.x + 1024))
        {
            const FP cam_y = gScreenManager->mCamPos->y;
            const FP cam_x = gScreenManager->mCamPos->x;

            s16 minY = FP_GetExponent(FP_FromInteger(gScreenManager->mCamYOff + mTtl) - cam_y);
            s16 maxY = FP_GetExponent(FP_FromInteger(gScreenManager->mCamYOff + mTtlRemainder) - cam_y);

            s32 ypos_int = FP_GetExponent(mYPos);
            if (ypos_int > mTtlRemainder)
            {
                ypos_int = mTtlRemainder + (ypos_int - mTtlRemainder) % mSegmentLength;
            }

            const s16 x_start = PsxToPCX<s16>(FP_GetExponent(mXPos + FP_FromInteger(gScreenManager->mCamXOff) - cam_x));

            s16 y_start = FP_GetExponent(FP_FromInteger(gScreenManager->mCamYOff + ypos_int) - cam_y);
            if (mYOffset + y_start > 240)
            {
                y_start = y_start % mSegmentLength + 240;
                ypos_int = FP_GetExponent(cam_y + FP_FromInteger(y_start - gScreenManager->mCamYOff));
            }

            if (minY < 0)
            {
                minY = 0;
            }

            if (maxY > 240)
            {
                maxY = 240;
            }

            GetAnimation().VRender(640, 240, ppOt, 0, 0);

            if (y_start >= minY)
            {
                for (s32 idx = 0; idx < mSegmentsCount; ++idx)
                {
                    s16 r = 128;
                    s16 g = 128;
                    s16 b = 128;
                    ShadowZone::ShadowZones_Calculate_Colour(FP_GetExponent(mXPos), ypos_int - (idx * mSegmentLength), GetScale(), &r, &g, &b);
                    mWebRes[idx].SetRGB(r, g, b);
                    mWebRes[idx].VRender(x_start, y_start + mYOffset, ppOt, 0, 0);
                    ClipPoly_Vertically_4584B0(&mWebRes[idx].mPolys[gPsxDisplay.mBufferIndex], mYOffset + minY, mYOffset + maxY);
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

} // namespace AO
