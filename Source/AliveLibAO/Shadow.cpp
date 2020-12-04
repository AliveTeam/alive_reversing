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

EXPORT Shadow* Shadow::ctor_461FB0()
{
    SetVTable(&field_18_anim, 0x4BA2B8);
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, 1, 0);
    field_18_anim.Init_402D20(296, gObjList_animations_505564, 0, 37, 6, ppRes, 1, 0, 0);

    field_14_flags.Clear(Flags::eBit1_ShadowAtBottom);
    field_14_flags.Set(Flags::eBit2_Enabled);

    field_18_anim.field_B_render_mode = 2;

    field_18_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_18_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

    field_18_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_18_anim.field_4_flags.Set(AnimFlags::eBit8_Loop);
    field_18_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_18_anim.field_4_flags.Set(AnimFlags::eBit17_bFreeResource);
    field_18_anim.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    field_18_anim.field_4_flags.Set(AnimFlags::eBit20_use_xy_offset);
    field_18_anim.field_4_flags.Set(AnimFlags::eBit21);

    return this;
}

void Shadow::dtor_462030()
{
    field_18_anim.vCleanUp();
}

void Shadow::Calculate_Position_462040(FP xpos, FP ypos, PSX_RECT* frameRect, FP spriteScale)
{
    if (field_14_flags.Get(Flags::eBit2_Enabled))
    {
        // TODO: Is this the same as PsxToPCX ??
        const short objX = (23 * frameRect->x) / 40;
        const short objW = (23 * frameRect->w) / 40;

        FP objY = {};
        if (field_14_flags.Get(Flags::eBit1_ShadowAtBottom))
        {
            // Get the bottom of the object
            objY = pScreenManager_4FF7C8->field_10_pCamPos->field_4_y + FP_FromInteger(frameRect->h - pScreenManager_4FF7C8->field_16_ypos);
        }
        else
        {
            objY = ypos;
        }

        const int lineType = spriteScale != FP_FromDouble(0.5) ? 7 : 0x70;
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_504C6C->RayCast_40C410(
            xpos,
            objY,
            xpos,
            objY + FP_FromInteger(240),
            &pLine,
            &hitX,
            &hitY,
            lineType))
        {
            const FP camXPos = pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos);

            short lineXScreen = pLine->field_0_rect.x - FP_GetExponent(camXPos);
            short lineWScreen = pLine->field_0_rect.w - FP_GetExponent(camXPos);

            if (lineWScreen < lineXScreen)
            {
                lineXScreen = pLine->field_0_rect.w - FP_GetExponent(camXPos);
                lineWScreen = pLine->field_0_rect.x - FP_GetExponent(camXPos);
            }

            field_18_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

            field_8_xpos = xpos;
            field_C_ypos = hitY + FP_FromInteger(3);

            field_10_scale = (FP_FromInteger(1) - (((hitY - objY) * FP_FromDouble(0.75)) / FP_FromInteger(240)) * spriteScale);

            // Object is before the line we hit
            if (objX < lineXScreen)
            {
                if (sCollisions_DArray_504C6C->RayCast_40C410(
                    FP_NoFractional(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)) + FP_FromInteger(lineXScreen - 1) - FP_FromInteger(4),
                    hitY - FP_FromInteger(2),
                    FP_NoFractional(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)) + FP_FromInteger(lineXScreen - 1) - FP_FromInteger(4),
                    hitY + FP_FromInteger(2),
                    &pLine,
                    &hitX,
                    &hitY,
                    lineType))
                {
                    lineXScreen = std::min(pLine->field_0_rect.x, pLine->field_0_rect.w)
                        - FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos));
                }
            }

            // Object is after the line we hit
            if (objX > lineWScreen)
            {
                const FP v23 = FP_NoFractional(((pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)) + FP_FromInteger(lineWScreen + 1)))
                    + FP_FromInteger(4);

                if (sCollisions_DArray_504C6C->RayCast_40C410(
                    v23,
                    hitY - FP_FromInteger(2),
                    v23,
                    hitY + FP_FromInteger(2),
                    &pLine,
                    &hitX,
                    &hitY,
                    lineType))
                {
                    lineWScreen = std::max(pLine->field_0_rect.w, pLine->field_0_rect.x)
                        - FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos));
                }
            }

            field_0_x1 = std::max(objX, lineXScreen);
            field_4_x2 = std::min(objW, lineWScreen);

            short height;
            if (FP_GetExponent(field_10_scale * FP_FromInteger(6)) <= 6)
            {
                height = FP_GetExponent(field_10_scale * FP_FromInteger(6));
            }
            else
            {
                height = 6;
            }

            const short finalYPos = FP_GetExponent(field_C_ypos + FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y) - height / 2;
            field_2_y1 = finalYPos;
            field_6_y2 = finalYPos + height;
        }
        else
        {
            // Didn't hit anything so don't draw a shadow
            field_18_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        }

        if (spriteScale == FP_FromDouble(0.5))
        {
            field_18_anim.field_C_layer = 7;
        }
        else
        {
            field_18_anim.field_C_layer = 26;
        }
    }
}

void Shadow::Render_462410(int** ppOt)
{
    if (field_14_flags.Get(Flags::eBit2_Enabled))
    {
        field_18_anim.field_14_scale = FP_FromInteger(1);
        
        BYTE rgb = 63;
        if (field_10_scale != FP_FromDouble(0.5))
        {
            rgb = 127;
        }

        field_18_anim.field_8_r = rgb;
        field_18_anim.field_9_g = rgb;
        field_18_anim.field_A_b = rgb;

        field_18_anim.VRender_403AE0(
            // Note: OG converted to FP and back here but its pointless
            field_0_x1,
            field_2_y1,
            ppOt,
            (field_4_x2 - field_0_x1) + 1,
            (field_6_y2 - field_2_y1) + 1);

        PSX_RECT frameRect = {};
        field_18_anim.Get_Frame_Rect_402B50(&frameRect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager_4FF7C8->field_2E_idx);
    }
}

}
