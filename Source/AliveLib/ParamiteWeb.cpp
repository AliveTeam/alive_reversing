#include "stdafx.h"
#include "ParamiteWeb.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Rope.hpp"
#include "ShadowZone.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"

ParamiteWeb* ParamiteWeb::ctor_4E1840(FP xpos, __int16 bottom, __int16 top, FP scale)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x547F58);
    field_4_typeId = Types::eLiftRope_108;

    if (scale == FP_FromInteger(1))
    {
        field_F6 = 15;
    }
    else
    {
        field_F6 = 7;
    }
    Animation_Init_424E10(148, 5, 16, Add_Resource_4DC130(ResourceManager::Resource_Animation, kWebResID), 1, 1u);

    if (scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 24;
        field_D6_scale = 1;
        field_20_animation.field_14_scale = FP_FromInteger(1);
        field_CC_sprite_scale = FP_FromInteger(1);
    }
    else
    {
        field_20_animation.field_C_render_layer = 5;
        field_20_animation.field_14_scale = FP_FromDouble(0.7);
        field_CC_sprite_scale = FP_FromDouble(0.7);
        field_D6_scale = 0;
        xpos += FP_FromInteger(2);
    }

    field_20_animation.field_8_r = 128;
    field_20_animation.field_9_g = 128;
    field_20_animation.field_A_b = 128;

    field_B8_xpos = xpos;
    field_FA_ttl_remainder = top;
    field_BC_ypos = FP_FromInteger(top);
    field_F8_ttl = bottom;

    field_F4 = 240 / field_F6;

    field_FC = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_Web, 0, sizeof(AnimationUnknown) * (field_F4));
    field_100 = reinterpret_cast<AnimationUnknown*>(*field_FC);

    for (int i = 0; i < field_F4; i++)
    {
        AnimationUnknown* pSegment = &field_100[i];
        pSegment = new (pSegment) AnimationUnknown(); // We have memory but no constructor was called.. so use placement new to get a constructed instance
        SetVTable(pSegment, 0x5447CC);
        pSegment->field_4_flags.Set(AnimFlags::eBit3_Render);
        pSegment->field_68_anim_ptr = &field_20_animation;
        pSegment->field_C_render_layer = field_20_animation.field_C_render_layer;
        pSegment->field_6C_scale = field_CC_sprite_scale;
        pSegment->field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        pSegment->field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    }

    field_104_bEnabled = 0;

    return this;
}

void ParamiteWeb::VUpdate()
{
    vUpdate_4E1F40();
}

void ParamiteWeb::VScreenChanged()
{
    vScreenChanged_4E1F80();
}

void ParamiteWeb::VRender(int** pOrderingTable)
{
    vRender_4E1BA0(pOrderingTable);
}

BaseGameObject* ParamiteWeb::VDestructor(signed int flags)
{
    return vdtor_4E1AF0(flags);
}

ParamiteWeb* ParamiteWeb::vdtor_4E1AF0(signed int flags)
{
    dtor_4E1B20();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ParamiteWeb::dtor_4E1B20()
{
    SetVTable(this, 0x547F58);
    ResourceManager::FreeResource_49C330(field_FC);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void ParamiteWeb::vUpdate_4E1F40()
{
    if (field_104_bEnabled == 1)
    {
        field_FA_ttl_remainder -= 8;
        if (field_FA_ttl_remainder <= field_F8_ttl)
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }
}

void ParamiteWeb::vScreenChanged_4E1F80()
{
    if (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void ParamiteWeb::vRender_4E1BA0(int** pOt)
{
    PSX_Point camCoords = {};
    gMap_5C3030.GetCurrentCamCoords_480680(&camCoords);
    if (field_C2_lvl_number == gMap_5C3030.sCurrentLevelId_5C3030 && field_C0_path_number == gMap_5C3030.sCurrentPathId_5C3032)
    {
        if (field_B8_xpos >= FP_FromInteger(camCoords.field_0_x) && field_B8_xpos <= FP_FromInteger(camCoords.field_0_x + 1024))
        {
            const FP cam_y = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;
            const FP cam_x = pScreenManager_5BB5F4->field_20_pCamPos->field_0_x;

            short minY = FP_GetExponent(FP_FromInteger(field_F8_ttl) - cam_y);
            short maxY = FP_GetExponent(FP_FromInteger(field_FA_ttl_remainder) - cam_y);
            
            short ypos_int = FP_GetExponent(field_BC_ypos);
            if (ypos_int > field_FA_ttl_remainder)
            {
                ypos_int = field_FA_ttl_remainder + (ypos_int - field_FA_ttl_remainder) % field_F6;
            }

            const short x_start = FP_GetExponent(field_B8_xpos - cam_x);
            
            short y_start = FP_GetExponent((FP_FromInteger(ypos_int)) - cam_y);
            if (y_start > 240)
            {
                y_start = y_start % field_F6 + 240;
                ypos_int = FP_GetExponent(cam_y + FP_FromInteger(y_start));
            }

            int idx = 0;
            if (minY < 0)
            {
                minY = 0;
            }

            if (maxY > 240)
            {
                maxY = 240;
            }

            field_20_animation.vRender_40B820(640, 240, pOt, 0, 0);

            if (y_start >= minY)
            {
                while (idx <field_F4)
                {
                    short r = 128;
                    short g = 128;
                    short b = 128;
                    ShadowZone::ShadowZones_Calculate_Colour_463CE0(FP_GetExponent(field_B8_xpos), ypos_int - (idx * field_F6), field_D6_scale, &r, &g, &b);
                    field_100[idx].field_8_r = static_cast<BYTE>(r);
                    field_100[idx].field_9_g = static_cast<BYTE>(g);
                    field_100[idx].field_A_b = static_cast<BYTE>(b);
                    field_100[idx].vRender_40B820(x_start, y_start, pOt, 0, 0);
                    PSX_RECT rect = {};
                    field_100[idx].GetRenderedSize_40C980(&rect);
                    pScreenManager_5BB5F4->InvalidateRect_40EC90(rect.x, rect.y, rect.w, rect.h, pScreenManager_5BB5F4->field_3A_idx);
                    ClipPoly_Vertically_4A09E0(&field_100[idx].field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index], minY, maxY);
                    y_start -= field_F6;
                    idx++;
                    if (y_start < minY)
                    {
                        break;
                    }
                }
            }
        }
    }
}
