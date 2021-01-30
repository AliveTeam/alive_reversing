#include "stdafx.h"
#include "AnimationUnknown.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "stdlib.hpp"
#include "Sys_common.hpp"

void AnimationUnknown::vDecode_40AC90()
{
    // VNull_409C20
}

void AnimationUnknown::vRender_40B820(int xpos, int ypos, PrimHeader** ppOt, __int16 width, signed int height)
{
    vRender_40C690(xpos, ypos, ppOt, width, height);
}

void AnimationUnknown::vCleanUp_40C630()
{
    vCleanUp_40C9C0();
}

void AnimationUnknown::GetRenderedSize_40C980(PSX_RECT* pRect)
{
    Poly_FT4_Get_Rect_409DA0(pRect, &field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index]);
}

void AnimationUnknown::vRender_40C690(int xpos, int ypos, PrimHeader** ppOt, int /*width*/, int /*height*/)
{
    Poly_FT4* pPoly = &field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index];
    if (field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        // Copy from animation to local
        *pPoly = field_68_anim_ptr->field_2C_ot_data[gPsxDisplay_5C1130.field_C_buffer_index];
        FrameInfoHeader* pFrameInfoHeader = field_68_anim_ptr->Get_FrameHeader_40B730(-1);

        if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit22_DeadMode))
        {
            ALIVE_FATAL("Impossible branch.");
        }

        FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_68_anim_ptr->field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

        int frameH = pFrameHeader->field_5_height;
        int frameW = pFrameHeader->field_4_width;

        int frameOffX = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x;
        int frameOffY = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.y;

        if (field_6C_scale != FP_FromInteger(1))
        {
            frameOffX = FP_GetExponent((FP_FromInteger(frameOffX) * field_6C_scale));
            frameOffY = FP_GetExponent((FP_FromInteger(frameOffY) * field_6C_scale));

            frameH = FP_GetExponent(FP_FromInteger(frameH) * field_6C_scale);
            frameW = FP_GetExponent((FP_FromInteger(frameW) * field_6C_scale));
        }

        int polyX = 0;
        int polyY = 0;
        int xConverted = PsxToPCX(xpos);
        if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit7_SwapXY))
        {
            if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit6_FlipY))
            {
                if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
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
                if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
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
        else if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit6_FlipY))
        {
            if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
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
            if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                polyX = xConverted - frameOffX - frameW;
            }
            else
            {
                polyX = frameOffX + xConverted;
            }
            polyY = frameOffY + ypos;
        }

        if (!field_4_flags.Get(AnimFlags::eBit16_bBlending))
        {
            SetRGB0(pPoly, field_8_r, field_9_g, field_A_b);
        }

        SetXYWH(pPoly,
            static_cast<short>(polyX),
            static_cast<short>(polyY),
            static_cast<short>(frameW - 1),
            static_cast<short>(frameH - 1));

        if (pFrameHeader->field_7_compression_type == 3 || pFrameHeader->field_7_compression_type == 6)
        {
            SetPrimExtraPointerHack(pPoly, &pFrameHeader->field_8_width2);
        }
        else
        {
            SetPrimExtraPointerHack(pPoly, nullptr);
        }
        OrderingTable_Add_4F8AA0(&ppOt[field_C_render_layer], &pPoly->mBase.header);
    }
}

void AnimationUnknown::vCleanUp_40C9C0()
{
    field_68_anim_ptr = nullptr;
}
