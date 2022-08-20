#include "stdafx_ao.h"
#include "Function.hpp"
#include "SnoozeParticle.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "CameraSwapper.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO {

const s16 xPositionDeltaEntries_4CF8E0[39] = {
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
    0,
    0,
    0,
    0};

SnoozeParticle::~SnoozeParticle()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjListDrawables->Remove_Item(this);
    }
}

void SnoozeParticle::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

SnoozeParticle::SnoozeParticle(FP xpos, FP ypos, Layer layer, FP scale)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

    mBaseGameObjectTypeId = ReliveTypes::eSnoozParticle;
    gObjListDrawables->Push_Back(this);

    mStartY = ypos;
    mXPos = xpos;
    mYPos = ypos;

    field_24_dy = FP_FromDouble(0.15);
    field_20_dx = FP_FromInteger(1);

    field_24_dy = ((field_24_dy * FP_FromInteger(Math_NextRandom())) / FP_FromInteger(256));
    field_24_dy += FP_FromDouble(0.35);
    field_24_dy = field_24_dy * FP_FromInteger(-1);

    mOtLayer = layer;

    mSpriteScale = (scale * FP_FromDouble(0.4));

    field_2C_scale_dx = FP_FromDouble(0.30);
    field_2C_scale_dx = field_2C_scale_dx / (FP_FromInteger(20) / -field_24_dy);

    mRGB.SetRGB(0, 0, 0);
    mState = SnoozeParticleState::eRising_0;
    field_3A_count_down = 1;
    field_38_idx = Math_NextRandom() % 36;
    field_20_dx = FP_FromInteger(xPositionDeltaEntries_4CF8E0[field_38_idx]);
    field_38_idx++;
}

void SnoozeParticle::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!sNumCamSwappers_507668)
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

                    mSpriteScale += field_2C_scale_dx;

                    if (field_38_idx > 36)
                    {
                        field_38_idx = 0;
                    }

                    const FP idx_toFP = FP_FromInteger(xPositionDeltaEntries_4CF8E0[field_38_idx]);
                    field_20_dx = idx_toFP;
                    field_38_idx += 1;
                    mXPos += idx_toFP;
                    mYPos += field_24_dy;
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
                mRGB.b /= 2;
                mRGB.b /= 2;
                mXPos += field_20_dx;
                mYPos += field_24_dy;

                if (field_3A_count_down > 0)
                {
                    field_3A_count_down--;
                }
                else
                {
                    SfxPlayMono(SoundEffect::ZPop_5, 0, 0);
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
        }
    }
}

//Identical to AE
const PSX_Point explosionVerts[6][2] = {
    {{-3, -4}, {-6, -7}},
    {{3, -4}, {6, -7}},
    {{4, -1}, {7, -1}},
    {{-4, 1}, {-7, 1}},
    {{-3, 4}, {-6, 7}},
    {{3, 4}, {6, 7}}};
const s16 zVerts[8] = {
    -4,
    -4,
    4,
    -4,
    -4,
    4,
    4,
    4};

void SnoozeParticle::VRender(PrimHeader** ppOt)
{
    //Identical to AE except xInScreen, yInScreen are offset by pScreenManager positions
    PSX_RECT rectToInvalidate = {};
    FP_Point* pCamPos = pScreenManager->mCamPos;
    const s16 bufIdx = gPsxDisplay.mBufferIndex;

    if (mState == SnoozeParticleState::eBlowingUp_2)
    {
        const s16 xInScreen = FP_GetExponent(mXPos - pCamPos->x) + pScreenManager->mCamXOff;
        const s16 yInScreen = FP_GetExponent(mYPos - pCamPos->y) + pScreenManager->mCamYOff;

        for (s32 i = 0; i < ALIVE_COUNTOF(explosionVerts); i++)
        {
            Line_G2* pZExplosionLine = &field_94_lines[bufIdx][i];
            LineG2_Init(pZExplosionLine);

            const s32 scaledLineRelativeStartX = FP_GetExponent(FP_FromInteger(explosionVerts[i][0].x) * mSpriteScale);
            const s32 scaledLineRelativeStartY = FP_GetExponent(FP_FromInteger(explosionVerts[i][0].y) * mSpriteScale);
            const s32 scaledLineRelativeEndX = FP_GetExponent(FP_FromInteger(explosionVerts[i][1].x) * mSpriteScale);
            const s32 scaledLineRelativeEndY = FP_GetExponent(FP_FromInteger(explosionVerts[i][1].y) * mSpriteScale);
            SetXY0(pZExplosionLine,
                   static_cast<s16>(PsxToPCX(xInScreen + scaledLineRelativeStartX, 11)),
                   static_cast<s16>(yInScreen + scaledLineRelativeStartY));
            SetXY1(pZExplosionLine,
                   static_cast<s16>(PsxToPCX(xInScreen + scaledLineRelativeEndX, 11)),
                   static_cast<s16>(yInScreen + scaledLineRelativeEndY));

            SetRGB0(pZExplosionLine,
                    static_cast<u8>(mRGB.r / 2),
                    static_cast<u8>(mRGB.g / 2),
                    static_cast<u8>(mRGB.b / 2));
            SetRGB1(pZExplosionLine,
                    static_cast<u8>(mRGB.r),
                    static_cast<u8>(mRGB.g),
                    static_cast<u8>(mRGB.b));

            Poly_Set_SemiTrans(&pZExplosionLine->mBase.header, 1);
            OrderingTable_Add(OtLayer(ppOt, mOtLayer), &pZExplosionLine->mBase.header);
        }
        rectToInvalidate.x = static_cast<s16>(PsxToPCX(xInScreen - 8, 0));
        rectToInvalidate.w = static_cast<s16>(PsxToPCX(xInScreen + 8, 0));
        rectToInvalidate.y = static_cast<s16>(yInScreen - 8);
        rectToInvalidate.h = static_cast<s16>(yInScreen + 8);
    }
    else
    {
        Line_G4* pZLine = &field_3C_lines[bufIdx];
        LineG4_Init(pZLine);

        const s16 xInScreen = FP_GetExponent(mXPos - pCamPos->x) + pScreenManager->mCamXOff;
        const s16 yInScreen = FP_GetExponent(mYPos - pCamPos->y) + pScreenManager->mCamYOff;

        const s16 RectX_v_Psx = xInScreen + FP_GetExponent(FP_FromInteger(zVerts[0]) * mSpriteScale);
        const s16 RectW_v_Psx = xInScreen + FP_GetExponent(FP_FromInteger(zVerts[5]) * mSpriteScale);

        const s16 rectX_v = PsxToPCX(RectX_v_Psx, 11);
        const s16 rectY_v = FP_GetExponent(FP_FromInteger(zVerts[1]) * mSpriteScale) + yInScreen;
        const s16 rectW_v = PsxToPCX(RectW_v_Psx, 11);
        const s16 rectH_v = yInScreen + FP_GetExponent(FP_FromInteger(zVerts[7]) * mSpriteScale);

        SetXY0(pZLine,
               rectX_v,
               rectY_v);
        SetXY1(pZLine,
               static_cast<s16>(PsxToPCX(xInScreen + FP_GetExponent(FP_FromInteger(zVerts[2]) * mSpriteScale), 11)),
               yInScreen + FP_GetExponent(FP_FromInteger(zVerts[3]) * mSpriteScale));
        SetXY2(pZLine,
               static_cast<s16>(PsxToPCX(xInScreen + FP_GetExponent(FP_FromInteger(zVerts[4]) * mSpriteScale), 11)),
               yInScreen + FP_GetExponent(FP_FromInteger(zVerts[5]) * mSpriteScale));
        SetXY3(pZLine,
               rectW_v,
               rectH_v);

        SetRGB0(pZLine,
                static_cast<u8>(mRGB.r * 8 / 10),
                static_cast<u8>(mRGB.g * 8 / 10),
                static_cast<u8>(mRGB.b * 8 / 10));
        SetRGB1(pZLine,
                static_cast<u8>(mRGB.r),
                static_cast<u8>(mRGB.g),
                static_cast<u8>(mRGB.b));
        SetRGB2(pZLine,
                static_cast<u8>(mRGB.r * 7 / 10),
                static_cast<u8>(mRGB.g * 7 / 10),
                static_cast<u8>(mRGB.b * 7 / 10));
        SetRGB3(pZLine,
                static_cast<u8>(mRGB.r / 2),
                static_cast<u8>(mRGB.g / 2),
                static_cast<u8>(mRGB.b / 2));

        Poly_Set_SemiTrans(&pZLine->mBase.header, 1);
        OrderingTable_Add(OtLayer(ppOt, mOtLayer), &pZLine->mBase.header);
        rectToInvalidate.x = rectX_v;
        rectToInvalidate.y = rectY_v;
        rectToInvalidate.w = rectW_v;
        rectToInvalidate.h = rectH_v;
    }
    Prim_SetTPage* thisTPage = &field_1B4_tPage[bufIdx];
    const s32 tPage = PSX_getTPage(TPageMode::e4Bit_0, TPageAbr::eBlend_1, 0, 0);
    Init_SetTPage(thisTPage, 1, 0, tPage);
    OrderingTable_Add(OtLayer(ppOt, mOtLayer), &thisTPage->mBase);

    pScreenManager->InvalidateRectCurrentIdx(
        rectToInvalidate.x, rectToInvalidate.y,
        rectToInvalidate.w, rectToInvalidate.h);
}

} // namespace AO
