#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Blood.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Primitives_common.hpp"
#include <algorithm>

#undef min
#undef max

namespace AO {

Blood::Blood(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, s32 count)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetSpriteScale(scale);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BloodDrop));
    Animation_Init(GetAnimRes(AnimId::BloodDrop));

    GetAnimation().SetSemiTrans(false);
    GetAnimation().SetRGB(127, 127, 127);

    if (GetSpriteScale() == FP_FromInteger(1))
    {
        mOtLayer = Layer::eLayer_Foreground_36;
    }
    else
    {
        mOtLayer = Layer::eLayer_Foreground_Half_17;
    }

    if (GetSpriteScale() != FP_FromInteger(1))
    {
        GetAnimation().SetFrame((GetAnimation().Get_Frame_Count() >> 1) + 1);
    }

    mTotalBloodCount = static_cast<s16>(count);
    mCurrentBloodCount = static_cast<s16>(count);

    mBloodParticle = relive_new BloodParticle[count];
    if (mBloodParticle)
    {
        mUpdateCalls = 0;

        mXPos = xpos - FP_FromInteger(12);
        mYPos = ypos - FP_FromInteger(12);

        mBloodXPos = FP_GetExponent(xpos - FP_FromInteger(12) + FP_FromInteger(gScreenManager->mCamXOff) - gScreenManager->mCamPos->x);
        mBloodYPos = FP_GetExponent(ypos - FP_FromInteger(12) + FP_FromInteger(gScreenManager->mCamYOff) - gScreenManager->mCamPos->y);

        const u8 u0 = 0;// mAnim.mVramRect.x & 0x3F;
        const u8 v0 = 0;//mAnim.mVramRect.y & 0xFF;

        GetAnimation().SetBlending(true);

        for (s32 i = 0; i < mTotalBloodCount; i++)
        {
            for (s32 j = 0; j < 2; j++)
            {
                BloodParticle* pParticle = &mBloodParticle[i];
                Poly_FT4* pSprt = &pParticle->field_10_prims[j];
                PolyFT4_Init(pSprt);
                Poly_Set_SemiTrans(&pSprt->mBase.header, 1);

                if (GetAnimation().GetBlending())
                {
                    Poly_Set_Blending(&pSprt->mBase.header, 1);
                }
                else
                {
                    Poly_Set_Blending(&pSprt->mBase.header, 0);
                    const auto rgb = GetAnimation().GetRgb();
                    SetRGB0(pSprt, rgb.r, rgb.g, rgb.b);
                }

                pSprt->mAnim = &GetAnimation();

                SetUV0(pSprt, u0, v0);
            }
        }

        // Has its own random seed based on the frame counter.. no idea why
        mRandSeed = static_cast<u8>(sGnFrame);

        for (s32 i = 0; i < mCurrentBloodCount; i++)
        {
            mBloodParticle[i].x = FP_FromInteger(mBloodXPos);
            mBloodParticle[i].y = FP_FromInteger(mBloodYPos);

            const FP randX = (FP_FromInteger(gRandomBytes[mRandSeed++]) / FP_FromInteger(16));
            const FP adjustedX = FP_FromDouble(1.3) * (randX - FP_FromInteger(8));
            mBloodParticle[i].mOffX = GetSpriteScale() * (xOff + adjustedX);

            const FP randY = (FP_FromInteger(gRandomBytes[mRandSeed++]) / FP_FromInteger(16));
            const FP adjustedY = FP_FromDouble(1.3) * (randY - FP_FromInteger(8));
            mBloodParticle[i].mOffY = GetSpriteScale() * (yOff + adjustedY);
        }
    }
    else
    {
        SetDead(true);
    }
}

Blood::~Blood()
{
     relive_delete[] mBloodParticle;
}

void Blood::VUpdate()
{
    if (mUpdateCalls > 0)
    {
        if (mUpdateCalls > 5)
        {
            mCurrentBloodCount -= 10;
        }

        if (mCurrentBloodCount <= 0)
        {
            SetDead(true);
            mCurrentBloodCount = 0;
            return;
        }

        for (s32 i = 0; i < mCurrentBloodCount; i++)
        {
            mBloodParticle[i].mOffY += FP_FromDouble(1.8);

            mBloodParticle[i].mOffX = mBloodParticle[i].mOffX * FP_FromDouble(0.9);
            mBloodParticle[i].mOffY = mBloodParticle[i].mOffY * FP_FromDouble(0.9);

            mBloodParticle[i].x += mBloodParticle[i].mOffX;
            mBloodParticle[i].y += mBloodParticle[i].mOffY;
        }
    }

    mUpdateCalls++;
}

void Blood::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        for (s32 i = 0; i < mCurrentBloodCount; i++)
        {
            BloodParticle* pParticle = &mBloodParticle[i];
            Poly_FT4* pSprt = &pParticle->field_10_prims[gPsxDisplay.mBufferIndex];

            const u8 u0 = 0; // mAnim.mVramRect.x & 63;

            SetUV0(pSprt, u0, 0 /*static_cast<u8>(mAnim.mVramRect.y)*/);
            SetTPage(pSprt, static_cast<u16>(PSX_getTPage(TPageAbr::eBlend_0)));

            const PerFrameInfo* pFrameHeader = GetAnimation().Get_FrameHeader(-1);

            const s16 x0 = PsxToPCX(FP_GetExponent(pParticle->x));
            const s16 y0 = FP_GetExponent(pParticle->y);

            SetXYWH(pSprt, x0, y0, static_cast<s16>(pFrameHeader->mWidth - 1), static_cast<s16>(pFrameHeader->mHeight - 1));

            if (!GetAnimation().GetBlending())
            {
                const auto rgb = GetAnimation().GetRgb();
                SetRGB0(pSprt, rgb.r, rgb.g, rgb.b);
            }

            OrderingTable_Add(OtLayer(ppOt, mOtLayer), &pSprt->mBase.header);
        }
    }
}

void Blood::VScreenChanged()
{
    SetDead(true);
}

} // namespace AO
