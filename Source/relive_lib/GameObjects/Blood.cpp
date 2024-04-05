#include "stdafx.h"
#include "../Function.hpp"
#include "Blood.hpp"
#include "ScreenManager.hpp"
#include "Math.hpp"
#include "../MapWrapper.hpp"
#include "../PsxDisplay.hpp"
#include "../../AliveLibAE/stdlib.hpp"
#include "../Primitives.hpp"
#include <algorithm>
#include "../GameType.hpp"

#undef min
#undef max

Blood::Blood(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, s32 count)
    : BaseAnimatedWithPhysicsGameObject(0), 
    mTotalBloodCount(count),
    mCurrentBloodCount(count),
    mBloodParticle(relive_new BloodParticle[count])
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

    if (GetSpriteScale() != FP_FromInteger(1) && GetGameType() == GameType::eAo)
    {
        GetAnimation().SetFrame((GetAnimation().Get_Frame_Count() / 2) + 1);
    }

    if (mBloodParticle)
    {
        mXPos = xpos - FP_FromInteger(12);
        mYPos = ypos - FP_FromInteger(12);

        mBloodXPos = FP_GetExponent(xpos - FP_FromInteger(12) - gScreenManager->CamXPos());
        mBloodYPos = FP_GetExponent(ypos - FP_FromInteger(12) - gScreenManager->CamYPos());

        const u8 u0 = 0;// mAnim.mVramRect.x & 0x3F;
        const u8 v0 = 0;//mAnim.mVramRect.y & 0xFF;

        GetAnimation().SetBlending(true);

        for (s32 i = 0; i < mTotalBloodCount; i++)
        {
            BloodParticle* pParticle = &mBloodParticle[i];
            Poly_FT4* pPoly = &pParticle->mPoly;
            pPoly->SetSemiTransparent(true);

            if (GetAnimation().GetBlending())
            {
                pPoly->SetShadeTex(true);
            }
            else
            {
                pPoly->SetShadeTex(false);
                const auto rgb = GetAnimation().GetRgb();
                pPoly->SetRGB0(rgb.r & 0xFF, rgb.g & 0xFF, rgb.b & 0xFF);
            }

            pPoly->mAnim = &GetAnimation();

            pPoly->SetUV0(u0, v0);
        }

        // Has its own random seed based on the frame counter.. no idea why
        mRandSeed = static_cast<u8>(sGnFrame);

        for (s32 i = 0; i < mCurrentBloodCount; i++)
        {
            mBloodParticle[i].x = FP_FromInteger(mBloodXPos);
            mBloodParticle[i].y = FP_FromInteger(mBloodYPos);

            const FP randX = FP_FromInteger(gRandomBytes[mRandSeed++]) / FP_FromInteger(16);
            const FP adjustedX = FP_FromDouble(1.3) * (randX - FP_FromInteger(8));
            mBloodParticle[i].mOffX = GetSpriteScale() * (xOff + adjustedX);

            const FP randY = FP_FromInteger(gRandomBytes[mRandSeed++]) / FP_FromInteger(16);
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

void Blood::VRender(OrderingTable& ot)
{
    if (GetMap().Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        for (s32 i = 0; i < mCurrentBloodCount; i++)
        {
            BloodParticle* pParticle = &mBloodParticle[i];
            Poly_FT4* pPoly = &pParticle->mPoly;

            pPoly->SetUV0(0, 0);
            pPoly->SetBlendMode(relive::TBlendModes::eBlend_0);

            const PerFrameInfo* pFrameHeader = GetAnimation().Get_FrameHeader(-1);

            const s16 x0 = PsxToPCX(FP_GetExponent(pParticle->x));
            const s16 y0 = FP_GetExponent(pParticle->y);

            pPoly->SetXYWH(x0, y0, static_cast<s16>(pFrameHeader->mWidth - 1), static_cast<s16>(pFrameHeader->mHeight - 1));

            if (!GetAnimation().GetBlending())
            {
                const auto rgb = GetAnimation().GetRgb();
                pPoly->SetRGB0(rgb.r & 0xFF, rgb.g & 0xFF, rgb.b & 0xFF);
            }

            ot.Add(mOtLayer, pPoly);
        }
    }
}

void Blood::VScreenChanged()
{
    SetDead(true);
}
