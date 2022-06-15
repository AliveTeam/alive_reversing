#include "stdafx.h"
#include "AnimationUnknown.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "stdlib.hpp"
#include "Sys_common.hpp"

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
    Poly_FT4* pPoly = &field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index];
    if (mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        // Copy from animation to local
        *pPoly = field_68_anim_ptr->field_2C_ot_data[gPsxDisplay_5C1130.field_C_buffer_index];
        FrameInfoHeader* pFrameInfoHeader = field_68_anim_ptr->Get_FrameHeader(-1);

        if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit22_DeadMode))
        {
            ALIVE_FATAL("Impossible branch.");
        }

        FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_68_anim_ptr->field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

        s32 frameH = pFrameHeader->field_5_height;
        s32 frameW = pFrameHeader->field_4_width;

        s32 frameOffX = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x;
        s32 frameOffY = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.y;

        if (field_6C_scale != FP_FromInteger(1))
        {
            frameOffX = FP_GetExponent(FP_FromInteger(frameOffX) * field_6C_scale);
            frameOffY = FP_GetExponent(FP_FromInteger(frameOffY) * field_6C_scale);

            frameH = FP_GetExponent(FP_FromInteger(frameH) * field_6C_scale);
            frameW = FP_GetExponent(FP_FromInteger(frameW) * field_6C_scale);

        }

        s32 polyX = 0;
        s32 polyY = 0;
        s32 xConverted = PsxToPCX(xpos);
        if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit7_SwapXY))
        {
            if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit6_FlipY))
            {
                if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
                if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
        else if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit6_FlipY))
        {
            if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
            if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                polyX = xConverted - frameOffX - frameW;
            }
            else
            {
                polyX = frameOffX + xConverted;
            }
            polyY = frameOffY + ypos;
        }

        if (!mAnimFlags.Get(AnimFlags::eBit16_bBlending))
        {
            SetRGB0(pPoly, mRed, mGreen, mBlue);
        }

        SetXYWH(pPoly,
                static_cast<s16>(polyX),
                static_cast<s16>(polyY),
                static_cast<s16>(frameW - 1),
                static_cast<s16>(frameH - 1));

        if (pFrameHeader->field_7_compression_type == CompressionType::eType_3_RLE_Blocks || pFrameHeader->field_7_compression_type == CompressionType::eType_6_RLE)
        {
            SetPrimExtraPointerHack(pPoly, &pFrameHeader->field_8_width2);
        }
        else
        {
            SetPrimExtraPointerHack(pPoly, nullptr);
        }
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, mRenderLayer), &pPoly->mBase.header);
    }
}

void AnimationUnknown::GetRenderedSize(PSX_RECT* pRect)
{
    Poly_FT4_Get_Rect_409DA0(pRect, &field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index]);
}

