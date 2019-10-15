#include "stdafx.h"
#include "Rope.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "Animation.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"

const TintEntry kRopeTints_55FD68[18] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 127u, 127u, 127u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};

EXPORT void CC ClipPoly_Vertically_4A09E0(Poly_FT4* pPoly, int minY, int maxY)
{
    const int d1 = minY - Y0(pPoly);
    const short polyHeight = Y3(pPoly) - Y0(pPoly);
    if (minY - Y0(pPoly) > 0 && d1 <= polyHeight)
    {
        SetXY0(pPoly, X0(pPoly), Y0(pPoly) + (short)d1);
        SetXY1(pPoly, X1(pPoly), Y1(pPoly) + (short)d1);

        SetUV0(pPoly, U0(pPoly), V0(pPoly) + (BYTE)d1);
        SetUV1(pPoly, U1(pPoly), V1(pPoly) + (BYTE)d1);
    }

    const int d2 = Y3(pPoly) - maxY;
    if (d2 > 0 && d2 <= polyHeight)
    {
        SetXY2(pPoly, X2(pPoly), Y2(pPoly) - (short)d2);
        SetXY3(pPoly, X3(pPoly), Y3(pPoly) - (short)d2);

        SetUV2(pPoly, U2(pPoly), V2(pPoly) - (BYTE)d2);
        SetUV3(pPoly, U3(pPoly), V3(pPoly) - (BYTE)d2);
    }
}

Rope* Rope::ctor_4A0A70(unsigned __int16 left, __int16 top, unsigned __int16 bottom, FP scale)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x546C70); // vTbl_LiftRope_00546C70

    field_4_typeId = Types::eLiftRope_108;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kRopesResID);
    Animation_Init_424E10(748, 9, 16, ppRes, 1, 1);
    SetTint_425600(kRopeTints_55FD68, gMap_5C3030.sCurrentLevelId_5C3030);

    field_20_animation.field_14_scale = scale;
    field_CC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_F6_rope_length = 15;
        field_20_animation.field_C_render_layer = 24;
        field_D6_scale = 1;
    }
    else
    {
        field_F6_rope_length = 7;
        field_20_animation.field_C_render_layer = 5;
        field_20_animation.field_14_scale = FP_FromDouble(0.7);
        field_CC_sprite_scale = FP_FromDouble(0.7);
        field_D6_scale = 0;
    };

    field_20_animation.field_8_r = static_cast<BYTE>(field_D0_r);
    field_20_animation.field_9_g = static_cast<BYTE>(field_D2_g);
    field_20_animation.field_A_b = static_cast<BYTE>(field_D4_b);

    field_102_top = top;
    field_106_bottom = bottom;
    field_100_left = left;
    field_104_right = left;

    field_B8_xpos = FP_FromInteger(left);
    field_BC_ypos = FP_FromInteger(bottom);

    field_F4_rope_segment_count = (240 / field_F6_rope_length) + 1; // psx screen height

    field_F8_ppRopeRes = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_Rope, 0, field_F4_rope_segment_count * sizeof(AnimationUnknown));
    field_FC_pRopeRes = reinterpret_cast<AnimationUnknown*>(*field_F8_ppRopeRes);

    for (int i = 0; i < field_F4_rope_segment_count; i++)
    {
        AnimationUnknown* pSegment = &field_FC_pRopeRes[i];
        pSegment = new (pSegment) AnimationUnknown(); // We have memory but no constructor was called.. so use placement new to get a constructed instance
        SetVTable(pSegment, 0x5447CC); // vTbl_RopeSegment_5447CC
        pSegment->field_4_flags.Set(AnimFlags::eBit3_Render);
        pSegment->field_68_anim_ptr = &field_20_animation;
        pSegment->field_C_render_layer = field_20_animation.field_C_render_layer;
        pSegment->field_6C_scale = scale;
        pSegment->field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        pSegment->field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    }

    return this;
}

BaseGameObject* Rope::VDestructor(signed int flags)
{
    return vdtor_4A0D80(flags);
}

void Rope::VUpdate()
{
    // nullsub@4A11E0
}

void Rope::VRender(int** pOrderingTable)
{
    vRender_4A0E30(pOrderingTable);
}

void Rope::dtor_4A0DB0()
{
    SetVTable(this, 0x546C70); // vTbl_LiftRope_00546C70
    ResourceManager::FreeResource_49C330(field_F8_ppRopeRes);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

Rope* Rope::vdtor_4A0D80(signed int flags)
{
    dtor_4A0DB0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

EXPORT void Rope::vRender_4A0E30(int** pOt)
{
    PSX_Point camPos = {};
    gMap_5C3030.GetCurrentCamCoords_480680(&camPos);
    // In the current level/map?
    if (field_C2_lvl_number == gMap_5C3030.sCurrentLevelId_5C3030 && field_C0_path_number == gMap_5C3030.sCurrentPathId_5C3032)
    {
        // In the current camera x range?
        if (field_B8_xpos >= FP_FromInteger(camPos.field_0_x) && field_B8_xpos <= FP_FromInteger(camPos.field_0_x + 375))
        {
            const FP camYPos = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;
            const FP camXPos = pScreenManager_5BB5F4->field_20_pCamPos->field_0_x;

            __int16 minY = FP_GetExponent(FP_FromInteger(field_102_top) - camYPos);
            __int16 maxY = FP_GetExponent(FP_FromInteger(field_106_bottom) - camYPos);

            __int16 ypos = FP_GetExponent(field_BC_ypos);
            if (ypos > field_106_bottom)
            {
                ypos = field_106_bottom + ((ypos - field_106_bottom) % field_F6_rope_length);
            }

            short screenX = FP_GetExponent(field_B8_xpos - camXPos);
            short screenY = ypos - FP_GetExponent(camYPos);

            if (screenY > 240)
            {
                screenY = (screenY % field_F6_rope_length) + 240;
                ypos = FP_GetExponent(camYPos + (FP_FromInteger(screenY)));
            }

            if (minY < 0)
            {
                minY = 0;
            }

            if (maxY > 240)
            {
                maxY = 240;
            }

            field_20_animation.vRender_40B820(640, 240, pOt, 0, 0);

            if (screenY >= minY)
            {
                for (int idx = 0; idx < field_F4_rope_segment_count; idx++)
                {
                    // Apply shadow to the segments colour
                    short r = field_D0_r;
                    short g = field_D2_g;
                    short b = field_D4_b;

                    ShadowZone::ShadowZones_Calculate_Colour_463CE0(
                        FP_GetExponent(field_B8_xpos),
                        ypos - (idx * field_F6_rope_length),
                        field_D6_scale,
                        &r,
                        &g,
                        &b);

                    field_FC_pRopeRes[idx].field_8_r = static_cast<BYTE>(r);
                    field_FC_pRopeRes[idx].field_9_g = static_cast<BYTE>(g);
                    field_FC_pRopeRes[idx].field_A_b = static_cast<BYTE>(b);

                    // Render the segment
                    PSX_RECT rect = {};
                    field_FC_pRopeRes[idx].vRender_40B820(screenX, screenY, pOt, 0, 0);
                    field_FC_pRopeRes[idx].GetRenderedSize_40C980(&rect);
                    pScreenManager_5BB5F4->InvalidateRect_40EC90(
                        rect.x,
                        rect.y,
                        rect.w,
                        rect.h,
                        pScreenManager_5BB5F4->field_3A_idx);

                    ClipPoly_Vertically_4A09E0(&field_FC_pRopeRes[idx].field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index], minY, maxY);

                    screenY -= field_F6_rope_length;

                    if (screenY < minY)
                    {
                        break;
                    }
                }
            }
        }
    }
}
