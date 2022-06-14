#include "AnimationUnknown.hpp"
#include "Animation.hpp"
#include "PsxDisplay.hpp"

namespace AO {

void AnimationUnknown::VCleanUp()
{
    VCleanUp2_404280();
}

void AnimationUnknown::VRender2(s32 xpos, s32 ypos, PrimHeader** ppOt)
{
    VRender2_403FD0(xpos, ypos, ppOt);
}

void AnimationUnknown::VRender(s32 /*xpos*/, s32 /*ypos*/, PrimHeader** /*pOt*/, s16 /*width*/, s16 /*height*/)
{
    // Empty @ 402A20
}

void AnimationUnknown::VDecode()
{
    // Empty @ 402A10
}

void AnimationUnknown::VCleanUp2_404280()
{
    field_68_anim_ptr = nullptr;
}

void AnimationUnknown::VRender2_403FD0(s32 xpos, s32 ypos, PrimHeader** ppOt)
{
    Poly_FT4* pPoly = &field_10_polys[gPsxDisplay_504C78.field_A_buffer_index];
    if (mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        // Copy from animation to local
        *pPoly = field_68_anim_ptr->field_2C_ot_data[gPsxDisplay_504C78.field_A_buffer_index];

        FrameInfoHeader* pFrameInfoHeader = field_68_anim_ptr->Get_FrameHeader(-1);

        FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_68_anim_ptr->field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

        s32 frameOffX = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x;
        s32 frameOffY = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.y;
        s32 frameH = pFrameHeader->field_5_height;
        s32 frameW = pFrameHeader->field_4_width;

        if (field_6C_scale != FP_FromInteger(1))
        {
            frameH = FP_GetExponent(FP_FromInteger(frameH) * field_6C_scale);
            frameW = FP_GetExponent(FP_FromInteger(frameW) * field_6C_scale);
            frameOffX = FP_GetExponent(FP_FromInteger(frameOffX) * field_6C_scale);
            frameOffY = FP_GetExponent(FP_FromInteger(frameOffY) * field_6C_scale);
        }

        s32 polyX = 0;
        s32 polyY = 0;
        if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit7_SwapXY))
        {
            if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit6_FlipY))
            {
                if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    polyX = xpos - frameOffY - frameH;
                }
                else
                {
                    polyX = frameOffY + xpos;
                }
                polyY = frameOffX + ypos;
            }
            else
            {
                if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    polyX = xpos - frameOffY - frameH;
                }
                else
                {
                    polyX = frameOffY + xpos;
                }
                polyY = ypos - frameOffX - frameW;
            }
        }
        else if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit6_FlipY))
        {
            if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                polyX = xpos - frameOffX - frameW;
            }
            else
            {
                polyX = frameOffX + xpos;
            }
            polyY = ypos - frameOffY - frameH;
        }
        else
        {
            if (field_68_anim_ptr->mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                polyX = xpos - frameOffX - frameW;
            }
            else
            {
                polyX = frameOffX + xpos;
            }
            polyY = frameOffY + ypos;
        }

        if (!mAnimFlags.Get(AnimFlags::eBit16_bBlending))
        {
            SetRGB0(pPoly, mRed, mGreen, mBlue);
        }

        const s32 w = frameW + polyX - 1;
        const s32 h = frameH + polyY - 1;
        SetXY0(pPoly, static_cast<s16>(polyX), static_cast<s16>(polyY));
        SetXY1(pPoly, static_cast<s16>(w), static_cast<s16>(polyY));
        SetXY2(pPoly, static_cast<s16>(polyX), static_cast<s16>(h));
        SetXY3(pPoly, static_cast<s16>(w), static_cast<s16>(h));

        SetPrimExtraPointerHack(pPoly, nullptr);
        OrderingTable_Add_498A80(OtLayer(ppOt, mRenderLayer), &pPoly->mBase.header);
    }
}

void AnimationUnknown::GetRenderedSize(PSX_RECT* pRect)
{
    Poly_FT4_Get_Rect(pRect, &field_10_polys[gPsxDisplay_504C78.field_A_buffer_index]);
}

}