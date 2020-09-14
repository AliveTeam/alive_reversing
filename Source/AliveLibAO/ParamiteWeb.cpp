#include "stdafx_ao.h"
#include "Function.hpp"
#include "ParamiteWeb.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "ResourceManager.hpp"
#include "Rope.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"

START_NS_AO

void ParamiteWeb::VUpdate()
{
    VUpdate_48AE70();
}

void ParamiteWeb::VScreenChanged_48AEA0()
{
    if (gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void ParamiteWeb::VScreenChanged()
{
    VScreenChanged_48AEA0();
}

BaseGameObject* ParamiteWeb::dtor_48AAB0()
{
    SetVTable(this, 0x4BD6F8);
    ao_delete_free_447540(field_EC_pRes);
    return dtor_417D10();
}

ParamiteWeb* ParamiteWeb::Vdtor_48AEC0(signed int flags)
{
    dtor_48AAB0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* ParamiteWeb::VDestructor(signed int flags)
{
    return Vdtor_48AEC0(flags);
}

ParamiteWeb* ParamiteWeb::ctor_48A920(FP xpos, __int16 bottom, __int16 top, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BD6F8);
    field_4_typeId = Types::eRope_73;

    field_C8_yOffset = 0;

    if (scale == FP_FromInteger(1))
    {
        field_E6_segment_length = 15;
    }
    else
    {
        field_E6_segment_length = 7;
    }

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2034, 1, 0);
    Animation_Init_417FD0(88, 3, 16, ppRes, 1);

    field_10_anim.field_14_scale = scale;
    field_BC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 24;
        field_C6_scale = 1;
    }
    else
    {
        field_10_anim.field_C_layer = 5;
        field_C6_scale = 0;
    }


    field_10_anim.field_8_r = 128;
    field_10_anim.field_9_g = 128;
    field_10_anim.field_A_b = 128;

    field_A8_xpos = xpos;
    field_EA_ttl_remainder = top;
    field_AC_ypos = FP_FromInteger(top);
    field_E8_ttl = bottom;

    field_E4_number_of_segments = 240 / field_E6_segment_length;

    field_EC_pRes = reinterpret_cast<AnimationUnknown*>(ao_new_malloc_447520(sizeof(AnimationUnknown) * field_E4_number_of_segments));

    if (field_EC_pRes)
    {
        for (int i = 0; i < field_E4_number_of_segments; i++)
        {
            AnimationUnknown* pSegment = &field_EC_pRes[i];
            SetVTable(pSegment, 0x4BA470);
            pSegment->field_4_flags.Set(AnimFlags::eBit3_Render);
            pSegment->field_68_anim_ptr = &field_10_anim;
            pSegment->field_C_layer = field_10_anim.field_C_layer;
            pSegment->field_6C_scale = field_BC_sprite_scale;
            pSegment->field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
            pSegment->field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        }
    }

    field_F0_bEnabled = 0;

    return this;
}

void ParamiteWeb::VUpdate_48AE70()
{
    if (field_F0_bEnabled == 1)
    {
        field_EA_ttl_remainder -= 8;
        if (field_EA_ttl_remainder <= field_E8_ttl)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

//TODO @ 100% - merge with Rope::vRender()
void ParamiteWeb::VRender_48AB10(int **pOt)
{
    NOT_IMPLEMENTED();
    PSX_Point camCoords = {};
    gMap_507BA8.GetCurrentCamCoords_444890(&camCoords);
    if (field_B2_lvl_number == gMap_507BA8.field_0_current_level && field_B0_path_number == gMap_507BA8.field_2_current_path)
    {
        if (field_A8_xpos >= FP_FromInteger(camCoords.field_0_x) && field_A8_xpos <= FP_FromInteger(camCoords.field_0_x + 1024))
        {

            const FP cam_y = pScreenManager_4FF7C8->field_10_pCamPos->field_4_y;
            const FP cam_x = pScreenManager_4FF7C8->field_10_pCamPos->field_0_x;

            short minY = pScreenManager_4FF7C8->field_16_ypos + FP_GetExponent(FP_FromInteger(field_E8_ttl) - cam_y);
            short maxY = pScreenManager_4FF7C8->field_16_ypos + FP_GetExponent(FP_FromInteger(field_EA_ttl_remainder) - cam_y);

            short ypos_int = FP_GetExponent(field_AC_ypos);
            if (ypos_int > field_EA_ttl_remainder)
            {
                ypos_int = field_EA_ttl_remainder + (ypos_int - field_EA_ttl_remainder) % field_E6_segment_length;
            }

            const short x_start = pScreenManager_4FF7C8->field_14_xpos + FP_GetExponent(field_A8_xpos - cam_x);

            short y_start = pScreenManager_4FF7C8->field_16_ypos + FP_GetExponent((FP_FromInteger(ypos_int)) - cam_y);
            if (field_C8_yOffset + y_start > 240)
            {
                y_start = y_start % field_E6_segment_length + 240;
                ypos_int = FP_GetExponent(cam_y + FP_FromInteger(y_start));
            }

            if (minY < 0)
            {
                minY = 0;
            }

            if (maxY > 240)
            {
                maxY = 240;
            }

            field_10_anim.VRender_403AE0(640, 240, pOt, 0, 0);

            if (y_start >= minY)
            {
                for(int idx = 0; idx < field_E4_number_of_segments; ++idx)
                {
                    short r = 128;
                    short g = 128;
                    short b = 128;
                    ShadowZone::ShadowZones_Calculate_Colour_435FF0(FP_GetExponent(field_A8_xpos), ypos_int - (idx * field_E6_segment_length), field_C6_scale, &r, &g, &b);
                    field_EC_pRes[idx].field_8_r = static_cast<BYTE>(r);
                    field_EC_pRes[idx].field_9_g = static_cast<BYTE>(g);
                    field_EC_pRes[idx].field_A_b = static_cast<BYTE>(b);
                    field_EC_pRes[idx].VRender2_403FD0(x_start, y_start + field_C8_yOffset, pOt);
                    PSX_RECT rect = {};
                    field_EC_pRes[idx].field_68_anim_ptr->Get_Frame_Rect_404220(&rect);
                    pScreenManager_4FF7C8->InvalidateRect_406E40(rect.x, rect.y, rect.w, rect.h, pScreenManager_4FF7C8->field_2E_idx);
                    ClipPoly_Vertically_4584B0(&field_EC_pRes[idx].field_10_polys[gPsxDisplay_504C78.field_A_buffer_index], field_C8_yOffset + minY, field_C8_yOffset + maxY);
                    y_start -= field_E6_segment_length;
                    if (y_start < minY)
                    {
                        break;
                    }
                }
            }
        }
    }
}

END_NS_AO

