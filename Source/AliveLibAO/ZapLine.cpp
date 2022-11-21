#include "stdafx_ao.h"
#include "Function.hpp"
#include "ZapLine.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Game.hpp"
#include "Math.hpp"

namespace AO {

ZapLine::~ZapLine()
{
    relive_delete[] mSprites;
    relive_delete[] mSpritePositions;
    relive_delete[] mZapPoints;
    relive_delete[] mSpriteSegmentPositions;
}

ZapLine::ZapLine(FP x1, FP y1, FP x2, FP y2, s32 aliveTime, ZapLineType type, Layer layer)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eZapLine);
    mZapLineType = type;

    AnimId animId = AnimId::None;
    if (mZapLineType == ZapLineType::eThin_1)
    {
        // Creates thin blue zap lines.
        mNumberOfPiecesPerSegment = 20;
        mNumberOfSegments = 12;
        animId = AnimId::Zap_Line_Blue;
        field_11C_tPageAbr = TPageAbr::eBlend_3;
    }
    else if (mZapLineType == ZapLineType::eThick_0)
    {
        // Creates thick red zap lines.
        mNumberOfPiecesPerSegment = 10;
        mNumberOfSegments = 28;
        animId = AnimId::Zap_Line_Red;
        field_11C_tPageAbr = TPageAbr::eBlend_1;
    }

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    Animation_Init(GetAnimRes(animId));

    GetAnimation().mFlags.Clear(AnimFlags::eSemiTrans);
    GetAnimation().SetRenderLayer(layer);
    mNumberOfSprites = mNumberOfSegments * mNumberOfPiecesPerSegment;


    mSprites = relive_new ZapLineSprites[mNumberOfSprites];
    mSpritePositions = relive_new PSX_Point[mNumberOfSprites];
    mZapPoints = relive_new ZapPoint[mNumberOfPiecesPerSegment];
    mSpriteSegmentPositions = relive_new FP_Point[mNumberOfSegments];

    mMaxAliveTime = static_cast<s16>(aliveTime);

    mXPos = x1;
    mYPos = y1;

    mState = ZapLineState::eInit_0;
    mAliveTimer = 0;

    const u8 u0 = 0;//mAnim.mVramRect.x & 0x3F;

    auto pFrameHeader = GetAnimation().Get_FrameHeader(-1);

    const u32 frameW = pFrameHeader->mWidth;
    const u32 frameH = pFrameHeader->mHeight;

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < mNumberOfSegments; j++)
        {
            for (s32 k = 0; k < mNumberOfPiecesPerSegment; k++)
            {
                Prim_Sprt* pSprt = &mSprites[(j * mNumberOfPiecesPerSegment) + k].field_0_sprts[i];
                Sprt_Init(pSprt);

                Poly_Set_SemiTrans(&pSprt->mBase.header, 1);
                Poly_Set_Blending(&pSprt->mBase.header, 1);

                pSprt->mAnim = &GetAnimation();

                SetUV0(pSprt, u0, 0 /*mAnim.mVramRect.y & 0xFF*/);
                pSprt->field_14_w = static_cast<s16>(frameW - 1);
                pSprt->field_16_h = static_cast<s16>(frameH - 1);
            }
        }
    }

    CalculateSourceAndDestinationPositions(x1, y1, x2, y2);
}

void ZapLine::CalculateSourceAndDestinationPositions(FP xPosSource, FP yPosSource, FP xPosDest, FP yPosDest)
{
    mXPosSrc = FP_GetExponent(xPosSource - (pScreenManager->mCamPos->x - FP_FromInteger(pScreenManager->mCamXOff)));
    mYPosSrc = FP_GetExponent(yPosSource - (pScreenManager->mCamPos->y - FP_FromInteger(pScreenManager->mCamYOff)));
    mXPosDst = FP_GetExponent(xPosDest - (pScreenManager->mCamPos->x - FP_FromInteger(pScreenManager->mCamXOff)));
    mYPosDst = FP_GetExponent(yPosDest - (pScreenManager->mCamPos->y - FP_FromInteger(pScreenManager->mCamYOff)));

    mXPosSrc = PsxToPCX(mXPosSrc, 11);
    mXPosDst = PsxToPCX(mXPosDst, 11);

    s16 xOff = 0;
    s16 yOff = 0;
    GetAnimation().Get_Frame_Offset(&xOff, &yOff);

    mXPosSrc = FP_GetExponent(FP_FromInteger(xOff) + FP_FromInteger(mXPosSrc));
    mYPosSrc = FP_GetExponent(FP_FromInteger(yOff) + FP_FromInteger(mYPosSrc));
    mXPosDst = FP_GetExponent(FP_FromInteger(xOff) + FP_FromInteger(mXPosDst));
    mYPosDst = FP_GetExponent(FP_FromInteger(yOff) + FP_FromInteger(mYPosDst));
}


void ZapLine::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ZapLine::CalculateThickSpriteSegmentPositions()
{
    // TODO: Convert bit operations to something more readable.
    s32 v1 = 0;
    if (mAliveTimer >= 8)
    {
        const s32 v4 = mMaxAliveTime - mAliveTimer;
        if (v4 >= 8)
        {
            v1 = 4;
        }
        else
        {
            v1 = (v4 / 4) + 3;
        }
    }
    else
    {
        v1 = (mAliveTimer / 4) + 3;
    }

    s32 v5 = 1 << v1;
    s32 v6 = 1 << (v1 - 1);

    mSpriteSegmentPositions[0].x = FP_FromInteger(mXPosSrc);
    mSpriteSegmentPositions[0].y = FP_FromInteger(mYPosSrc);

    mSpriteSegmentPositions[mNumberOfSegments - 1].x = FP_FromInteger(mXPosDst);
    mSpriteSegmentPositions[mNumberOfSegments - 1].y = FP_FromInteger(mYPosDst);

    s32 angExtra = 0;
    if ((sGnFrame / 8) & 1)
    {
        angExtra = 0;
    }
    else
    {
        angExtra = 128;
    }

    const FP xDiff = FP_FromInteger(mXPosDst - mXPosSrc) / FP_FromInteger(mNumberOfSegments);
    const FP xDiffDiv = (xDiff * FP_FromDouble(1.5));

    const FP yDiff = FP_FromInteger(mYPosDst - mYPosSrc) / FP_FromInteger(mNumberOfSegments);
    const FP yDiffDiv = (-yDiff * FP_FromDouble(1.5));


    // First and last done above.
    for (s32 i = 1; i < mNumberOfSegments - 1; i++)
    {
        const u8 ang = static_cast<u8>(angExtra + 18 * i);
        mSpriteSegmentPositions[i].x = FP_FromInteger(Math_NextRandom() % v5) + (Math_Cosine(ang) * yDiffDiv) + FP_FromInteger(mXPosSrc) + (FP_FromInteger(i) * xDiff) - FP_FromInteger(v6);

        mSpriteSegmentPositions[i].y = FP_FromInteger(Math_NextRandom() % v5) + (Math_Cosine(ang) * xDiffDiv) + FP_FromInteger(mYPosSrc) + (FP_FromInteger(i) * yDiff) - FP_FromInteger(v6);
    }

    mPsxDisplayRects[0].x = 0;
    mPsxDisplayRects[0].y = 0;
    mPsxDisplayRects[0].w = gPsxDisplay.mWidth;
    mPsxDisplayRects[0].h = gPsxDisplay.mHeight;

    mPsxDisplayRects[1].x = 0;
    mPsxDisplayRects[1].y = 0;
    mPsxDisplayRects[1].w = gPsxDisplay.mWidth;
    mPsxDisplayRects[1].h = gPsxDisplay.mHeight;
}

void ZapLine::CalculateThinSpriteSegmentPositions()
{
    mSpriteSegmentPositions[0].x = FP_FromInteger(mXPosSrc);
    mSpriteSegmentPositions[0].y = FP_FromInteger(mYPosSrc);
    mSpriteSegmentPositions[mNumberOfSegments - 1].x = FP_FromInteger(mXPosDst);
    mSpriteSegmentPositions[mNumberOfSegments - 1].y = FP_FromInteger(mYPosDst);

    const FP x2Diff = FP_FromInteger(mXPosDst - mXPosSrc) / FP_FromInteger(mNumberOfSegments);
    const FP y2Diff = FP_FromInteger(mYPosDst - mYPosSrc) / FP_FromInteger(mNumberOfSegments);

    const FP y2DiffDiv = -y2Diff * FP_FromDouble(0.1);
    const FP x2DiffDiv = x2Diff * FP_FromDouble(0.1);

    for (s32 i = 1; i < mNumberOfSegments - 1; i++)
    {
        const FP rnd = FP_FromInteger(Math_NextRandom() % 32 - 16);
        mSpriteSegmentPositions[i].x = (y2DiffDiv * rnd) + FP_FromInteger(mXPosSrc) + (FP_FromInteger(i) * x2Diff);
        mSpriteSegmentPositions[i].y = (x2DiffDiv * rnd) + FP_FromInteger(mYPosSrc) + (FP_FromInteger(i) * y2Diff);
    }
}

void ZapLine::CalculateZapPoints_479380()
{
    FP acc = FP_FromInteger(0);
    const FP delta = FP_FromInteger(1) / FP_FromInteger(mNumberOfPiecesPerSegment);
    for (s32 i = 0; i < mNumberOfPiecesPerSegment; i++)
    {
        const FP accSqrd = (acc * acc);
        mZapPoints[i].field_0 = accSqrd - FP_FromRaw(2 * acc.fpValue) + FP_FromInteger(1);
        mZapPoints[i].field_4 = -FP_FromRaw(2 * accSqrd.fpValue) + FP_FromRaw(2 * acc.fpValue) + FP_FromInteger(1);
        mZapPoints[i].field_8 = accSqrd;
        acc += delta;
    }
}

void ZapLine::CalculateSpritePositionsInner(s32 idx1, s32 idx2, s32 idx3, s16 idx4)
{
    const FP x1 = mSpriteSegmentPositions[idx1].x;
    const FP y1 = mSpriteSegmentPositions[idx1].y;

    const FP x2 = mSpriteSegmentPositions[idx2].x;
    const FP y2 = mSpriteSegmentPositions[idx2].y;

    const FP x3 = mSpriteSegmentPositions[idx3].x;
    const FP y3 = mSpriteSegmentPositions[idx3].y;

    for (s32 i = 0; i < mNumberOfPiecesPerSegment; i++)
    {
        auto pItem = &mSpritePositions[i + (idx4 * mNumberOfPiecesPerSegment)];

        pItem->x = FP_GetExponent(
            FP_FromRaw((
                           (mZapPoints[i].field_8 * x3) + (mZapPoints[i].field_4 * x2) + (mZapPoints[i].field_0 * x1))
                           .fpValue
                       >> 1));

        pItem->y = FP_GetExponent(
            FP_FromRaw((
                           (mZapPoints[i].field_8 * y3) + (mZapPoints[i].field_4 * y2) + (mZapPoints[i].field_0 * y1))
                           .fpValue
                       >> 1));
    }
}

void ZapLine::UpdateSpriteVertexPositions()
{
    for (s32 i = 0; i < mNumberOfSegments; i++)
    {
        for (s32 j = 0; j < mNumberOfPiecesPerSegment; j++)
        {
            const auto pPoint = &mSpritePositions[j + (i * mNumberOfPiecesPerSegment)];
            Prim_Sprt* pSprt = &mSprites->field_0_sprts[j + (i * mNumberOfPiecesPerSegment)];
            SetXY0(&pSprt[0], pPoint->x, pPoint->y);
            SetXY0(&pSprt[1], pPoint->x, pPoint->y);
        }
    }
}

void ZapLine::CalculateSpritePositionsOuter()
{
    for (s16 i = 0; i < mNumberOfSegments; i++)
    {
        if (i == 0)
        {
            // First item.
            CalculateSpritePositionsInner(0, 0, 1, 0);
        }
        else
        {
            const s16 lastIdx = mNumberOfSegments - 1;
            if (i == lastIdx)
            {
                // Last item.
                CalculateSpritePositionsInner(mNumberOfSegments - 2, lastIdx, lastIdx, mNumberOfSegments - 1);
            }
            else
            {
                // Other items.
                CalculateSpritePositionsInner(i - 1, i, i + 1, i);
            }
        }
    }
}

void ZapLine::VUpdate()
{
    mAliveTimer++;

    // TODO: States 3 and 4 might not actually be needed, since states 1 and 2 do the same thing; though the class only seems to render in states 3 and 4.
    switch (mState)
    {
        case ZapLineState::eInit_0:
            CalculateZapPoints_479380();

            if (mZapLineType == ZapLineType::eThin_1)
            {
                CalculateThinSpriteSegmentPositions();
            }
            else if (mZapLineType == ZapLineType::eThick_0)
            {
                CalculateThickSpriteSegmentPositions();
            }

            mState = ZapLineState::eInitSpritePositions_1;
            break;

        case ZapLineState::eInitSpritePositions_1:
            CalculateSpritePositionsOuter();
            mState = ZapLineState::eInitSpriteVertices_2;
            break;

        case ZapLineState::eInitSpriteVertices_2:
        case ZapLineState::eUpdateSpriteVertices_4:
            UpdateSpriteVertexPositions();

            if (mAliveTimer >= mMaxAliveTime && mZapLineType != ZapLineType::eThin_1)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            if (mZapLineType == ZapLineType::eThin_1)
            {
                CalculateThinSpriteSegmentPositions();
            }
            else if (mZapLineType == ZapLineType::eThick_0)
            {
                CalculateThickSpriteSegmentPositions();
            }
            mState = ZapLineState::eUpdateSpritePositions_3;
            break;

        case ZapLineState::eUpdateSpritePositions_3:
            CalculateSpritePositionsOuter();
            mState = ZapLineState::eUpdateSpriteVertices_4;
            break;
    }
}

void ZapLine::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0)
        && mState > ZapLineState::eInitSpriteVertices_2)
    {
        const auto bufferIdx = gPsxDisplay.mBufferIndex;
        const s32 calcTPage = PSX_getTPage(field_11C_tPageAbr);

        Prim_SetTPage* pTPage = &field_EC_tPage_p8[bufferIdx];
        Init_SetTPage(pTPage, 0, 0, calcTPage);
        OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pTPage->mBase);

        for (s32 i = 0; i < mNumberOfSegments; i++)
        {
            for (s32 j = 0; j < mNumberOfPiecesPerSegment; j++)
            {
                Prim_Sprt* pSprt = &mSprites->field_0_sprts[j + (i * mNumberOfPiecesPerSegment)];
                OrderingTable_Add(OtLayer(ppOt, GetAnimation().GetRenderLayer()), &pSprt[bufferIdx].mBase.header);
            }
        }

        PSX_RECT* pRect = &mPsxDisplayRects[bufferIdx];
        pRect->x = 32767;
        pRect->w = -32767;
        pRect->y = 32767;
        pRect->h = -32767;

        for (s32 i = 0; i < mNumberOfSegments; i++)
        {
            const PSX_Point* pPoint = &mSpritePositions[i * mNumberOfPiecesPerSegment];
            for (s32 j = 0; j < mNumberOfPiecesPerSegment; j++)
            {
                if (pPoint->x < pRect->x)
                {
                    pRect->x = pPoint->x;
                }

                if (pPoint->x > pRect->w)
                {
                    pRect->w = pPoint->x;
                }

                if (pPoint->y < pRect->y)
                {
                    pRect->y = pPoint->y;
                }

                if (pPoint->y > pRect->h)
                {
                    pRect->h = pPoint->y;
                }
            }
        }

        pRect->x -= 25;
        pRect->w += 25;
        pRect->y -= 25;
        pRect->h += 25;
    }
}

} // namespace AO
