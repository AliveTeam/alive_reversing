#include "stdafx.h"
#include "ParamiteWebLine.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "PsxDisplay.hpp"
#include "ShadowZone.hpp"
#include "Rope.hpp"

ParamiteWebLine* ParamiteWebLine::ctor_4E1FC0(Path_ParamiteWebLine* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930( 0);
    SetVTable(&field_108_anim_flare, 0x544290);
    SetVTable(this, 0x547FA0);
    field_4_typeId = Types::eWebLine_146;
    field_100 = tlvInfo;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, kWebResID);
    Animation_Init_424E10(148, 5, 16u, ppRes, 1, 1u);

    if (pTlv->field_10_scale)
    {
        field_20_animation.field_C_render_layer = 5;
        field_20_animation.field_14_scale = FP_FromDouble(0.7);
        field_CC_sprite_scale = FP_FromDouble(0.7);
        field_D6_scale = 0;
        field_F6_piece_length = 7;
        field_1A2 = 1;
        field_D0_r = 50;
        field_D2_g = 50;
        field_D4_b = 200;
    }
    else
    {
        field_20_animation.field_C_render_layer = 24;
        field_20_animation.field_14_scale = FP_FromInteger(1);
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_F6_piece_length = 15;
        field_1A2 = 2;
        field_D0_r = 10;
        field_D2_g = 10;
        field_D4_b = 10;
    }

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos + FP_FromInteger(20),
        field_BC_ypos + FP_FromInteger(20),
        &pLine, &hitX, &hitY, 0x100))
    {
        field_B8_xpos = FP_FromInteger(pLine->field_0_rect.x);

        const FP screenTop = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;
        if (FP_FromInteger(pLine->field_0_rect.y) >= screenTop)
        {
            field_F8_top = pLine->field_0_rect.y;
        }
        else
        {
            field_F8_top = FP_GetExponent(screenTop);
        }

        const FP screenBottom = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(240);
        if (FP_FromInteger(pLine->field_0_rect.h) <= screenBottom)
        {
            field_FA_bottom = pLine->field_0_rect.h;
        }
        else
        {
            field_FA_bottom = FP_GetExponent(screenBottom);
        }
        field_BC_ypos = FP_FromInteger(field_FA_bottom);
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    field_F4_anim_segment_count = (field_F6_piece_length + field_FA_bottom - field_F8_top) / field_F6_piece_length;
    field_FC = reinterpret_cast<AnimationUnknown*>(malloc_non_zero_4954F0(sizeof(AnimationUnknown) * field_F4_anim_segment_count));
    for (int i = 0; i < field_F4_anim_segment_count; i++)
    {
        new (&field_FC[i]) AnimationUnknown(); // We have memory but no constructor was called.. so use placement new to get a constructed instance
        SetVTable(&field_FC[i], 0x5447CC);

        field_FC[i].field_4_flags.Set(AnimFlags::eBit3_Render);
        field_FC[i].field_68_anim_ptr = &field_20_animation;
        field_FC[i].field_C_render_layer = field_20_animation.field_C_render_layer;
        field_FC[i].field_6C_scale = field_CC_sprite_scale;
        field_FC[i].field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_FC[i].field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    }

    field_104_wobble_idx = 0;
    field_106_wobble_pos = field_F8_top;

    BYTE** ppFlareRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kOmmflareResID, 0, 0);
    if (field_108_anim_flare.Init_40A030(1632, gObjList_animations_5C1A24, this, 39, 21u, ppFlareRes, 1u, 0, 0))
    {
        field_108_anim_flare.field_8_r = 100;
        field_108_anim_flare.field_9_g = 100;
        field_108_anim_flare.field_A_b = 100;

        field_108_anim_flare.field_C_render_layer = field_20_animation.field_C_render_layer;
        field_108_anim_flare.field_14_scale = FP_FromDouble(0.3);
        field_108_anim_flare.field_B_render_mode = 1;

        field_108_anim_flare.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_108_anim_flare.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        field_108_anim_flare.field_4_flags.Set(AnimFlags::eBit7_SwapXY);

        field_1A0 = field_F8_top;
        field_1A4_delay_counter = Math_RandomRange_496AB0(0, 10);
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eListAddFailed);
    }
    return this;
}

void ParamiteWebLine::Wobble_4E29D0(short ypos)
{
    short yPosToUse = ypos;
    if (ypos < field_F8_top)
    {
        yPosToUse = field_F8_top;
    }

    field_104_wobble_idx = 7;

    if (yPosToUse <= field_FA_bottom)
    {
        field_106_wobble_pos = yPosToUse;
    }
    else
    {
        field_106_wobble_pos = field_FA_bottom;
    }
}

void ParamiteWebLine::VUpdate()
{
    vUpdate_4E2A50();
}

void ParamiteWebLine::VScreenChanged()
{
    return vScreenChanged_4E2BC0();
}

BaseGameObject* ParamiteWebLine::VDestructor(signed int flags)
{
    return vdtor_4E2460(flags);
}

PSX_RECT* ParamiteWebLine::vGetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx)
{
    return vGetBoundingRect_4E2B40(pRect, pointIdx);
}

void ParamiteWebLine::VRender(int** pOrderingTable)
{
    vRender_4E2530(pOrderingTable);
}

ParamiteWebLine* ParamiteWebLine::vdtor_4E2460(signed int flags)
{
    dtor_4E2490();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ParamiteWebLine::dtor_4E2490()
{
    SetVTable(this, 0x547FA0);
    Mem_Free_495560(field_FC);
    field_108_anim_flare.vCleanUp_40C630();
    Path::TLV_Reset_4DB8E0(field_100, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void ParamiteWebLine::vUpdate_4E2A50()
{
    if (field_104_wobble_idx > 0)
    {
        field_104_wobble_idx--;
        field_1A0 = field_F8_top;
        return;
    }

    if (field_1A4_delay_counter > 0)
    {
        field_1A4_delay_counter--;
    }
    else
    {
        field_1A0 += field_1A2;
        if (field_1A0 >field_FA_bottom)
        {
            field_1A0 = field_F8_top;
            SFX_Play_46FA90(static_cast<char>(Math_RandomRange_496AB0(103, 104)), Math_RandomRange_496AB0(40, 80));
            field_106_wobble_pos = field_F8_top;
            return;
        }
    }
    field_106_wobble_pos = field_F8_top;
}

PSX_RECT* ParamiteWebLine::vGetBoundingRect_4E2B40(PSX_RECT* pRect, int /*idx*/)
{
    const short xpos = FP_GetExponent(field_B8_xpos);

    pRect->x = xpos - 2;
    pRect->y = field_F8_top;

    pRect->w = xpos + 2;
    pRect->h = field_FA_bottom;

    return pRect;
}

const __int16 word_563A8C[10] = { 0, 1, -3, 2, -4, 4, -6, 4, 0, 0 };

void ParamiteWebLine::vRender_4E2530(int** ppOt)
{
    if (!field_104_wobble_idx && !field_1A4_delay_counter)
    {
        field_108_anim_flare.vRender_40B820(
            FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
            FP_GetExponent(FP_FromInteger(field_1A0) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
            ppOt, 0, 0);
        PSX_RECT rect = {};
        field_108_anim_flare.Get_Frame_Rect_409E10(&rect);
        pScreenManager_5BB5F4->InvalidateRect_40EC90(rect.x, rect.y, rect.w, rect.h, pScreenManager_5BB5F4->field_3A_idx);
    }
    
    field_20_animation.vRender_40B820(640, 240, ppOt, 0, 0);

    int idx = 0;
    __int16 render_ypos = field_FA_bottom;
    while (render_ypos > field_F8_top)
    {
        short r = field_D0_r;
        short g = field_D2_g;
        short b = field_D4_b;

        FP xVal = field_B8_xpos;
        if (field_104_wobble_idx != 0)
        {
            if (render_ypos >= field_106_wobble_pos)
            {
                if (field_106_wobble_pos == field_FA_bottom)
                {
                    break;
                }
                const FP bottomDiff = FP_FromInteger(field_FA_bottom - field_106_wobble_pos);
                const FP ang = FP_FromInteger(field_FA_bottom - render_ypos) * FP_FromInteger(128);
                xVal = field_B8_xpos - (Math_Cosine_496D60(ang / bottomDiff) * FP_FromInteger(word_563A8C[field_104_wobble_idx]));
            }
            else
            {
                if (field_106_wobble_pos == field_F8_top)
                {
                    break;
                }
                const FP topDiff = FP_FromInteger(field_106_wobble_pos - field_F8_top);
                const FP ang = FP_FromInteger(field_106_wobble_pos - render_ypos) * FP_FromInteger(128);
                xVal = (Math_Cosine_496D60(ang / topDiff) * FP_FromInteger(word_563A8C[field_104_wobble_idx])) + field_B8_xpos;
            }
        }

        const short render_xpos = FP_GetExponent(xVal);
        ShadowZone::ShadowZones_Calculate_Colour_463CE0(render_xpos, render_ypos, field_D6_scale, &r, &g, &b);
        field_FC[idx].field_8_r = static_cast<BYTE>(r);
        field_FC[idx].field_9_g = static_cast<BYTE>(g);
        field_FC[idx].field_A_b = static_cast<BYTE>(b);

        field_FC[idx].vRender_40B820(
            FP_GetExponent(FP_FromInteger(render_xpos) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
            FP_GetExponent(FP_FromInteger(render_ypos) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
            ppOt, 0, 0);

        PSX_RECT rect = {};
        field_FC[idx].GetRenderedSize_40C980(&rect);
        pScreenManager_5BB5F4->InvalidateRect_40EC90(rect.x, rect.y, rect.w, rect.h, pScreenManager_5BB5F4->field_3A_idx);

        ClipPoly_Vertically_4A09E0(
            &field_FC[idx].field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index],
            FP_GetExponent(FP_FromInteger(field_F8_top) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
            FP_GetExponent(FP_FromInteger(field_FA_bottom) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y));

        render_ypos -= field_F6_piece_length;
        idx++;
    }
}

void ParamiteWebLine::vScreenChanged_4E2BC0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}
