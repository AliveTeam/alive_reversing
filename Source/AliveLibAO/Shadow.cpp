#include "stdafx_ao.h"
#include "Function.hpp"
#include "Shadow.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "ScreenManager.hpp"
#include "Collisions.hpp"

#undef min
#undef max

namespace AO {

Shadow::Shadow()
{
    const AnimRecord& shadowRec = AO::AnimRec(AnimId::ObjectShadow);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, shadowRec.mResourceId, TRUE, FALSE);
    field_18_animation.Init(shadowRec.mFrameTableOffset, gAnimations, 0, shadowRec.mMaxW, shadowRec.mMaxH, ppRes);

    field_14_flags.Clear(Flags::eBit1_ShadowAtBottom);
    field_14_flags.Set(Flags::eBit2_Enabled);

    field_18_animation.mRenderMode = TPageAbr::eBlend_2;

    field_18_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    field_18_animation.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);

    field_18_animation.mAnimFlags.Set(AnimFlags::eBit2_Animate);
    field_18_animation.mAnimFlags.Set(AnimFlags::eBit8_Loop);
    field_18_animation.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    field_18_animation.mAnimFlags.Set(AnimFlags::eBit17_bOwnPal);
    field_18_animation.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
    field_18_animation.mAnimFlags.Set(AnimFlags::eBit20_use_xy_offset);
    field_18_animation.mAnimFlags.Set(AnimFlags::eBit21);
}

Shadow::~Shadow()
{
    field_18_animation.VCleanUp();
}

void Shadow::Calculate_Position(FP xpos, FP ypos, PSX_RECT* frameRect, FP spriteScale, Scale scale)
{
    if (field_14_flags.Get(Flags::eBit2_Enabled))
    {
        // TODO: Is this the same as PsxToPCX ??
        const s16 objX = (23 * frameRect->x) / 40;
        const s16 objW = (23 * frameRect->w) / 40;

        FP objY = {};
        if (field_14_flags.Get(Flags::eBit1_ShadowAtBottom))
        {
            // Get the bottom of the object
            objY = FP_FromInteger(frameRect->h) + pScreenManager->CamYPos();
        }
        else
        {
            objY = ypos;
        }

        const s32 lineType = spriteScale != FP_FromDouble(0.5) ? 7 : 0x70;
        
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (sCollisions->Raycast(
                xpos,
                objY,
                xpos,
                objY + FP_FromInteger(240),
                &pLine,
                &hitX,
                &hitY,
                lineType))
        {
            const s16 camXPos = FP_GetExponent(pScreenManager->CamXPos());
            s16 lineXScreen = pLine->field_0_rect.x - camXPos;
            s16 lineWScreen = pLine->field_0_rect.w - camXPos;

            if (lineWScreen < lineXScreen)
            {
                lineXScreen = pLine->field_0_rect.w - camXPos;
                lineWScreen = pLine->field_0_rect.x - camXPos;
            }

            field_18_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);

            field_8_xpos = xpos;
            field_C_ypos = hitY + FP_FromInteger(3);

            field_10_scale = (FP_FromInteger(1) - (((hitY - objY) * FP_FromDouble(0.75)) / FP_FromInteger(240))) * spriteScale;

            // Object is before the line we hit
            if (objX < lineXScreen)
            {
                if (sCollisions->Raycast(
                        FP_NoFractional(pScreenManager->CamXPos()) + (FP_FromInteger(lineXScreen - 1)) - FP_FromInteger(4),
                        hitY - FP_FromInteger(2),
                        FP_NoFractional(pScreenManager->CamXPos()) + (FP_FromInteger(lineXScreen - 1)) - FP_FromInteger(4),
                        hitY + FP_FromInteger(2),
                        &pLine,
                        &hitX,
                        &hitY,
                        lineType))
                {
                    lineXScreen = std::min(pLine->field_0_rect.x, pLine->field_0_rect.w) - FP_GetExponent(pScreenManager->CamXPos());
                }
            }

            // Object is after the line we hit
            if (objX > lineWScreen)
            {
                if (sCollisions->Raycast(
                        FP_NoFractional(pScreenManager->CamXPos()) + (FP_FromInteger(lineWScreen + 1)) + FP_FromInteger(4),
                        hitY - FP_FromInteger(2),
                        FP_NoFractional(pScreenManager->CamXPos()) + (FP_FromInteger(lineWScreen + 1)) + FP_FromInteger(4),
                        hitY + FP_FromInteger(2),
                        &pLine,
                        &hitX,
                        &hitY,
                        lineType))
                {
                    lineWScreen = std::max(pLine->field_0_rect.x, pLine->field_0_rect.w) - FP_GetExponent(pScreenManager->CamXPos());
                }
            }


            field_0_x1 = std::max(objX, lineXScreen);
            field_4_x2 = std::min(objW, lineWScreen);

            s16 height;
            if (FP_GetExponent(field_10_scale * FP_FromInteger(6)) <= 6)
            {
                height = FP_GetExponent(field_10_scale * FP_FromInteger(6));
            }
            else
            {
                height = 6;
            }

            const s16 finalYPos = FP_GetExponent(field_C_ypos - pScreenManager->CamYPos()) - height / 2;
            field_2_y1 = finalYPos;
            field_6_y2 = finalYPos + height;
        }
        else
        {
            // Didn't hit anything so don't draw a shadow
            field_18_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        }

        if (scale == Scale::Fg)
        {
            field_18_animation.mRenderLayer = Layer::eLayer_Shadow_26;
        }
        else
        {
            field_18_animation.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        }
    }
}

void Shadow::Render(PrimHeader** ppOt)
{
    if (field_14_flags.Get(Flags::eBit2_Enabled))
    {
        field_18_animation.field_14_scale = FP_FromInteger(1);
        if (field_10_scale == FP_FromDouble(0.5))
        {
            field_18_animation.mRed = 63;
            field_18_animation.mGreen = 63;
            field_18_animation.mBlue = 63;
        }
        else
        {
            field_18_animation.mRed = 127;
            field_18_animation.mGreen = 127;
            field_18_animation.mBlue = 127;
        }

        field_18_animation.VRender(
            // Note: OG converted to FP and back here but its pointless
            field_0_x1,
            field_2_y1,
            ppOt,
            (field_4_x2 - field_0_x1) + 1,
            (field_6_y2 - field_2_y1) + 1);

        PSX_RECT frameRect = {};
        field_18_animation.Get_Frame_Rect(&frameRect);
        pScreenManager->InvalidateRectCurrentIdx(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h);
    }
}

} // namespace AO
