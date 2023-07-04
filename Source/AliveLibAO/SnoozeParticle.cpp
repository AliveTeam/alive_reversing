#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "SnoozeParticle.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Math.hpp"
#include "../relive_lib/Events.hpp"
#include "CameraSwapper.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

static const s16 xPositionDeltaEntries[36] = {
    1,
    0,
    0,
    1,
    0,
    0,
    1,
    0,
    1,
    0,
    1,
    1,
    1,
    0,
    1,
    0,
    1,
    0,
    0,
    1,
    0,
    0,
    0,
    -1,
    0,
    -1,
    0,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0,
    -1,
    0};

static const PSX_Point explosionVerts[6][2] = {
    {{-3, -4}, {-6, -7}},
    {{3, -4}, {6, -7}},
    {{4, -1}, {7, -1}},
    {{-4, 1}, {-7, 1}},
    {{-3, 4}, {-6, 7}},
    {{3, 4}, {6, 7}} };

static const s16 zVerts[8] = {
    -4,
    -4,
    4,
    -4,
    -4,
    4,
    4,
    4 };

SnoozeParticle::SnoozeParticle(FP xpos, FP ypos, Layer layer, FP scale)
    : BaseGameObject(true, 0)
{
    SetDrawable(true);

    SetType(ReliveTypes::eSnoozParticle);
    gObjListDrawables->Push_Back(this);

    mStartY = ypos;
    mXPos = xpos;
    mYPos = ypos;

    mDestY = FP_FromDouble(0.15);

    mDestY = (mDestY * FP_FromInteger(Math_NextRandom())) / FP_FromInteger(256);
    mDestY += FP_FromDouble(0.35);
    mDestY = mDestY * FP_FromInteger(-1);

    mOtLayer = layer;

    mSpriteScale = (scale * FP_FromDouble(0.4));

    mScaleDx = FP_FromDouble(0.30);
    mScaleDx = mScaleDx / (FP_FromInteger(20) / -mDestY);

    mRGB.SetRGB(0, 0, 0);
    mState = SnoozeParticleState::eRising_0;
    mBlowUp = false;
    mIdx = Math_NextRandom() % 36;
    mDestX = FP_FromInteger(xPositionDeltaEntries[mIdx]);
    mIdx++;
}

SnoozeParticle::~SnoozeParticle()
{
    if (GetDrawable())
    {
        gObjListDrawables->Remove_Item(this);
    }
}

void SnoozeParticle::VScreenChanged()
{
    SetDead(true);
}

void SnoozeParticle::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!gNumCamSwappers)
    {
        switch (mState)
        {
            case SnoozeParticleState::eRising_0:
                if (mYPos >= mStartY - FP_FromInteger(20))
                {
                    if (mRGB.r < 70 && mRGB.g < 70 && mRGB.b < 20)
                    {
                        mRGB.r += 14;
                        mRGB.g += 14;
                        mRGB.b += 4;
                    }

                    mSpriteScale += mScaleDx;

                    if (mIdx >= ALIVE_COUNTOF(xPositionDeltaEntries))
                    {
                        mIdx = 0;
                    }

                    const FP idx_toFP = FP_FromInteger(xPositionDeltaEntries[mIdx]);
                    mDestX = idx_toFP;
                    mXPos += idx_toFP;
                    mYPos += mDestY;
                    mIdx++;
                }
                else
                {
                    mState = SnoozeParticleState::eBlowingUp_2;
                }
                break;

            case SnoozeParticleState::eUnused_1:
                break;

            case SnoozeParticleState::eBlowingUp_2:
                mRGB.r /= 2;
                mRGB.g /= 2;
                mRGB.b /= 2;
                mXPos += mDestX;
                mYPos += mDestY;

                if (mBlowUp)
                {
                    SfxPlayMono(relive::SoundEffects::ZPop, 0);
                    SetDead(true);
                }
                else
                {
                    mBlowUp = true;
                }
        }
    }
}

void SnoozeParticle::VRender(BasePrimitive** ppOt)
{
    //Identical to AE except xInScreen, yInScreen are offset by gScreenManager positions
    FP_Point* pCamPos = gScreenManager->mCamPos;
    const s16 bufIdx = gPsxDisplay.mBufferIndex;

    if (mState == SnoozeParticleState::eBlowingUp_2)
    {
        const s16 xInScreen = FP_GetExponent(mXPos - pCamPos->x) + gScreenManager->mCamXOff;
        const s16 yInScreen = FP_GetExponent(mYPos - pCamPos->y) + gScreenManager->mCamYOff;

        for (s32 i = 0; i < ALIVE_COUNTOF(explosionVerts); i++)
        {
            Line_G2* pZExplosionLine = &mG2Lines[bufIdx][i];

            const s32 scaledLineRelativeStartX = FP_GetExponent(FP_FromInteger(explosionVerts[i][0].x) * mSpriteScale);
            const s32 scaledLineRelativeStartY = FP_GetExponent(FP_FromInteger(explosionVerts[i][0].y) * mSpriteScale);
            const s32 scaledLineRelativeEndX = FP_GetExponent(FP_FromInteger(explosionVerts[i][1].x) * mSpriteScale);
            const s32 scaledLineRelativeEndY = FP_GetExponent(FP_FromInteger(explosionVerts[i][1].y) * mSpriteScale);
            pZExplosionLine->SetXY0(
                   static_cast<s16>(PsxToPCX(xInScreen + scaledLineRelativeStartX, 11)),
                   static_cast<s16>(yInScreen + scaledLineRelativeStartY));
            pZExplosionLine->SetXY1(
                   static_cast<s16>(PsxToPCX(xInScreen + scaledLineRelativeEndX, 11)),
                   static_cast<s16>(yInScreen + scaledLineRelativeEndY));

            pZExplosionLine->SetRGB0(
                    static_cast<u8>(mRGB.r / 2),
                    static_cast<u8>(mRGB.g / 2),
                    static_cast<u8>(mRGB.b / 2));
            pZExplosionLine->SetRGB1(
                    static_cast<u8>(mRGB.r),
                    static_cast<u8>(mRGB.g),
                    static_cast<u8>(mRGB.b));

            pZExplosionLine->SetSemiTransparent(true);
            OrderingTable_Add(OtLayer(ppOt, mOtLayer), pZExplosionLine);
        }
    }
    else
    {
        Line_G4* pZLine = &mG4Lines[bufIdx];

        const s16 xInScreen = FP_GetExponent(mXPos - pCamPos->x) + gScreenManager->mCamXOff;
        const s16 yInScreen = FP_GetExponent(mYPos - pCamPos->y) + gScreenManager->mCamYOff;

        const s16 RectX_v_Psx = xInScreen + FP_GetExponent(FP_FromInteger(zVerts[0]) * mSpriteScale);
        const s16 RectW_v_Psx = xInScreen + FP_GetExponent(FP_FromInteger(zVerts[5]) * mSpriteScale);

        const s16 rectX_v = PsxToPCX(RectX_v_Psx, 11);
        const s16 rectY_v = FP_GetExponent(FP_FromInteger(zVerts[1]) * mSpriteScale) + yInScreen;
        const s16 rectW_v = PsxToPCX(RectW_v_Psx, 11);
        const s16 rectH_v = yInScreen + FP_GetExponent(FP_FromInteger(zVerts[7]) * mSpriteScale);

        pZLine->SetXY0(
               rectX_v,
               rectY_v);
        pZLine->SetXY1(
               static_cast<s16>(PsxToPCX(xInScreen + FP_GetExponent(FP_FromInteger(zVerts[2]) * mSpriteScale), 11)),
               yInScreen + FP_GetExponent(FP_FromInteger(zVerts[3]) * mSpriteScale));
        pZLine->SetXY2(
               static_cast<s16>(PsxToPCX(xInScreen + FP_GetExponent(FP_FromInteger(zVerts[4]) * mSpriteScale), 11)),
               yInScreen + FP_GetExponent(FP_FromInteger(zVerts[5]) * mSpriteScale));
        pZLine->SetXY3(
               rectW_v,
               rectH_v);

        pZLine->SetRGB0(
                static_cast<u8>(mRGB.r * 8 / 10),
                static_cast<u8>(mRGB.g * 8 / 10),
                static_cast<u8>(mRGB.b * 8 / 10));
        pZLine->SetRGB1(
                static_cast<u8>(mRGB.r),
                static_cast<u8>(mRGB.g),
                static_cast<u8>(mRGB.b));
        pZLine->SetRGB2(
                static_cast<u8>(mRGB.r * 7 / 10),
                static_cast<u8>(mRGB.g * 7 / 10),
                static_cast<u8>(mRGB.b * 7 / 10));
        pZLine->SetRGB3(
                static_cast<u8>(mRGB.r / 2),
                static_cast<u8>(mRGB.g / 2),
                static_cast<u8>(mRGB.b / 2));

        pZLine->SetSemiTransparent(true);
        pZLine->SetBlendMode(relive::TBlendModes::eBlend_1);
        OrderingTable_Add(OtLayer(ppOt, mOtLayer), pZLine);
    }
}

} // namespace AO
