#include "stdafx.h"
#include "Rope.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "Animation.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"

const TintEntry kRopeTints_55FD68[18] = {
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_6, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_9, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};

EXPORT void CC ClipPoly_Vertically_4A09E0(Poly_FT4* pPoly, s32 minY, s32 maxY)
{
    const s32 d1 = minY - Y0(pPoly);
    const s16 polyHeight = Y3(pPoly) - Y0(pPoly);
    if (minY - Y0(pPoly) > 0 && d1 <= polyHeight)
    {
        SetXY0(pPoly, X0(pPoly), Y0(pPoly) + (s16) d1);
        SetXY1(pPoly, X1(pPoly), Y1(pPoly) + (s16) d1);

        SetUV0(pPoly, U0(pPoly), V0(pPoly) + (u8) d1);
        SetUV1(pPoly, U1(pPoly), V1(pPoly) + (u8) d1);
    }

    const s32 d2 = Y3(pPoly) - maxY;
    if (d2 > 0 && d2 <= polyHeight)
    {
        SetXY2(pPoly, X2(pPoly), Y2(pPoly) - (s16) d2);
        SetXY3(pPoly, X3(pPoly), Y3(pPoly) - (s16) d2);

        SetUV2(pPoly, U2(pPoly), V2(pPoly) - (u8) d2);
        SetUV3(pPoly, U3(pPoly), V3(pPoly) - (u8) d2);
    }
}

Rope* Rope::ctor_4A0A70(u16 left, s16 top, u16 bottom, FP scale)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x546C70); // vTbl_LiftRope_00546C70

    SetType(AETypes::eLiftRope_108);

    const AnimRecord& rec = AnimRec(AnimId::Lift_Rope);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    SetTint_425600(kRopeTints_55FD68, gMap_5C3030.field_0_current_level);

    field_20_animation.field_14_scale = scale;
    field_CC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_F6_rope_length = 15;
        field_20_animation.field_C_render_layer = Layer::eLayer_RopeWebDrill_24;
        field_D6_scale = 1;
    }
    else
    {
        field_F6_rope_length = 7;
        field_20_animation.field_C_render_layer = Layer::eLayer_RopeWebDrill_Half_5;
        field_20_animation.field_14_scale = FP_FromDouble(0.7);
        field_CC_sprite_scale = FP_FromDouble(0.7);
        field_D6_scale = 0;
    };

    field_20_animation.field_8_r = static_cast<u8>(field_D0_r);
    field_20_animation.field_9_g = static_cast<u8>(field_D2_g);
    field_20_animation.field_A_b = static_cast<u8>(field_D4_b);

    field_102_top = top;
    field_106_bottom = bottom;
    field_100_left = left;
    field_104_right = left;

    field_B8_xpos = FP_FromInteger(left);
    field_BC_ypos = FP_FromInteger(bottom);

    field_F4_rope_segment_count = (240 / field_F6_rope_length) + 1; // psx screen height

    field_F8_ppRopeRes = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_Rope, 0, field_F4_rope_segment_count * sizeof(AnimationUnknown));
    field_FC_pRopeRes = reinterpret_cast<AnimationUnknown*>(*field_F8_ppRopeRes);

    for (s32 i = 0; i < field_F4_rope_segment_count; i++)
    {
        AnimationUnknown* pSegment = &field_FC_pRopeRes[i];
        pSegment = new (pSegment) AnimationUnknown(); // We have memory but no constructor was called.. so use placement new to get a constructed instance
        SetVTable(pSegment, 0x5447CC);                // vTbl_RopeSegment_5447CC
        pSegment->field_4_flags.Set(AnimFlags::eBit3_Render);
        pSegment->field_68_anim_ptr = &field_20_animation;
        pSegment->field_C_render_layer = field_20_animation.field_C_render_layer;
        pSegment->field_6C_scale = scale;
        pSegment->field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        pSegment->field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    }

    return this;
}

BaseGameObject* Rope::VDestructor(s32 flags)
{
    return vdtor_4A0D80(flags);
}

void Rope::VUpdate()
{
    // nullsub@4A11E0
}

void Rope::VRender(PrimHeader** ppOt)
{
    vRender_4A0E30(ppOt);
}

void Rope::dtor_4A0DB0()
{
    SetVTable(this, 0x546C70); // vTbl_LiftRope_00546C70
    ResourceManager::FreeResource_49C330(field_F8_ppRopeRes);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

Rope* Rope::vdtor_4A0D80(s32 flags)
{
    dtor_4A0DB0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

EXPORT void Rope::vRender_4A0E30(PrimHeader** ppOt)
{
    PSX_Point camPos = {};
    gMap_5C3030.GetCurrentCamCoords_480680(&camPos);
    // In the current level/map?
    if (field_C2_lvl_number == gMap_5C3030.field_0_current_level && field_C0_path_number == gMap_5C3030.field_2_current_path)
    {
        // In the current camera x range?
        if (field_B8_xpos >= FP_FromInteger(camPos.field_0_x) && field_B8_xpos <= FP_FromInteger(camPos.field_0_x + 375))
        {
            const FP camXPos = pScreenManager_5BB5F4->field_20_pCamPos->field_0_x;
            const FP camYPos = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;

            s16 minY = FP_GetExponent(FP_FromInteger(field_102_top) - camYPos);
            s16 maxY = FP_GetExponent(FP_FromInteger(field_106_bottom) - camYPos);

            s16 ypos = FP_GetExponent(field_BC_ypos);
            if (ypos > field_106_bottom)
            {
                ypos = field_106_bottom + ((ypos - field_106_bottom) % field_F6_rope_length);
            }

            s16 screenX = FP_GetExponent(field_B8_xpos - camXPos);
            s16 screenY = ypos - FP_GetExponent(camYPos);

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

            field_20_animation.vRender_40B820(640, 240, ppOt, 0, 0);

            if (screenY >= minY)
            {
                for (s32 idx = 0; idx < field_F4_rope_segment_count; idx++)
                {
                    // Apply shadow to the segments colour
                    s16 r = field_D0_r;
                    s16 g = field_D2_g;
                    s16 b = field_D4_b;

                    ShadowZone::ShadowZones_Calculate_Colour_463CE0(
                        FP_GetExponent(field_B8_xpos),
                        ypos - (idx * field_F6_rope_length),
                        field_D6_scale,
                        &r,
                        &g,
                        &b);

                    field_FC_pRopeRes[idx].field_8_r = static_cast<u8>(r);
                    field_FC_pRopeRes[idx].field_9_g = static_cast<u8>(g);
                    field_FC_pRopeRes[idx].field_A_b = static_cast<u8>(b);

                    // Render the segment
                    field_FC_pRopeRes[idx].vRender_40B820(
                        screenX,
                        screenY,
                        ppOt,
                        0,
                        0);

                    PSX_RECT rect = {};
                    field_FC_pRopeRes[idx].GetRenderedSize_40C980(&rect);
                    pScreenManager_5BB5F4->InvalidateRect_40EC90(
                        rect.x,
                        rect.y,
                        rect.w,
                        rect.h,
                        pScreenManager_5BB5F4->field_3A_idx);

                    ClipPoly_Vertically_4A09E0(
                        &field_FC_pRopeRes[idx].field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index],
                        minY,
                        maxY);

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
