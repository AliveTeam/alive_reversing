#include "stdafx.h"
#include "ParamiteWeb.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Rope.hpp"
#include "ShadowZone.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"

ParamiteWeb::ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eLiftRope_108);

    if (scale == FP_FromInteger(1))
    {
        field_F6_segment_length = 15;
    }
    else
    {
        field_F6_segment_length = 7;
    }

    const AnimRecord& rec = AnimRec(AnimId::ParamiteWeb);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    if (scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_RopeWebDrill_24;
        field_D6_scale = 1;
        field_20_animation.field_14_scale = FP_FromInteger(1);
        field_CC_sprite_scale = FP_FromInteger(1);
    }
    else
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_RopeWebDrill_Half_5;
        field_20_animation.field_14_scale = FP_FromDouble(0.7);
        field_CC_sprite_scale = FP_FromDouble(0.7);
        field_D6_scale = 0;
        xpos += FP_FromInteger(2);
    }

    field_20_animation.field_8_r = 128;
    field_20_animation.field_9_g = 128;
    field_20_animation.field_A_b = 128;

    field_B8_xpos = xpos;
    field_FA_ttl_remainder = static_cast<s16>(top);
    field_BC_ypos = FP_FromInteger(top);
    field_F8_ttl = static_cast<s16>(bottom);

    field_F4_number_of_segments = 240 / field_F6_segment_length;

    field_FC_ppRes = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_Web, 0, sizeof(AnimationUnknown) * (field_F4_number_of_segments));
    field_100_pRes = reinterpret_cast<AnimationUnknown*>(*field_FC_ppRes);

    for (s32 i = 0; i < field_F4_number_of_segments; i++)
    {
        AnimationUnknown* pSegment = &field_100_pRes[i];
        pSegment = new (pSegment) AnimationUnknown(); // We have memory but no constructor was called.. so use placement new to get a constructed instance
        pSegment->field_4_flags.Set(AnimFlags::eBit3_Render);
        pSegment->field_68_anim_ptr = &field_20_animation;
        pSegment->field_C_render_layer = field_20_animation.field_C_render_layer;
        pSegment->field_6C_scale = field_CC_sprite_scale;
        pSegment->field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        pSegment->field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    }

    field_104_bEnabled = 0;
}

ParamiteWeb::~ParamiteWeb()
{
    ResourceManager::FreeResource_49C330(field_FC_ppRes);
}

void ParamiteWeb::VUpdate()
{
    if (field_104_bEnabled == 1)
    {
        field_FA_ttl_remainder -= 8;
        if (field_FA_ttl_remainder <= field_F8_ttl)
        {
            mFlags.Set(BaseGameObject::eDead);
        }
    }
}

void ParamiteWeb::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

void ParamiteWeb::VRender(PrimHeader** ppOt)
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);
    if (field_C2_lvl_number == gMap.mCurrentLevel && field_C0_path_number == gMap.mCurrentPath)
    {
        if (field_B8_xpos >= FP_FromInteger(camCoords.field_0_x) && field_B8_xpos <= FP_FromInteger(camCoords.field_0_x + 1024))
        {
            const FP cam_y = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;
            const FP cam_x = pScreenManager_5BB5F4->field_20_pCamPos->field_0_x;

            s16 minY = FP_GetExponent(FP_FromInteger(field_F8_ttl) - cam_y);
            s16 maxY = FP_GetExponent(FP_FromInteger(field_FA_ttl_remainder) - cam_y);

            s16 ypos_int = FP_GetExponent(field_BC_ypos);
            if (ypos_int > field_FA_ttl_remainder)
            {
                ypos_int = field_FA_ttl_remainder + (ypos_int - field_FA_ttl_remainder) % field_F6_segment_length;
            }

            const s16 x_start = FP_GetExponent(field_B8_xpos - cam_x);

            s16 y_start = FP_GetExponent((FP_FromInteger(ypos_int)) - cam_y);
            if (y_start > 240)
            {
                y_start = y_start % field_F6_segment_length + 240;
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

            field_20_animation.vRender_40B820(640, 240, ppOt, 0, 0);

            if (y_start >= minY)
            {
                for (s32 idx = 0; idx < field_F4_number_of_segments; ++idx)
                {
                    s16 r = 128;
                    s16 g = 128;
                    s16 b = 128;
                    ShadowZone::ShadowZones_Calculate_Colour(FP_GetExponent(field_B8_xpos), ypos_int - (idx * field_F6_segment_length), field_D6_scale, &r, &g, &b);
                    field_100_pRes[idx].field_8_r = static_cast<u8>(r);
                    field_100_pRes[idx].field_9_g = static_cast<u8>(g);
                    field_100_pRes[idx].field_A_b = static_cast<u8>(b);
                    field_100_pRes[idx].vRender_40B820(x_start, y_start, ppOt, 0, 0);
                    PSX_RECT rect = {};
                    field_100_pRes[idx].GetRenderedSize_40C980(&rect);
                    pScreenManager_5BB5F4->InvalidateRect_40EC90(rect.x, rect.y, rect.w, rect.h, pScreenManager_5BB5F4->field_3A_idx);
                    ClipPoly_Vertically_4A09E0(&field_100_pRes[idx].field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index], minY, maxY);
                    y_start -= field_F6_segment_length;
                    if (y_start < minY)
                    {
                        break;
                    }
                }
            }
        }
    }
}
