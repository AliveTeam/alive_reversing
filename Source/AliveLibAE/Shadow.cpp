#include "stdafx.h"
#include "Shadow.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"

Shadow* Shadow::ctor_4AC990()
{
    SetVTable(&field_18_animation, 0x544290); // gVtbl_animation_2a_544290

    BYTE** ppAnimRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, TRUE, FALSE);
    field_18_animation.Init_40A030(240, gObjList_animations_5C1A24, 0, 64, 6, ppAnimRes, 1, 0, 0);

    field_14_flags.Clear(Flags::eBit1_ShadowAtBottom);
    field_14_flags.Set(Flags::eBit2_Enabled);

    field_18_animation.field_B_render_mode = 2;

    field_18_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_18_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

    field_18_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit17);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit20_use_xy_offset);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit21);

    return this;
}

void Shadow::dtor_4ACA30()
{
    field_18_animation.vCleanUp_40C630();
}

void Shadow::Calculate_Position_4ACA50(FP xpos, FP ypos, PSX_RECT* frameRect, FP spriteScale, __int16 scale)
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
            objY = FP_FromInteger(frameRect->h) + pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;
        }
        else
        {
            objY = ypos;
        }

        const int lineType = scale != 0 ? 0xF : 0xF0;
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            xpos,
            objY,
            xpos,
            objY + FP_FromInteger(240),
            &pLine,
            &hitX,
            &hitY,
            lineType))
        {
            const short camXPos = FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
            short lineXScreen = pLine->field_0_rect.x - camXPos;
            short lineWScreen = pLine->field_0_rect.w - camXPos;

            if (lineWScreen < lineXScreen)
            {
                lineXScreen = pLine->field_0_rect.w - camXPos;
                lineWScreen = pLine->field_0_rect.x - camXPos;
            }

            field_18_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

            field_8_xpos = xpos;
            field_C_ypos = hitY;

            field_10_scale = (FP_FromInteger(1) - (((hitY - objY) *  FP_FromDouble(0.75)) / FP_FromInteger(240))) * spriteScale;

            // Object is before the line we hit
            if (objX < lineXScreen)
            {
                if (sCollisions_DArray_5C1128->Raycast_417A60(
                    FP_NoFractional(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x) + (FP_FromInteger(lineXScreen - 1)) - FP_FromInteger(4),
                    hitY - FP_FromInteger(2),
                    FP_NoFractional(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x) + (FP_FromInteger(lineXScreen - 1)) - FP_FromInteger(4),
                    hitY + FP_FromInteger(2),
                    &pLine,
                    &hitX,
                    &hitY,
                    lineType))
                {
                    lineXScreen = std::min(pLine->field_0_rect.x, pLine->field_0_rect.w) - FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
                }
            }

            // Object is after the line we hit
            if (objW > lineWScreen)
            {
                if (sCollisions_DArray_5C1128->Raycast_417A60(
                    FP_NoFractional(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x) + (FP_FromInteger(lineWScreen + 1)) + FP_FromInteger(4),
                    hitY - FP_FromInteger(2),
                    FP_NoFractional(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x) + (FP_FromInteger(lineWScreen + 1)) + FP_FromInteger(4),
                    hitY + FP_FromInteger(2),
                    &pLine,
                    &hitX,
                    &hitY,
                    lineType))
                {
                    lineWScreen = std::max(pLine->field_0_rect.x, pLine->field_0_rect.w) - FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
                }
            }


            field_0_x1 = std::max(objX, lineXScreen);
            field_4_x2 = std::min(objW, lineWScreen);

            const short temp = FP_GetExponent(field_10_scale * FP_FromInteger(6));
            short height;
            if (temp <= 6)
            {
                height = temp;
            }
            else
            {
                height = 6;
            }

            const short finalYPos = FP_GetExponent(field_C_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y) - height / 2;
            field_2_y1 = finalYPos;
            field_6_y2 = height + finalYPos;
        }
        else
        {
            // Didn't hit anything so don't draw a shadow
            field_18_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        }

        if (scale)
        {
            field_18_animation.field_C_render_layer = Layer::eLayer_26;
        }
        else
        {
            field_18_animation.field_C_render_layer = Layer::eLayer_7;
        }
    }
}

void Shadow::Render_4ACE60(PrimHeader** ppOt)
{
    if (field_14_flags.Get(Flags::eBit2_Enabled))
    {
        field_18_animation.field_14_scale = FP_FromInteger(1);
        if (field_10_scale == FP_FromDouble(0.5))
        {
            field_18_animation.field_8_r = 63;
            field_18_animation.field_9_g = 63;
            field_18_animation.field_A_b = 63;
        }
        else
        {
            field_18_animation.field_8_r = 127;
            field_18_animation.field_9_g = 127;
            field_18_animation.field_A_b = 127;
        }

        field_18_animation.vRender_40B820(
            // Note: OG converted to FP and back here but its pointless
            field_0_x1,
            field_2_y1,
            ppOt,
            (field_4_x2 - field_0_x1) + 1,
            (field_6_y2 - field_2_y1) + 1);

        PSX_RECT frameRect = {};
        field_18_animation.Get_Frame_Rect_409E10(&frameRect);
        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager_5BB5F4->field_3A_idx);
    }
}
