#include "stdafx.h"
#include "AnimationUnknown.hpp"
#include "PsxDisplay.hpp"
#include "GameType.hpp"

void AnimationUnknown::VCleanUp()
{
    mAnimPtr = nullptr;
}
void AnimationUnknown::VDecode()
{
    // Empty
}

void AnimationUnknown::VRender(s32 xpos, s32 ypos, OrderingTable& ot, s16 /*width*/, s32 /*height*/)
{
    Poly_FT4* pPoly = &mPoly;
    if (GetRender())
    {
        // Copy from animation to local
        *pPoly = mAnimPtr->mPoly;

        const PerFrameInfo* pFrameInfoHeader = mAnimPtr->Get_FrameHeader(-1);

        s32 frameH = pFrameInfoHeader->mHeight;
        s32 frameW = pFrameInfoHeader->mWidth;

        s32 frameOffX = pFrameInfoHeader->mXOffset;
        s32 frameOffY = pFrameInfoHeader->mYOffset;

        if (mSpriteScale != FP_FromInteger(1))
        {
            frameOffX = FP_GetExponent(FP_FromInteger(frameOffX) * mSpriteScale);
            frameOffY = FP_GetExponent(FP_FromInteger(frameOffY) * mSpriteScale);

            frameH = FP_GetExponent(FP_FromInteger(frameH) * mSpriteScale);
            frameW = FP_GetExponent(FP_FromInteger(frameW) * mSpriteScale);

        }

        s32 polyX = 0;
        s32 polyY = 0;

        // TODO: Factor out when file formats are converted
        const bool isAe = GetGameType() == GameType::eAe;
        s32 xConverted = isAe ? PsxToPCX(xpos) : xpos;
        if (mAnimPtr->GetSwapXY())
        {
            if (mAnimPtr->GetFlipY())
            {
                if (mAnimPtr->GetFlipX())
                {
                    polyX = xConverted - frameOffY - frameH;
                }
                else
                {
                    polyX = frameOffY + xConverted;
                }
                polyY = frameOffX + ypos;
            }
            else
            {
                if (mAnimPtr->GetFlipX())
                {
                    polyX = xConverted - frameOffY - frameH;
                }
                else
                {
                    polyX = frameOffY + xConverted;
                }
                polyY = ypos - frameOffX - frameW;
            }
        }
        else if (mAnimPtr->GetFlipY())
        {
            if (mAnimPtr->GetFlipX())
            {
                polyX = xConverted - frameOffX - frameW;
            }
            else
            {
                polyX = frameOffX + xConverted;
            }
            polyY = ypos - frameOffY - frameH;
        }
        else
        {
            if (mAnimPtr->GetFlipX())
            {
                polyX = xConverted - frameOffX - frameW;
            }
            else
            {
                polyX = frameOffX + xConverted;
            }
            polyY = frameOffY + ypos;
        }

        if (!GetBlending())
        {
            pPoly->SetRGB0(mRgb.r & 0xFF, mRgb.g & 0xFF, mRgb.b & 0xFF);
        }
        
        pPoly->SetXYWH(
                static_cast<s16>(polyX),
                static_cast<s16>(polyY),
                static_cast<s16>(frameW - 1),
                static_cast<s16>(frameH - 1));


        ot.Add(GetRenderLayer(), pPoly);
    }
}

void AnimationUnknown::GetRenderedSize(PSX_RECT* pRect)
{
    Poly_FT4_Get_Rect(pRect, &mPoly);
}

