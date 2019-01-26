#include "stdafx.h"
#include "Water.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"

Water* Water::ctor_4E02C0(Path_Water* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x547F10); // vTbl_Water_547F10
 
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kWdropResID);
    if (ppRes)
    {
        Animation_Init_424E10(104, 11, 7, ppRes, 1, 1);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit25_bDecompressDone);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

        Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSplashResID);

        field_114_tlvInfo = tlvInfo;
        field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
        field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

        field_104_top_left = pTlv->field_8_top_left;
        field_108_bottom_right = pTlv->field_C_bottom_right;

        field_104_top_left.field_0_x += -FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
        field_104_top_left.field_2_y += -FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
        
        field_108_bottom_right.field_0_x += -FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
        field_108_bottom_right.field_2_y += -FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

        field_124_tlv_data = pTlv->field_10_data;

        // Limit upper bound
        if (field_124_tlv_data.field_10_max_drops > 128)
        {
            field_124_tlv_data.field_10_max_drops = 128;
        }

        field_130_splash_x_vel = field_124_tlv_data.field_16_splash_x_velocity << 8;
        field_134_emit_x_vel = field_124_tlv_data.field_16_splash_x_velocity << 8;
        field_118_radius = FP_FromInteger((field_108_bottom_right.field_0_x - field_104_top_left.field_0_x) / 2);
        field_11C_centre = FP_FromInteger(field_104_top_left.field_0_x) + field_118_radius;

        field_138_splash_time = 0;

        field_F4_ppWaterRes = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_Water, 0, field_124_tlv_data.field_10_max_drops * sizeof(Water_Res));
        if (field_F4_ppWaterRes)
        {
            field_F8_pWaterRes = reinterpret_cast<Water_Res*>(*field_F4_ppWaterRes);
            field_FC_state = pTlv->field_1_unknown;

            if (field_FC_state == 2)
            {
                field_140_time_on = sGnFrame_5C1B84 + field_124_tlv_data.field_1A_timeout;
            }

            field_148_bHitTimeout &= ~1u;
            field_10C_update_count = 0;
            field_10E_current_particle_idx = 0;

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit13_Is8Bit))
            {
                field_FE_texture_mode = 1;
            }
            else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
            {
                field_FE_texture_mode = 2;
            }
            else
            {
                field_FE_texture_mode = 0;
            }

            BYTE u0 = field_20_animation.field_84_vram_rect.x & 63;
            if (field_FE_texture_mode == 1)
            {
                u0 = 2 * u0;
            }
            else if (field_FE_texture_mode == 0)
            {
                u0 = 4 * u0;
            }

            const BYTE v0 = field_20_animation.field_84_vram_rect.y & 0xFF;

            const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[field_20_animation.Get_FrameHeader_40B730(-1)->field_0_frame_header_offset]);
            field_120_frame_width = pFrameHeader->field_4_width;
            field_122_frame_height = pFrameHeader->field_5_height;

            const BYTE u1 = pFrameHeader->field_4_width + u0 - 1;
            const BYTE v1 = pFrameHeader->field_5_height + v0 - 1;

            const int tPage = PSX_getTPage_4F60E0(
                static_cast<char>(field_FE_texture_mode),
                3,
                field_20_animation.field_84_vram_rect.x,
                field_20_animation.field_84_vram_rect.y);

            for (int i = 0; i < field_124_tlv_data.field_10_max_drops; i++)
            {
                field_F8_pWaterRes[i].field_18_enabled = 0;
                // HACK/OG BUG: PC only uses first poly ??
                Poly_FT4* pPoly = field_F8_pWaterRes[i].field_20_polys;

                PolyFT4_Init_4F8870(pPoly);
                Poly_Set_SemiTrans_4F8A60(&pPoly->mBase.header, TRUE);
                Poly_Set_Blending_4F8A20(&pPoly->mBase.header, TRUE);

                const int clut = PSX_getClut_4F6350(
                    field_20_animation.field_8C_pal_vram_x.field_0_x,
                    field_20_animation.field_8C_pal_vram_x.field_2_y);

                SetClut(pPoly, static_cast<short>(clut));
                SetTPage(pPoly, static_cast<short>(tPage));

                SetUV0(pPoly, u0, v0);
                SetUV1(pPoly, u1, v0);
                SetUV2(pPoly, u0, v1);
                SetUV3(pPoly, u1, v1);
            }

            field_100_screen_x = FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
            field_102_screen_y = FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

            PSX_RECT rect = {};
            rect.y = field_20_animation.field_8C_pal_vram_x.field_2_y;
            rect.x = field_20_animation.field_8C_pal_vram_x.field_0_x + 1;
            rect.w = 1;
            rect.h = 1;

            // Some sort of hack to set the first 2 pixels to black/transparent?
            const BYTE zeroedData[4] = {};
            PSX_LoadImage_4F5FB0(&rect, zeroedData);

            field_144_sound_channels = 0;
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }
    else
    {
        field_6_flags.Clear(BaseGameObject::eDrawable);
        field_6_flags.Set(BaseGameObject::eDead);
    }

    return this;
}
