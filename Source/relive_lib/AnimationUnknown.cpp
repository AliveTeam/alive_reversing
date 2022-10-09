#include "stdafx.h"
#include "../relive_lib/AnimationUnknown.hpp"
#include "PsxDisplay.hpp"
#include "GameType.hpp"

void AnimationUnknown::VCleanUp()
{
    field_68_anim_ptr = nullptr;
}
void AnimationUnknown::VDecode()
{
    // Empty
}

void AnimationUnknown::VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 /*width*/, s32 /*height*/)
{
    Poly_FT4* pPoly = &field_10_polys[gPsxDisplay.mBufferIndex];
    if (mFlags.Get(AnimFlags::eRender))
    {
        // Copy from animation to local
        *pPoly = field_68_anim_ptr->mOtData[gPsxDisplay.mBufferIndex];

        const PerFrameInfo* pFrameInfoHeader = field_68_anim_ptr->Get_FrameHeader(-1);

        s32 frameH = pFrameInfoHeader->mHeight;
        s32 frameW = pFrameInfoHeader->mWidth;

        s32 frameOffX = pFrameInfoHeader->mXOffset;
        s32 frameOffY = pFrameInfoHeader->mYOffset;

        if (field_6C_scale != FP_FromInteger(1))
        {
            frameOffX = FP_GetExponent(FP_FromInteger(frameOffX) * field_6C_scale);
            frameOffY = FP_GetExponent(FP_FromInteger(frameOffY) * field_6C_scale);

            frameH = FP_GetExponent(FP_FromInteger(frameH) * field_6C_scale);
            frameW = FP_GetExponent(FP_FromInteger(frameW) * field_6C_scale);

        }

        s32 polyX = 0;
        s32 polyY = 0;

        // TODO: Factor out when file formats are converted
        const bool isAe = GetGameType() == GameType::eAe;
        s32 xConverted = isAe ? PsxToPCX(xpos) : xpos;
        if (field_68_anim_ptr->mFlags.Get(AnimFlags::eSwapXY))
        {
            if (field_68_anim_ptr->mFlags.Get(AnimFlags::eFlipY))
            {
                if (field_68_anim_ptr->mFlags.Get(AnimFlags::eFlipX))
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
                if (field_68_anim_ptr->mFlags.Get(AnimFlags::eFlipX))
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
        else if (field_68_anim_ptr->mFlags.Get(AnimFlags::eFlipY))
        {
            if (field_68_anim_ptr->mFlags.Get(AnimFlags::eFlipX))
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
            if (field_68_anim_ptr->mFlags.Get(AnimFlags::eFlipX))
            {
                polyX = xConverted - frameOffX - frameW;
            }
            else
            {
                polyX = frameOffX + xConverted;
            }
            polyY = frameOffY + ypos;
        }

        if (!mFlags.Get(AnimFlags::eBlending))
        {
            SetRGB0(pPoly, mRed, mGreen, mBlue);
        }

        SetXYWH(pPoly,
                static_cast<s16>(polyX),
                static_cast<s16>(polyY),
                static_cast<s16>(frameW - 1),
                static_cast<s16>(frameH - 1));


        // TODO: Or pass this mAnimRes ?
        SetPrimExtraPointerHack(pPoly, &field_68_anim_ptr->mAnimRes);
        OrderingTable_Add(OtLayer(ppOt, mRenderLayer), &pPoly->mBase.header);
    }
}

void AnimationUnknown::GetRenderedSize(PSX_RECT* pRect)
{
    Poly_FT4_Get_Rect(pRect, &field_10_polys[gPsxDisplay.mBufferIndex]);
}

