#include "stdafx.h"
#include "Blood.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "PsxDisplay.hpp"

BaseGameObject* Blood::VDestructor(signed int flags)
{
    return vDtor_40F5A0(flags);
}

void Blood::VUpdate()
{
    vUpdate_40F650();
}

void Blood::VRender(int** pOrderingTable)
{
    vRender_40F780(pOrderingTable);
}

void Blood::VScreenChanged()
{
    vScreenChanged_40FAD0();
}

Blood* Blood::ctor_40F0B0(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, __int16 count)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    
    SetVTable(this, 0x544200); // vTbl_Blood_544200

    field_CC_sprite_scale = scale;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kBloodropResID);
    Animation_Init_424E10(308, 11, 7, ppRes, 1, 1);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit25_bDecompressDone);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_8_r = 127;
    field_20_animation.field_9_g = 127;
    field_20_animation.field_A_b = 127;

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_12C_render_layer = 36;
    }
    else
    {
        field_12C_render_layer = 17;
    }

    field_126_total_count = count;
    field_122_to_render_count = count;

    field_F4_ppResBuf = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_Blood, 0, count * sizeof(BloodParticle));
    if (field_F4_ppResBuf)
    {
        field_F8_pResBuf = reinterpret_cast<BloodParticle*>(*field_F4_ppResBuf);
        field_128_timer = 0;
        field_B8_xpos = xpos - FP_FromInteger(12);
        field_BC_ypos = ypos - FP_FromInteger(12);
        field_11E_xpos = FP_GetExponent(xpos - FP_FromInteger(12) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
        field_120_ypos = FP_GetExponent(ypos - FP_FromInteger(12) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit13_Is8Bit))
        {
            // 8 bit
            field_11C_texture_mode = 1;
        }
        else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
        {
            // 16 bit
            field_11C_texture_mode = 2;
        }
        else
        {
            // 4 bit
            field_11C_texture_mode = 0;
        }

        BYTE u0 = field_20_animation.field_84_vram_rect.x & 63;
        if (field_11C_texture_mode == 1)
        {
            u0 = 2 * u0;
        }
        else if (field_11C_texture_mode == 0)
        {
            u0 = 4 * u0;
        }

        BYTE v0 = field_20_animation.field_84_vram_rect.y & 0xFF;

        FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[field_20_animation.Get_FrameHeader_40B730(-1)->field_0_frame_header_offset]);

        const short frameW = pFrameHeader->field_4_width;
        const short frameH = pFrameHeader->field_5_height;

        field_20_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending);

        for (int i = 0; i < field_126_total_count; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                BloodParticle* pParticle = &field_F8_pResBuf[i];
                Prim_Sprt* pSprt = &pParticle->field_10_prims[j];
                Sprt_Init_4F8910(pSprt);
                Poly_Set_SemiTrans_4F8A60(&pSprt->mBase.header, 1);

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit16_bBlending))
                {
                    Poly_Set_Blending_4F8A20(&pSprt->mBase.header, 1);
                }
                else
                {
                    Poly_Set_Blending_4F8A20(&pSprt->mBase.header, 0);
                    SetRGB0(pSprt, field_20_animation.field_8_r, field_20_animation.field_9_g, field_20_animation.field_A_b);
                }

                SetClut(pSprt,
                    static_cast<short>(
                        PSX_getClut_4F6350(
                            field_20_animation.field_8C_pal_vram_xy.field_0_x,
                            field_20_animation.field_8C_pal_vram_xy.field_2_y)));

                SetUV0(pSprt, u0, v0);
                pSprt->field_14_w = frameW - 1;
                pSprt->field_16_h = frameH - 1;
            }
        }

        // Has its own random seed based on the frame counter.. no idea why
        field_124_rand_seed = static_cast<BYTE>(sGnFrame_5C1B84);
        for (int i = 0; i < field_122_to_render_count; i++)
        {
            field_F8_pResBuf[i].field_0_x = FP_FromInteger(field_11E_xpos);
            field_F8_pResBuf[i].field_4_y = FP_FromInteger(field_120_ypos);

            const FP randX = FP_FromInteger(sRandomBytes_546744[field_124_rand_seed++]) / FP_FromInteger(16);
            const FP adjustedX = FP_FromDouble(1.3) * (randX - FP_FromInteger(8));
            field_F8_pResBuf[i].field_8_offx = field_CC_sprite_scale * (xOff + adjustedX);

            const FP randY = FP_FromInteger(sRandomBytes_546744[field_124_rand_seed++]) / FP_FromInteger(16);
            const FP adjustedY = FP_FromDouble(1.3) * (randY - FP_FromInteger(8));
            field_F8_pResBuf[i].field_C_offy = field_CC_sprite_scale * (yOff + adjustedY);
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    return this;

}

void Blood::dtor_40F5D0()
{
    SetVTable(this, 0x544200); // vTbl_Blood_544200
    if (field_F4_ppResBuf)
    {
        ResourceManager::FreeResource_49C330(field_F4_ppResBuf);
    }
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

BaseGameObject* Blood::vDtor_40F5A0(signed int flags)
{
    dtor_40F5D0();
    if (flags & 1)
    {
        alive_delete_free(this);
    }
    return this;
}

void Blood::vUpdate_40F650()
{
    if (field_128_timer > 0)
    {
        if (field_128_timer > 5)
        {
            field_122_to_render_count -= 10;
        }

        if (field_122_to_render_count <= 0)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_122_to_render_count = 0;
            return;
        }

        for (int i=0; i<field_122_to_render_count; i++)
        {
            field_F8_pResBuf[i].field_C_offy += FP_FromDouble(1.8);

            field_F8_pResBuf[i].field_8_offx = field_F8_pResBuf[i].field_8_offx * FP_FromDouble(0.9);
            field_F8_pResBuf[i].field_C_offy = field_F8_pResBuf[i].field_C_offy * FP_FromDouble(0.9);
            
            field_F8_pResBuf[i].field_0_x += field_F8_pResBuf[i].field_8_offx;
            field_F8_pResBuf[i].field_4_y += field_F8_pResBuf[i].field_C_offy;
        }
    }

    field_128_timer++;
}

void Blood::vRender_40F780(int** pOt)
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos,
        0))
    {
        PSX_Point xy = { 32767, 32767 };
        PSX_Point wh = { -32767, -32767 };

        for (int i = 0; i < field_122_to_render_count; i++)
        {
            BloodParticle* pParticle = &field_F8_pResBuf[i];
            Prim_Sprt* pSprt = &pParticle->field_10_prims[gPsxDisplay_5C1130.field_C_buffer_index];

            BYTE u0 = field_20_animation.field_84_vram_rect.x & 63;
            if (field_11C_texture_mode == 1)
            {
                u0 *= 2;
            }
            else if (field_11C_texture_mode == 0)
            {
                u0 *= 4;
            }
            
            SetUV0(pSprt, u0, field_20_animation.field_84_vram_rect.y & 0xFF);

            FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[field_20_animation.Get_FrameHeader_40B730(-1)->field_0_frame_header_offset]);

            pSprt->field_14_w = pFrameHeader->field_4_width - 1;
            pSprt->field_16_h = pFrameHeader->field_5_height - 1;

            const short x0 = PsxToPCX(FP_GetExponent(pParticle->field_0_x));
            const short y0 = FP_GetExponent(pParticle->field_4_y);

            SetXY0(pSprt, x0, y0);

            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit16_bBlending))
            {
                SetRGB0(pSprt, field_20_animation.field_8_r, field_20_animation.field_9_g, field_20_animation.field_A_b);
            }

            OrderingTable_Add_4F8AA0(
                &pOt[field_12C_render_layer],
                &pSprt->mBase.header);

            if (x0 < xy.field_0_x)
            {
                xy.field_0_x = x0;
            }

            if (x0 > wh.field_0_x)
            {
                wh.field_0_x = x0;
            }

            if (y0 < xy.field_2_y)
            {
                xy.field_2_y = y0;
            }

            if (y0 > wh.field_2_y)
            {
                wh.field_2_y = y0;
            }
        }
        
        const int tpage = PSX_getTPage_4F60E0(
            static_cast<char>(field_11C_texture_mode),
            0,
            field_20_animation.field_84_vram_rect.x,
            field_20_animation.field_84_vram_rect.y);

        Prim_SetTPage* pTPage = &field_FC_tPages[gPsxDisplay_5C1130.field_C_buffer_index];
        Init_SetTPage_4F5B60(pTPage, 0, 0, static_cast<short>(tpage));
        OrderingTable_Add_4F8AA0(&pOt[field_12C_render_layer], &pTPage->mBase);

        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            (xy.field_0_x - 12),
            (xy.field_2_y - 12),
            (wh.field_0_x + 12),
            (wh.field_2_y + 12),
            pScreenManager_5BB5F4->field_3A_idx);
    }
}

void Blood::vScreenChanged_40FAD0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}
